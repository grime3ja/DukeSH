#ifndef __cs361_builtins__
#define __cs361_builtins__

#include <stdbool.h>

typedef struct directory directory_t;

int echo (char *);
int export (char *);
int cd (char *);
int pwd ();
int unset (char *);
int which (char *);

#endif
