#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLENGTH 100

static void usage (void);
bool read_line (FILE *, char *, size_t);

int
main (int argc, char *argv[])
{
  if (argc != 2)
    {
      usage ();
    }

  char *file_name = argv[1];
  FILE *file = fopen (file_name, "r");
  char *buffer = calloc (1, MAXLENGTH);

  while (read_line(file, buffer, sizeof(buffer))) 
    {
      printf("%s", buffer);
    }
  free (buffer);
  return EXIT_SUCCESS;
}

bool read_line (FILE *f, char *buffer, size_t size)
{
    if (f != NULL && buffer != NULL) {
        char *ret = fgets(buffer, size, f);
        return ret != NULL;
    }
    return false;
}

static void
usage (void)
{
  printf ("cat, print the contents of a file\n");
  printf ("usage: cat FILE\n");
}
