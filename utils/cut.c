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
  // Print the cursor and get the next command entered.
  char *filename = argv[argc-1];
  FILE *file = fopen(filename, "r");
  char line[LINELEN];
  if(argc == 2){
    while(fgets(line, LINELEN ,file) != NULL){
      if(strstr(line, " ") == NULL){
        printf("%s", line);
      }else{
        char* token = strtok(line," ");
        printf("%s\n", token);
      }
    }
  }
  else if(strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-f") == 0){
    char* delimiter = argv[2];
    int num = strtol(argv[4], NULL,10);
    int counter = 0;
    char c;
    while((c = fgetc(file)) != EOF){
      if(c == '\n'){
        counter++;
      }
    }
    fseek(file, 0, SEEK_SET);
    while (fgets (line, LINELEN, file))
      {
        char *token = strtok (line, delimiter);
        for (int i = 0; i < num - 1; i++)
          {
            token = strtok (NULL, delimiter);
          }
        if(((strlen(line) / 2)) + 1 < num){
          for(int i = 0; i < counter; i++ ){
            printf("\n");
          }
          break;
        }
        if (strchr (token, 10))
          {
            printf ("%s", token);
            break;
          }
        printf ("%s\n", token);
      }
  }
  else if(strcmp(argv[1], "-f") == 0){
    int counter = 0;
    int num = strtol(argv[2], NULL,10);
    if(num < 2){
      usage();
      return EXIT_FAILURE;
    }
    while (fgets (line, LINELEN, file))
      {
        char *token = strtok (line, " ");
        for (int i = 0; i < num - 1; i++)
          {
            token = strtok (NULL, " ");
            counter += 1;
          }
        if (strchr (token, 10))
          {
            printf ("%s", token);
            if(counter + 1 == num){
              printf("\n");
            }
            break;
          }
        printf ("%s\n", token);
      }
  }
  else if(strcmp(argv[1], "-d") == 0){
    char* delimiter = argv[2];
    while(fgets(line, LINELEN ,file) != NULL){
      char* token = strtok(line, delimiter);
      printf ("%s\n", token);
    }
  }
  fclose (file);
  return EXIT_SUCCESS;
}

static void
usage (void)
{
  printf ("cut, splits each line based on a delimiter\n");
  printf ("usage: cut [FLAG] FILE\n");
  printf ("FLAG can be:\n");
  printf ("  -d C     split each line based on the character C (default ' ')\n");
  printf ("  -f N     print the Nth field (1 is first, default 1)\n");
  printf ("If no FILE specified, read from STDIN\n");
}
