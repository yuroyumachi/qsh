
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "qsh.h"

int qsh_exec(char **args) {
	pid_t pid;
	// pid_t wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		if (execvp(args[0], args) == -1) {
			perror("?");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0) {
		perror("?");
	}
	else {
		do {
			pid_t wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 0;
}
