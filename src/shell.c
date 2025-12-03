#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "hash.h"
#include "model.h"
#include "process.h"
#include "shell.h"

// No command line can be more than 100 characters
#define MAXLENGTH 100

void
shell (FILE *input)
{
  hash_init (100);
  hash_insert ("?", "0");
  char buffer[MAXLENGTH];

  fsm_t *cmdmodel = cmdline_init ();

  while (1)
    {
      // Print the cursor and get the next command entered
      printf ("$ ");
      memset (buffer, 0, sizeof (buffer));
      if (fgets (buffer, MAXLENGTH, input) == NULL)
        break;

      if (input != stdin)
        printf ("%s", buffer);

      // is the command quit? if so, break the loop
      if (strncmp (buffer, "quit", 4) == 0)
        {
          break;
        }
      // is the command echo? if so, enter the builtins.c:echo function
      else if (strncmp (buffer, "echo", 4) == 0)
        {
          echo (buffer);
        }
      else if (strncmp (buffer, "pwd", 3) == 0)
        {
          pwd ();
        }
      else if (strncmp (buffer, "cd", 2) == 0)
        {
          char *temp = strtok (strtok (buffer, "cd"), " ");
          cd (temp);
        }
      else if (strncmp (buffer, "which", 5) == 0)
        {
          which (buffer);
        }
      // TODO these two functions in builtins.c
      else if (strncmp (buffer, "export", 6) == 0)
        {
          export (buffer);
        }
      else if (strncmp (buffer, "unset", 5) == 0)
        {
          unset (buffer);
        }
      else
        {
          fflush (stdout);
          // if the command line argument is not a built in command,
          // it must be a utility function, thus we will start a FSM

          if (cmdmodel == NULL)
            {
              break;
            }

          size_t length = strlen (buffer);

          char temp[length];
          memset (temp, 0, sizeof (temp));
          // replace "\n" with "NL" to allow the FSM to run the execution
          if (strncmp ("/usr", buffer, 4) == 0)
            snprintf (temp, length - 3, "./%s", buffer + 5);
          else
            snprintf (temp, length, "%s", buffer);
          strncat (temp, " NL", 4);
          char *event_str = strtok (temp, " ");

          // fill in fsm struct
          while (event_str != NULL)
            {
              event_t event = lookup (event_str);
              cmdmodel->current_token = event_str;
              event_str = strtok (NULL, " ");
              handle_event (cmdmodel, event);
            }
          cmdmodel->state = Init;
        }
    }
  printf ("\n");
  hash_destroy ();
  free (cmdmodel);
}