/*
 * string.c
 *
 *  Created on: 27.07.2012
 *      Author: pascal
 */

#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"

char *strcpy(char *to, const char *from)
{
	size_t i;
	for(i = 0; from[i] != '\0'; i++)
		to[i] = from[i];
	//'\0' muss auch kopiert werden
	to[i] = from[i];
	return to;
}

char *strncpy(char *to, const char *from, size_t size)
{
	size_t i;
	for(i = 0; from[i] != '\0' && i < size; i++)
			to[i] = from[i];

	//'\0' muss auch kopiert werden falls i !>= size
	if(i < size)
	{
		for(; i < size; i ++)
			to[i] = '\0';
	}
	return to;
}

int strcmp(const char *str1, const char *str2)
{
	size_t i;
	for(i = 0; str1[i] != '\0' && str2[i] != '\0'; i++)
	{
		if(str1[i] == str2[i])
			continue;
		return str1[i] - str2[i];
	}
	if(str1[i] == str2[i])
		return 0;
	return str1[i] - str2[i];
}

int strncmp(const char *str1, const char *str2, size_t size)
{
	size_t i;
	for(i = 0; str1[i] != '\0' && str2[i] != '\0' && i < size; i++)
	{
		if(str1[i] == str2[i])
			continue;
		return str1[i] - str2[i];
	}
	if(str1[i] == str2[i])
		return 0;
	return str1[i] - str2[i];
}

size_t strlen(const char *cs)
{
	size_t i;
	for(i = 0; cs[i] != '\0'; i++);
	return i;
}

void *strdup(const char *s)
{
	char *new;
	new = malloc(strlen(s) + 1);
	if(new)
		memcpy(new, s, strlen(s) + 1);
	return new;
}

char *strtok(char *string, const char *delimiters)
{
	static char *last;
	char *del, *token;
	char sc, dc;

	if(string == NULL)
	{
		if(last == NULL)
			return NULL;
		else
			string = last;
	}

	//Erste Delimiters überspringen
	cont:
	sc = *string++;
	for(del = (char*)delimiters; (dc = *del) != '\0'; del++)
		if(sc == dc)
			goto cont;

	if(sc == '\0')
	{
		last = NULL;
		return NULL;
	}
	token = string - 1;

	//String nach Delimiters durchsuchen
	while(1)
	{
		sc = *string++;
		del = (char*)delimiters;
		do
		{
			if((dc = *del++) == sc)
			{
				if(sc == '\0')
					string = NULL;
				else
					string[-1] = '\0';
				last = string;
				return token;
			}
		}
		while(dc != '\0');
	}
}

/*
 * Macht praktisch das Selbe wie strtok nur ist sie threadsafe. Achtung: erster Parameter wird verändert
 * Parameter:	str = Adresse auf String. Der eigentliche String und der Zeiger darauf wird verändert!
 * 				delimiters = String mit Zeichen, die die Tokens trennen
 * Rückgabe:	Zeiger auf Token
 */
char *strtok_s(char **str, const char *delimiters)
{
	char *string = *str;
	char *del, *token;
	char sc, dc;

	if(str == NULL || string == NULL)
		return NULL;

	//Erste Delimiters überspringen
	cont:
	sc = *string++;
	for(del = (char*)delimiters; (dc = *del) != '\0'; del++)
		if(sc == dc)
			goto cont;

	if(sc == '\0')
		return NULL;

	token = string - 1;

	//String nach Delimiters durchsuchen
	while(1)
	{
		sc = *string++;
		del = (char*)delimiters;
		do
		{
			if((dc = *del++) == sc)
			{
				if(sc == '\0')
					string = NULL;
				else
					string[-1] = '\0';
				*str = string;
				return token;
			}
		}
		while(dc != '\0');
	}
}

char *strcat(char *str1, const char *str2)
{
	size_t i;
	size_t length = strlen(str1);
	for(i = 0; str2[i] != '\0'; i++)
	{
		str1[length + i] = str2[i];
	}
	str1[length + i] = '\0';
	return str1;
}

char *strncat(char *str1, const char *str2, size_t n)
{
	size_t i;
	size_t length = strlen(str1);
	for(i = 0; str2[i] != '\0' && i < n; i++)
	{
		str1[length + i] = str2[i];
	}
	str1[length + i] = '\0';
	return str1;
}

char *strrchr(const char *str, int ch)
{
	char *s;

	for(s = (char*)str + strlen(str); s >= (char*)str; s--)
	{
		if(*s == ch)
			return s;
	}

	return NULL;
}

char *strchr(const char *str, int ch)
{
	char *s;

	for(s = (char*)str; *s != '\0'; s++)
	{
		if(*s == ch)
			return s;
	}

	//Nullcharakter
	if(*s == ch)
		return s;

	return NULL;
}

size_t strspn(const char *dest, const char *src)
{
	size_t i;

	for(i = 0; dest[i] != '\0'; i++)
	{
		size_t j;
		bool found = false;
		for(j = 0; src[j] != '\0'; j++)
		{
			if(dest[i] == src[j])
			{
				found = true;
				break;
			}
		}
		if(!found)
			break;
	}

	return i;
}

size_t strcspn(const char *dest, const char *src)
{
	size_t i;

	for(i = 0; dest[i] != '\0'; i++)
	{
		size_t j;
		bool found = false;
		for(j = 0; src[j] != '\0'; j++)
		{
			if(dest[i] == src[j])
			{
				found = true;
				break;
			}
		}
		if(found)
			break;
	}

	return i;
}

char *strpbrk(const char *dest, const char *breakset)
{
	for(; *dest != '\0'; dest++)
	{
		size_t i;
		for(i = 0; breakset[i] != '\0'; i++)
		{
			if(*dest == breakset[i])
				return (char*)dest;
		}
	}

	return NULL;
}

char *strstr(const char *str, const char *substr)
{
	char *s;
	size_t size;

	if(substr == NULL)
		return str;

	 size = strlen(substr);

	for(s = str; *s != '\0'; s++)
	{
		if(strncmp(s, substr, size) == 0)
			return s;
	}

	return NULL;
}


void *memchr(const void *ptr, int ch, size_t count)
{
	unsigned char *p;
	for(p = (unsigned char*)ptr; count > 0; p++, count--)
	{
		if(*p == (unsigned char)ch)
			return p;
	}

	return NULL;
}

int memcmp(const void *lhs, const void *rhs, size_t count)
{
	unsigned char *l = (unsigned char*)lhs;
	unsigned char *r = (unsigned char*)rhs;

	size_t i;
	for(i = 0; i < count && l[i] == r[i]; i++);

	return l[i] - r[i];
}

void *memset(void *block, int c, size_t n)
{
	unsigned char volatile *i;
	for(i = block; (uintptr_t)i < (uintptr_t)block + n; i++)
		*i = (unsigned char)c;
	return block;
}

void *memmove(void *to, const void *from, size_t size)
{
	size_t i;
	char *dest = to;
	const char *src = from;
	if(from < to)	//Von hinten nach vorne kopieren
	{
		for(i = size; i > 0 ; i--)
		{
			dest[i - 1] = src[i - 1];
		}
	}
	else			//Von vorne nach hinten kopieren
	{
		for(i = 0; i < size ; i++)
		{
			dest[i] = src[i];
		}
	}
	return to;
}

void *memcpy(void *to, const void *from, size_t size)
{
	size_t i;
	const char *src = from;
	char *dest = to;
	for(i = 0; i < size; i++)
		dest[i] = src[i];
	return to;
}
