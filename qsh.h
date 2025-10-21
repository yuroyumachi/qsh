
#ifndef QSH_H
#define QSH_H

#include <stdio.h>

char *qsh_input(void);
char **qsh_split_line(char *string);

int qsh_exec(char **args);

#endif
