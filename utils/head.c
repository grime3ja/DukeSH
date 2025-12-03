#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// You may assume that lines are no longer than 1024 bytes
#define LINELEN 1024

static void usage (void);

int
main (int argc, char *argv[])
{
  if (argc < 2)
    {
      usage ();
      return EXIT_FAILURE;
    }

  int n = 5;
  if (strcmp(argv[1], "-n") == 0)
    {
      n = strtol(argv[2], NULL, 10);
    }
  else if (argc > 3)
    {
      return EXIT_FAILURE;
    }
  char *filename = argv[argc - 1];
  FILE *file = fopen (filename, "r");
  char line[LINELEN];
  for (int i = 0; i < n; i++)
    {
      if (fgets (line, LINELEN, file))
        printf ("%s", line);
      // else
      //   {
      //     printf ("\n");
      //     break;
      //   }
    }
  return EXIT_SUCCESS;
}

static void
usage (void)
{
  printf ("head, prints the first few lines of a file\n");
  printf ("usage: head [FLAG] FILE\n");
  printf ("FLAG can be:\n");
  printf ("  -n N     show the first N lines (default 5)\n");
  printf ("If no FILE specified, read from STDIN\n");
}
