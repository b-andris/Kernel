/*
 * console.h
 *
 *  Created on: 27.01.2015
 *      Author: pascal
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "stddef.h"
#include "stdint.h"
#include "stdbool.h"
#include "lock.h"

#define	CONSOLE_AUTOREFRESH	1
#define CONSOLE_AUTOSCROLL	2

typedef struct{
	uint8_t x, y;
}cursor_t;

typedef struct{
	uint8_t id;
	char *name;
	uint8_t flags;

	void* waitingThread;

	lock_t lock;

	void *buffer;
	uint16_t height, width;
	uint8_t color;
	cursor_t cursor;

	char ansi_buf[16];
	uint8_t ansi_buf_ofs;
	cursor_t saved_cursor;

	char *inputBuffer;
	size_t inputBufferSize, inputBufferStart, inputBufferEnd;
}console_t;

extern console_t initConsole;
extern console_t *activeConsole;

void console_Init();
console_t *console_create(char *name, uint8_t color);
console_t *console_getByName(char *name);
void console_ansi_write(console_t *console, char c);
void console_write(console_t *console, char c);
void console_clear(console_t *console);
void console_clearLine(console_t *console, uint16_t line);
void console_switch(uint8_t page);
void console_changeColor(console_t *console, uint8_t color);
void console_setCursor(console_t *console, cursor_t cursor);

#endif /* CONSOLE_H_ */
