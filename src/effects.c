#define _POSIX_C_SOURCE 200809L // needed for strdup extension

#include <assert.h>
#include <fcntl.h>
#include <inttypes.h>
#include <spawn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "builtins.h"
#include "effects.h"
#include "hash.h"
#include "model.h"
#include "pipe.h"

/* Executed when starting to process a new command line. The fsm_t
   should have been updated to include a pointer to the current token.
   For instance, if the command line was "ls -l data NL", the fsm_t
   has a field that points to "ls". */
void
start_command (fsm_t *cmdmodel)
{
  cmdmodel->command = cmdmodel->current_token;
  cmdmodel->args = calloc (1, 100);
  cmdmodel->args[cmdmodel->nargs++] = cmdmodel->current_token;
}

/* Executed when processing a token after the command name. For instance,
   if the command line was "ls -l data NL", this function will be called
   when the current token is "-l" and again when it is "data". */
void
append (fsm_t *cmdmodel)
{
  if (cmdmodel->nargs >= 100)
    return;

  assert (cmdmodel->args != NULL);

  cmdmodel->args[cmdmodel->nargs++] = cmdmodel->current_token;
}

/* Executed when either a NL or | (pipe) is encountered. For instance, if
   the command line is "ls -l data NL", the current token will be "NL"; also,
   the FSM's args array should be complete, containing "ls", "-l", and "data",
   followed by several NULL pointers. */
void
execute (fsm_t *cmdmodel)
{
  assert (cmdmodel->args != NULL);

  // setup openning and using directories
  size_t length = cmdmodel->nargs;

  char *util_name = cmdmodel->args[0];

  char *args[length + 2];
  int i = 0;
  args[i++] = util_name;
  while (cmdmodel->args[i])
    {
      args[i] = cmdmodel->args[i];
      i++;
    }
  args[i] = NULL;

  if (!fopen (args[i - 1], "r") && strncmp (util_name, "./bin/env", 9) != 0)
    {
      char *file_name = "pipe.txt";

      args[i] = file_name;
      args[i + 1] = NULL;
    }

  pid_t pid = -1;
  extern char **environ;
  if (strncmp (cmdmodel->current_token, "|", 1) == 0)
    spawn_result (args);
  else
    {
      if (strncmp ("./", util_name, 2) != 0)
        {
          char *bin = calloc (BUFFER_LENGTH, sizeof (char));
          snprintf (bin, strlen (util_name) + 7, "./bin/%s", util_name);
          if (!fopen (bin, "r"))
            {
              snprintf (bin, strlen (util_name) + 10, "/usr/bin/%s",
                        util_name);
            }
          util_name = strdup (bin);
          free (bin);
        }
      int status = posix_spawn (&pid, util_name, NULL, NULL, args, environ);
      waitpid (pid, &status, 0);
      int return_code = WEXITSTATUS (status);
      char temp[4];
      snprintf (temp, 4, "%d", return_code);
      hash_insert ("?", temp);
      remove ("pipe.txt");
    }

  memset (&cmdmodel->args, 0x0, 100);
  free (cmdmodel->args);
  cmdmodel->nargs = 0;
}

// No changes are needed to the effects below

void
link_commands (fsm_t *cmdmodel)
{
  execute (cmdmodel);
}

void
error_pipe (fsm_t *cmdmodel)
{
  fprintf (stderr, "ERROR: Received token %s while in state %s\n",
           cmdmodel->current_token, state_name (cmdmodel->state));
}

void
error_newline (fsm_t *cmdmodel)
{
  fprintf (stderr, "ERROR: Received token %s while in state %s\n",
           cmdmodel->current_token, state_name (cmdmodel->state));
}
