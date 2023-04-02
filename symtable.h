/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Kopáček Jiří    xkopac06
 *
 *      File: symtable.h
 */


#ifndef SYMTABLE_H_INCLUDED
#define SYMTABLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "custom_string.h"

// ============================================================== EXTERNAL

/**
 * @struct Variable/function data structure
*/
typedef struct symdata
{
    int param_count; // -1 by default
    bool defined; // false by default
} tSymdata;

typedef struct tBSTNode {
	char *key;
	int isVariable;
	tSymdata *data;
	struct tBSTNode *LPtr;
	struct tBSTNode *RPtr;
} *tBSTNodePtr;

/**
 * @brief Initializes the symtable
 * @param symtable Pointer to the symtable
*/
void symtable_init(tBSTNodePtr *symtable);

/**
 * @brief Inserts an empty function into the symtable without defined properties
 * @param symtable Pointer to the symtable
 * @param key Name of the function
 * @return 0 if ok, 99 if error
*/
int symtable_create_function(tBSTNodePtr *symtable, char *key);

/**
 * @brief Inserts an empty variable into the symtable without defined properties
 * @param symtable Pointer to the symtable
 * @param key Name of the variable
 * @return 0 if ok, 99 if error
*/
int symtable_create_variable(tBSTNodePtr *symtable, char *key);

/**
 * @brief Searches for the function in the symtable and stores a reference to it's data in the parameter
 * @param symtable Pointer to the symtable
 * @param key Name of the function
 * @param foundFunction Pointer to a symbol data pointer, stores the search result (NULL if not found)
 * @return 0 if found, 1 if not
*/
int symtable_search_function(tBSTNodePtr *symtable, char *key, tSymdata **foundFunction);

/**
 * @brief Searches for the variable in the symtable and stores a reference to it's data in the parameter
 * @param symtable Pointer to the symtable
 * @param key Name of the variable
 * @param foundVariable Pointer to a symbol data pointer, stores the search result (NULL if not found)
 * @return 0 if found, 1 if not
*/
int symtable_search_variable(tBSTNodePtr *symtable, char *key, tSymdata **foundVariable);

/**
 * @brief Creates a chain of tokens for syntax analysis... or just deletes a symbol, try to guess
 * @param symtable Pointer to the symtable
 * @param key Name of the symbol
 */
void symtable_delete_symbol(tBSTNodePtr *symtable, char *key);

/**
 * @brief Checks for undefined functions in the symtable
 * @param symtable Pointer to the symtable
 * @return 0 if all defined, 1 if not defined
 */
int symtable_contains_undefined(tBSTNodePtr *symtable);

/**
 * @brief Frees and destroys the symtable
 * @param symtable Pointer to the symtable
 */
void symtable_dispose(tBSTNodePtr *symtable);


// ============================================================== INTERNAL

/**
 * @brief Frees the tSymdata structure
 * @param data Pointer to the data structure
*/
void BST_free_data(tSymdata *data);

/**
 * @brief Tree initialization
 * @param RootPtr Pointer to the root
*/
void BST_init(tBSTNodePtr *RootPtr);

/**
 * @brief Searches for the node based on the given key
 * @param RootPtr Pointer to the root
 * @param key Name of the searched node
 * @param foundNode Pointer to the pointer to save the result of the search
 * @return 0 if found, 1 if not found
*/
int BST_search(tBSTNodePtr *RootPtr, char *key, tBSTNodePtr *foundNode);

/**
 * @brief Inserts a new node (or rewrites and old one)
 * @param RootPtr Pointer to the root
 * @param key Name of the inserted node
 * @param isVar 0 if function, 1 if variable
 * @param content Pointer to the inserted structure
 * @return 0 if ok, 1 if error
*/
int BST_insert(tBSTNodePtr *RootPtr, char *key, int isVar, tSymdata *content);

/**
 * @brief Deletes the node based on the given key
 * @param RootPtr Pointer to the root
 * @param key Name of the deleted node
*/
void BST_delete (tBSTNodePtr *RootPtr, char *key);

/**
 * @brief Deletes the entire tree, recursive
 * @param RootPtr Pointer to the root
*/
void BST_dispose(tBSTNodePtr *RootPtr);

/**
 * @brief Testing function
 * @param RootPtr Pointer to the root
 * @param indent Indentation when printing (for testing pass 0)
*/
void BST_print(tBSTNodePtr *RootPtr, int indent);



#endif // SYMTABLE_H_INCLUDED
