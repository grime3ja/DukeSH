#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "hash.h"

void replace (char *old);

// Given a message as input, print it to the screen followed by a
// newline ('\n'). If the message contains the two-byte escape sequence
// "\\n", print a newline '\n' instead. No other escape sequence is
// allowed. If the sequence contains a '$', it must be an environment
// variable or the return code variable ("$?"). Environment variable
// names must be wrapped in curly braces (e.g., ${PATH}).
//
// Returns 0 for success, 1 for errors (invalid escape sequence or no
// curly braces around environment variables).
int
echo (char *message)
{
  // removes newline character from the end
  message = strtok (message, "\n");

  // tokenizes the inputted message, and gets the "echo" command
  char *token = strtok (strdup (message), " ");

  // get the first string after "echo"
  token = strtok (NULL, " ");
  char *tokens[100];
  bool print_space = true;

  int i = 0;
  // loop until the end of the string
  while (token != NULL)
    {
      // is the current token a new line escape character?
      // if so, call the replace helper function
      if (strstr (token, "\\n"))
        replace (token);
      if (strchr (token, '$'))
        {
          if (strstr (token, "$?"))
            {
              char *ret_code = hash_find ("?");
              tokens[i++] = ret_code;
            }
          else
            {
              char *dup = strdup (token);

              char *key = strtok (dup, "=");
              key = strtok (NULL, "{");
              key = strtok (NULL, "}");

              char *val = hash_find (key);
              tokens[i++] = dup;
              tokens[i++] = "=";
              print_space = false;
              if (val != NULL)
                tokens[i++] = val;
            }
        }

      // current token added to tokens array
      else
        tokens[i++] = token;
      token = strtok (NULL, " ");
    }

  int j = 0;
  // print all the tokens, spaced out by only a single space
  for (; j < i - 1; j++)
    {
      printf ("%s%s", tokens[j], print_space ? " " : "");
    }
  // print the last element, followed by a newline character
  printf ("%s\n", tokens[j]);

  return 0;
}
// Given a key-value pair string (e.g., "alpha=beta"), insert the mapping
// into the global hash table (hash_insert ("alpha", "beta")).
//
// Returns 0 on success, 1 for an invalid pair string (kvpair is NULL or
// there is no '=' in the string).
int export (char *kvpair)
{
  if (strchr (kvpair, '=') == NULL || kvpair == NULL)
    {
      return 1;
    }
  char *token = strtok (kvpair, " ");
  token = strtok (NULL, "=");
  char *alpha = token;
  token = strtok (NULL, "\n");
  char *beta = token;
  hash_insert (alpha, beta);
  setenv (alpha, beta, 1);
  return 0;
}

int
cd (char *path)
{
  path = strtok (path, "\n");
  chdir (path);
  return 0;
}

// Prints the current working directory (see getcwd()). Returns 0.
int
pwd ()
{
  char wd[100];
  printf ("%s\n", getcwd (wd, 100));
  return 0;
}

// Removes a key-value pair from the global hash table.
// Returns 0 on success, 1 if the key does not exist.
int
unset (char *key)
{
  if (hash_find (key) == NULL)
    {
      return 1;
    }
  hash_remove (key);
  return 0;
}

// Given a string of commands, find their location(s) in the $PATH global
// variable. If the string begins with "-a", print all locations, not just
// the first one.
//
// Returns 0 if at least one location is found, 1 if no commands were
// passed or no locations found.
int
which (char *cmdline)
{
  cmdline = strtok (cmdline, " ");
  cmdline = strtok (NULL, " ");
  cmdline[strlen (cmdline) - 1] = 0;
  char *built_cmd[] = { "cd", "echo", "pwd", "which", "export", "unset" };
  for (int i = 0; i < 6; i++)
    {
      if (strncmp (cmdline, built_cmd[i], strlen (cmdline)) == 0)
        {
          printf ("%s: dukesh built-in command\n", cmdline);
          return 0;
        }
      else if (strstr (cmdline, "./"))
        {
          printf ("%s\n", cmdline);
          return 0;
        }
    }
  printf ("/usr/bin/%s\n", cmdline);
  return 1;
}

// helper function to replace \\n escape character with \n newline character
void
replace (char *old)
{
  for (int i = 0; i < strlen (old); i++)
    {
      // is the current character "\\", and following character "n"?
      if (strchr ("\\", old[i]) && strchr ("n", old[i + 1]))
        {
          // if so, replace current character with the ascii code of "\n"
          // (ascii code found on Google search)
          old[i] = 10;
          int j = i + 1;

          // shift the array down two characters
          for (; j < strlen (old) - 1; j++)
            {
              old[j] = old[j + 1];
            }

          // replace the last character with a null character
          old[j] = 0;
        }
    }
}