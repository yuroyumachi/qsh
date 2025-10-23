
#include "qsh.h"
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
	char *cmd;
	char **args;
	int status;

	puts ("Que SHell");

	do {
		printf ("] ");
		cmd = input ();
		if (cmd == NULL) {
			return 0;
		}
		args = split_line (cmd);

		status = qsh_exec (args);

		if (args != NULL) {
			for (int i = 0; args[i]; ++i)
				free (args[i]);
			free (args);
		}

		if (cmd)
			free (cmd);
	} while (status != QSH_EXIT);

	return 0;
}
