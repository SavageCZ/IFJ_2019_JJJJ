/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Pojsl Jakub	    xpojsl00
 *          Sasín Jonáš	    xsasin05
 *
 *      File: token_list.h
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

typedef struct tDLElem {
        Token token;
        struct tDLElem *rptr;
} *tDLElemPtr;

typedef struct {
    tDLElemPtr First;
    tDLElemPtr Last;
} tDLList;

/**
 * @brief Initializes the empty list
 * @param tDLList Pointer to the list
 * @return void
*/
void DLInitList (tDLList *);

/**
 * @brief Deletes all elements from the list / state after InitList
 * @param tDLList Pointer to the list
 * @return void
*/
void DLDisposeList (tDLList *);

/**
 * @brief Inserts/creates new Token as the new element at the end of the token list and sets the error code
 * @param tDLList Pointer to the list
 * @param token pointer to the token which we create the copy of
 * @param err_code error code that is checked after the function call
 * @return void
*/
void DLInsertLast(tDLList *, Token *token, int *err_code);

/**
 * @brief Deletes the first element from the list
 * @param tDLList Pointer to the list
 * @return void
*/
void DLDeleteFirst (tDLList *);

/**
 * @brief Initializes a new token
 * @param token Pointer to the token to be initialized
 * @param error_code error code that is checked after the function call
 * @return void
*/
void init_token(Token *token, int *error_code);
