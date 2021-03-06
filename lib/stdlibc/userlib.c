/*
 * userlib.c
 *
 *  Created on: 11.08.2012
 *      Author: pascal
 */

#include "userlib.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"

void initLib()
{
	//stdin
	stdin = calloc(sizeof(FILE), 1);
	stdin->error = IO_NO_ERROR;
	stdin->mode.read = true;
	stdin->stream_id = 0;
	setvbuf(stdin, NULL, _IONBF, BUFSIZ);
	//stdout
	stdout = calloc(sizeof(FILE), 1);
	stdout->error = IO_NO_ERROR;
	stdout->mode.write = true;
	stdout->stream_id = 1;
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	//stderr
	stderr = calloc(sizeof(FILE), 1);
	stderr->error = IO_NO_ERROR;
	stderr->mode.write = true;
	stderr->stream_id = 2;
	setvbuf(stderr, NULL, _IONBF, BUFSIZ);
}
#ifndef BUILD_KERNEL
extern int main(int argc, char *argv[]);

int getArgCount(char *cmd)
{
	int count = 0;

	int i;
	for(i = 0; ; i++)
	{
		if(cmd[i] == '\0')
		{
			count++;
			break;
		}
		else if(cmd[i] == ' ')
			count++;
	}

	return count;
}

void c_main(size_t length, void *data)
{
	initLib();

	int argc = getArgCount(data);
	char *argv[argc + 1];

	argv[0] = strtok(data, " ");

	int i;
	for(i = 1; i < argc; i++)
	{
		argv[i] = strtok(NULL, " ");
	}
	argv[argc] = NULL;

	exit(main(argc, argv));
}
#endif

void reverse(char *s)
{
	size_t i, j;
	for(i = 0, j = strlen(s) - 1; i < j; i++, j--)
	{
		char c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
