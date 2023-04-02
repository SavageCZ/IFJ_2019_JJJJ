/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Kopáček Jiří    xkopac06
 *          Pojsl Jakub	    xpojsl00
 *
 *      File: custom_string.c
 */

#include <stdlib.h>
#include <string.h>
#include "custom_string.h"

void string_clear(tString *s)
{
	s->length = 0;
	s->str[0] = '\0';
}

int string_init(tString *s)
{
    if (s == NULL)
    {
        return 1;
    }

	if ((s->str = (char *) malloc(STRINGLEN * sizeof(char))) == NULL)
	{
		return 1;
	}
	else
	{
		string_clear(s);
		s->allocSize = STRINGLEN;
		return 0;
	}
}

void string_free(tString *s)
{
	free(s->str);
}

int string_append_char(tString *s, char c)
{
    if (s == NULL)
    {
        return 1;
    }
	if (s->length + 1 >= s->allocSize - RESERVED)
	{
		if ((s->str = (char *) realloc(s->str, (s->length + STRINGLEN) * sizeof(char))) == NULL)
		{
			return 1;
		}
		s->allocSize = s->allocSize + STRINGLEN;
	}
	s->str[s->length] = c;
	s->length += 1;
	s->str[s->length] = '\0';
	return 0;
}

int string_concat(tString *s1, tString *s2)
{
    if (s1 == NULL || s2 == NULL)
    {
        return 1;
    }
	int new_size = s1->length + s2->length;
	if (new_size >= s1->allocSize)
	{
		if ((s1->str = (char *) realloc(s1->str, new_size * sizeof(char))) == NULL)
		{
			return 1;
		}
		s1->allocSize = new_size;
	}

	s1->length += s2->length;
	strcat(s1->str, s2->str);
	return 0;
}

int string_copy(tString *s1, tString *s2)
{
    if (s1 == NULL || s2 == NULL)
    {
        return 1;
    }
    if (s1->length >= s2->allocSize)
    {
        if ((s2->str = (char *) realloc(s2->str, s1->allocSize * sizeof(char))) == NULL)
        {
            return 1;
        }
        s2->allocSize = s1->allocSize;
    }

    s2->length = s1->length;
    strcpy(s2->str, s1->str);
    return 0;
}

int string_compare(tString *s1, tString* s2)
{
    if (s1 == NULL || s2 == NULL)
    {
        return -99;
    }

    return strcmp(s1->str, s2->str);
}

int string_compare_char(tString *s1, char* s2)
{
    if (s1 == NULL || s2 == NULL)
    {
        return -99;
    }

    return strcmp(s1->str, s2);
}

int string_print(tString *s)
{
    for (int i = 0; i < s->length; i++)
    {
        printf("%c", s->str[i]);
    }
    printf("\n");
    return 0;
}


