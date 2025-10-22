
#include "qsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int qsh_cd (char **args);
int qsh_bye (char **args);
int qsh_help (char **args);

const char *builtin_str[] = { "cd", "bye", "help" };

int (*builtin_func[]) (char **) = { &qsh_cd, &qsh_bye, &qsh_help };

int
builtin_amount ()
{
	return sizeof (builtin_str) / sizeof (char *);
}

// 切换工作目录, 然后打印出当前目录
int
qsh_cd (char **args)
{
	if (args[1] != NULL) {
		if (chdir (args[1]) != 0) {
			perror ("?");
		}
	}

	char *buffer = getcwd (NULL, 0);
	if (buffer == NULL) {
		perror ("?");
		return -1;
	}
	printf ("%s\n", buffer);
	free (buffer);

	return QSH_SUCCESS;
}

// 永远返回 QSH_EXIT
int
qsh_bye (char **args)
{
	return QSH_EXIT;
}

//
int
qsh_help (char **args)
{
	puts ("qsh - v0.1.0");
	puts ("available builtins:");

	for (int i = 0; i < builtin_amount (); ++i) {
		printf ("\t%s\n", builtin_str[i]);
	}

	return QSH_SUCCESS;
}

int
launch (char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork ();
	if (pid == 0) {
		if (execvp (args[0], args) == -1) {
			perror ("?");
		}
		exit (EXIT_FAILURE);
	} else if (pid < 0) {
		perror ("?");
	} else {
		do {
			wpid = waitpid (pid, &status, WUNTRACED);
		} while (!WIFEXITED (status) && !WIFSIGNALED (status));
	}

	return QSH_SUCCESS;
}

int
qsh_exec (char **args)
{
	if (args[0] == NULL) {
		return 0;
	}

	for (int i = 0; i < builtin_amount (); ++i) {
		if (strcmp (args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i]) (args);
		}
	}

	return launch (args);
}
