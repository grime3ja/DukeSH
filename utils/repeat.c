#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void usage (void);

int
main (int argc, char *argv[])
{
  if (argc < 3)
    {
      usage ();
      return EXIT_FAILURE;
    }

  int index = 1;
  while (argv[index])
    {
      for (int i = 0; i < strtol (argv[index], NULL, 10); i++)
        {
          char *get = getenv (argv[index + 1]);
          size_t length = strlen (argv[index + 1]);
          if (get)
            length += strlen (get);
          char out[length + 3];
          snprintf (out, length + 3, "%s=%s\n", argv[index + 1], get ? get : "");
          write (STDOUT_FILENO, out, strlen (out));
        }
      index += 2;
    }
  
  return EXIT_SUCCESS;
}

static void
usage (void)
{
  printf ("repeat, a tool for printing repeated environment variables\n");
  printf ("usage: repeat N VAR ...\n");
  printf ("each N must be a positive integer\n");
  printf ("N VAR can be repeated, but each repetition must have both\n");
}
