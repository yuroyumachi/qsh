
#include "qsh.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MINIMUM_BUFFER_SIZE 4
char *
qsh_input (void)
{
	char *buffer = (char *)malloc (MINIMUM_BUFFER_SIZE * sizeof (char));
	if (buffer == NULL)
		return NULL;

	int length = 0;
	int capacity = MINIMUM_BUFFER_SIZE;

	int ch;
	while ((ch = getchar ()) != EOF && ch != '\n') {
		buffer[length++] = (char)ch;

		if (length >= capacity) {
			capacity *= 2;
			char *tmp = (char *)realloc (buffer,
						     capacity * sizeof (char));

			if (tmp == NULL) {
				free (buffer);
				return NULL;
			} else {
				buffer = tmp;
			}
		}
	}

	if (length == 0 && ch == EOF) {
		free (buffer);
		return NULL;
	}

	buffer[length] = '\0';
	return buffer;
}

// 参数: s - 用于处理的字符串, pos - 字符串的起始位置
// 返回: 以字符串的形式返回从 *pos 开始遇到的第一个记号
// 说明: 在结束处理后, pos会设定为结束处理的位置(也就是下一个记号的位置)
//	如果没有更多的记号可供返回, 返回 NULL
char *
tokenizing (char *s, int *pos)
{
	if (s[*pos] == '\0') {
		return NULL;
	}

	int capacity = 4;
	int length = 0;
	char *buffer = malloc (capacity * sizeof (char));
	if (buffer == NULL) {
		return NULL;
	}
	char quote_ch = '\0';
	int i;
	for (i = *pos; s[i] != '\0'; i++) {
		if (quote_ch) {
			if (quote_ch == s[i]) {
				quote_ch = '\0';
			} else {
				goto buffer_append;
			}
		} else if (s[i] == '\"' || s[i] == '\'') {
			quote_ch = s[i];
		} else if (isspace (s[i])) {
			if (length > 0) {
				break;
			}
		} else {
			goto buffer_append;
		}
		continue;

	buffer_append:
		buffer[length++] = s[i];
		if (length >= capacity) {
			capacity *= 2;
			char *tmp = realloc (buffer, capacity * sizeof (char));
			if (tmp == NULL) {
				free (buffer);
				return NULL;
			}
			buffer = tmp;
		}
	}
	buffer[length] = '\0';
	if (buffer[0] == '\0') {
		free (buffer);
		return NULL;
	}
	*pos = i;
	return buffer;
}

// 参数: s - 要切割的字符串
// 返回: 返回指向一串字符的指针
// 说明: 如果 alloc 失败, 返回 NULL
char **
qsh_split_line (char *s)
{
	int capacity = 4;
	int length = 0;
	char **args = malloc (capacity * sizeof (char *));
	if (args == NULL) {
		return NULL;
	}
	int pos = 0;
	while ((args[length++] = tokenizing (s, &pos)) != NULL) {
		if (length >= capacity) {
			char **tmp
			    = realloc (args, capacity * sizeof (char *));
			if (tmp == NULL) {
				goto alloc_error_cleanup;
			}
			args = tmp;
		}
	}
	args[length] = NULL;
	return args;

alloc_error_cleanup:
	for (int i = 0; i < length; ++i) {
		free (args[i]);
	}
	free (args);
	return NULL;
}
