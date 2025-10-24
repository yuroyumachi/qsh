
#include "qsh.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void
handle_sigint (int signal)
{
	return;
}

int
main (void)
{
	char *cmd;
	char **args;
	int status;

	signal (SIGINT, handle_sigint);

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
