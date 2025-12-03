#include <fcntl.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pipe.h"

/* Re-implementation of the create_child() in pipe.c. Instead of
   using the calls to pipe/fork/dup2/exec, combine the latter three
   into a call to posix_spawn(). Sample call:

     char *sum = spawn_result ("cksum", "data/f1.txt");
     // sum is "3015617425 6 data/f1.txt\n" [with the newline]
   */
char *
// spawn_result (char *const exec, char *const filename)
spawn_result (char *args[])
{
  pid_t pid = -1;

  char *exec = args[0];

  int fd[2];
  pipe (fd);

  posix_spawn_file_actions_t actions;
  posix_spawn_file_actions_init (&actions);

  fd[1] = open ("pipe.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

  posix_spawn_file_actions_adddup2 (&actions, fd[1], STDOUT_FILENO);

  posix_spawn_file_actions_adddup2 (&actions, fd[0], STDIN_FILENO);

  posix_spawn_file_actions_addclose (&actions, fd[0]);

  extern char **environ;
  posix_spawnp (&pid, exec, &actions, NULL, args, environ);
  posix_spawn_file_actions_destroy (&actions);

  char *buffer = calloc (BUFFER_LENGTH, sizeof (char));

  if (pid != 0)
    {
      wait (NULL);
      // close (fd[1]);
      read (fd[1], buffer, BUFFER_LENGTH);

      char *dup = strndup (buffer, strlen (buffer));
      snprintf (dup, strlen (buffer), "%s", buffer);
      free (buffer);
      return dup;
    }

  return NULL;
}
