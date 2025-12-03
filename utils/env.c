#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <spawn.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXLENGTH 100

static void usage (void);

int
main (int argc, char *argv[], char *envp[])
{
  if (argc < 2)
  {
    usage ();
    return EXIT_FAILURE;
  }
  // fflush (stdout);
  
  size_t length = 0;

  // iterate until you get to the last element of envp
  while (envp[++length] != NULL);

  // add a new environment variable
  int i = 1;
  while (!(strstr (argv[i], "./")))
    {
      envp[length++] = argv[i++];
    }
  envp[length] = NULL;
  
  size_t dir_length = strlen (argv[i]) - 3;

  char *directory_name = argv[i++];

  char util_name[dir_length];
  snprintf (util_name, strlen (directory_name), "%s", directory_name + 6);

  char *args[MAXLENGTH];
  memset (&args, 0, MAXLENGTH - 1);
  int j = 0;
  args[j++] = util_name;
  while (argv[i])
    {
      args[j++] = argv[i++];
    }
  args[j++] = directory_name;
  args[j] = NULL;

  pid_t pid = -1;

  // spawn the process that is specified with the environment variable


  int status = posix_spawn (&pid, directory_name, NULL, NULL, args, envp);

  waitpid (pid, &status, 0);

  return WEXITSTATUS (status);
}

static void
usage (void)
{
  printf ("env, set environment variables and execute program\n");
  printf ("usage: env [name=value ...] PROG ARGS\n");
}

