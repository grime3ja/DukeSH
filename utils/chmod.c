#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>

static void usage (void);
void process_string (char *string, mode_t *mode);

int
main (int argc, char **argv)
{
  if (argc < 3)
    {
      usage ();
      return EXIT_FAILURE;
    }
  
  mode_t mode;

  char *file_name = argv[2];

  if ((mode = strtol (argv[1], NULL, 8)) == 0)
    {
      file_name = argv[4];
      char *mode_str = calloc (10, sizeof (char));
      snprintf (mode_str, 12, "%s %s %s", argv[1], argv[2], argv[3]);
      process_string (mode_str, &mode);
      free (mode_str);
    }

  chmod (file_name, mode);

  return EXIT_SUCCESS;
}

void
process_string (char *string, mode_t *mode)
{
  char *modes_str = calloc (3, sizeof (char));
  char *tok = strtok (string, " ");
  // for (int i = 0, j = 0; i < 4; i++, j += 2)
  while (tok)
    {
      char curr[3];
      int temp = 0;

      snprintf (curr, 4, "%s", tok);

      if (strchr (curr, 'r'))
        {
          temp += 4;
        }
      if (strchr (curr, 'w'))
        {
          temp += 2;
        }
      if (strchr (curr, 'x'))
        {
          temp += 1;
        }

      char *dup = strdup (modes_str);
      snprintf (modes_str, strlen (modes_str) + 2, "%s%d", dup, temp);
      tok = strtok (NULL, " ");
    }
    *mode = strtoll (modes_str, NULL, 8);
    free (modes_str);
}

static void
usage (void)
{
  printf ("chmod, changes permissions on a file\n");
  printf ("usage: chmod USR GRP OTH FILE\n\n");
  printf ("USR, GRP, and OTH must be of the rwx format,\n");
  printf ("with - indicating a permission is not allwed.\n");
}
