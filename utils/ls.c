#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void usage (void);
void list_dir (int i, struct dirent **, char *, char *);
int alphasort_no_case (const struct dirent **, const struct dirent **);
void replace_hidden (char **, char *);
void print_perm (mode_t);

// WARNING WARNING WARNING:
// When using opendir and readdir to read directory listings, the files
// are returned in the order they were created. There is randomness to that
// creation that can cause tests to fail. You MUST sort the file names as
// expected in the test case files. To avoid making your task harder than
// it needs to be, we strongly suggest you use scandir instead.

int
main (int argc, char *argv[])
{
  if (argc < 2)
    {
      usage ();
      return EXIT_FAILURE;
    }

  char *flags = NULL;
  
  struct dirent **directory;
  
  size_t length = argc - 1;
  
  if (argc > 2)
    {
      flags = argv[length - 1];
    }

  char *directory_name = argv[length];

  int i = scandir (directory_name, &directory, 0, alphasort_no_case);

  if (i != -1)
    {
      list_dir (i, directory, flags, directory_name);
      free (directory);
      return EXIT_SUCCESS;
    }
  return EXIT_FAILURE;
}

void
list_dir (int i, struct dirent **directory, char *flags, char *dir)
{
  bool show_curr;
  bool hid_size;
  for (int j = 0; j < i; j++)
    {
      show_curr = false;
      hid_size = false;
      char *curr = strdup (directory[j]->d_name);
      size_t length = strlen (curr);
      if (flags != NULL)
      {
          char temp[length + 8];
          snprintf (temp, length + 9, "%s/%s", dir, curr);
          int fd = open (temp, O_RDONLY);
          struct stat file_stats;
          fstat (fd, &file_stats);
          if (strchr (flags, 'a'))
            {
              if (!strchr (flags, 's'))
                show_curr = true;
              else
                hid_size = true;
            }
          if (strchr (flags, 's') &&
          (strncmp (curr, ".", length) != 0 && strncmp (curr, "..", length) != 0))
            {
              if (strncmp (curr, ".", 1) != 0 || hid_size)
                {
                  show_curr = true;
    
                  char *contents = calloc (file_stats.st_size, sizeof (char));
                  ssize_t bytes = read (fd, contents, file_stats.st_size);
                  printf ("%ld ", bytes);

                  free (contents);
                }
            }
          if (strchr (flags, 'p'))
            {
              if (strncmp (curr, ".", length) == 0 || strncmp (curr, "..", length) == 0)
                printf ("d");
              else
                printf ("-");
              
              print_perm (file_stats.st_mode);

              printf (" ");
            }
          snprintf (curr, length + 2, "%s\n", directory[j]->d_name);
          printf ("%s", show_curr ? curr : "");
          // close (fd);
        }
      else
        {
          if (strncmp(curr, ".", 1) != 0)
            {
              printf ("%s\n", curr);
            }
        }
      free (directory[j]);
    }
}  

// helper function for alphasort while ignoring case
// credit: stack overflow led us to strcasecmp from strings.h
int
alphasort_no_case (const struct dirent **a, const struct dirent **b)
{
  char *a_ignore_hidden = strdup ((*a)->d_name);
  char *b_ignore_hidden = strdup ((*b)->d_name);
  
  if (strncmp (a_ignore_hidden, ".", 1) == 0)
    {
      replace_hidden (&a_ignore_hidden, a_ignore_hidden);
    }

  if (strncmp (b_ignore_hidden, ".", 1) == 0)
    {
      replace_hidden (&b_ignore_hidden, b_ignore_hidden);
    }
  
  return strcasecmp (a_ignore_hidden, b_ignore_hidden);
}

void
replace_hidden (char **path, char *original)
{
  if (strlen (*path) > 2)
    {
      snprintf (*path, strlen (*path), "%s", original + 1);
    }
}

void
print_perm (mode_t mode)
{
  if (mode & S_IRUSR)
    printf ("r");
  else
    printf ("-");
  if (mode & S_IWUSR)
    printf ("w");
  else
    printf ("-");
  if (mode & S_IXUSR)
    printf ("x");
  else
    printf ("-");

  if (mode & S_IRGRP)
    printf ("r");
  else
    printf ("-");
  if (mode & S_IWGRP)
    printf ("w");
  else
    printf ("-");
  if (mode & S_IXGRP)
    printf ("x");
  else
    printf ("-");

  if (mode & S_IROTH)
    printf ("r");
  else
    printf ("-");
  if (mode & S_IWOTH)
    printf ("w");
  else
    printf ("-");
  if (mode & S_IXOTH)
    printf ("x");
  else
    printf ("-");
}

static void
usage (void)
{
  printf ("ls, list directory contents\n");
  printf ("usage: ls [FLAG ...] [DIR]\n");
  printf ("FLAG is one or more of:\n");
  printf ("  -a       list all files (even hidden ones)\n");
  printf ("  -p       list permission bitmask\n");
  printf ("  -s       list file sizes\n");
  printf ("If no DIR specified, list current directory contents\n");
  printf ("Permission bitmasks are 10-character strings such as:\n");
  printf ("  -rwxr-x---\n");
  printf ("The first character is d for directories and - for regular files\n");
}
