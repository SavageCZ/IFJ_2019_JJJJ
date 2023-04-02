/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Kopáček Jiří    xkopac06
 *          Pojsl Jakub	    xpojsl00
 *
 *      File: custom_string.h
 */

#ifndef CUSTOM_STRING_H_INCLUDED
#define CUSTOM_STRING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRINGLEN 8 // Shortest length of string (number of chars)
#define RESERVED 4 // Maximum number of free chars left before reallocation

typedef struct
{
	char *str; //pointer on the beginning of string
	int length; /// number of chars in string including '\0'
	int allocSize; /// number of allocated chars
} tString;

/**
 * @brief initialization of string, allocation
 * @param s pointer to string
 * @return 0 if init was successful, 1 otherwise
*/
int string_init(tString *s);

/**
 * @brief clears the string
 * @param s pointer to string
 */
void string_clear(tString *s);

/**
 * @brief deallocates the string
 * @param s pointer to string
 *
 */
void string_free(tString *s);

/**
 * @brief concatenate two strings
 * @param s1 string to which to append
 * @param s2 string to be appended
 * @return 0 if successful, 1 otherwise
 */
int string_concat(tString *s, tString* s2);

/**
 * @brief appends char by the end of the string
 * @param s pointer to string
 * @param c char to be appended
 * @return 0 if successful, 1 otherwise
 */
int string_append_char(tString *s, char c);

/**
 * @brief Copies the first string to the second
 * @param s1 source string
 * @param s2 destination string
 * @return 0 if successful, 1 otherwise
 */
int string_copy(tString *s1, tString *s2);

/**
 * @brief Compares two strings
 * @param s1
 * @param s2
 * @return 0 if equal, -99 if error, -1/1 if not equal
 */
int string_compare(tString *s1, tString* s2);

/**
 * @brief Compares string and a char array
 * @param s1
 * @param s2
 * @return 0 if equal, -99 if error, -1/1 if not equal
 */
int string_compare_char(tString *s1, char* s2);

/**
 * @brief Prints the string for testing purposes
 * @param s pointer to string
 */
int string_print(tString *s);

#endif // CUSTOM_STRING_H_INCLUDED
