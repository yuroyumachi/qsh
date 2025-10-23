
#ifndef QSH_H
#define QSH_H

#define QSH_VERSION "v0.1.0"

enum { QSH_SUCCESS = 0, QSH_ALLOC_ERROR = 1, QSH_EXIT = 7 };

#include <stdio.h>

char *input (void);
char *preprocess (char *data);
char **split_line (char *s);

int qsh_exec (char **args);

#endif
