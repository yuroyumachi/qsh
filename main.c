
#include "qsh.h"
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
	char *cmd;
	char **args;

	while (1) {
		printf ("] ");
		cmd = qsh_input ();
		if (cmd == NULL) {
			perror ("?");
		}
		args = qsh_split_line (cmd);

		qsh_exec (args);

		if (args != NULL) {
			for (int i = 0; args[i]; ++i)
				free (args[i]);
			free (args);
		}

		if (cmd)
			free (cmd);
	}

	return 0;
}
