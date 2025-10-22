
#ifndef QSH_H
#define QSH_H

enum { QSH_SUCCESS = 0, ALLOC_ERROR = -1, QSH_EXIT = -7 };

#include <stdio.h>

char *qsh_input (void);
char **qsh_split_line (char *string);

int qsh_exec (char **args);

#endif
