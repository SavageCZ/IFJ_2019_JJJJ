/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Diviš Jan       xdivis12
 *
 *      File: stack.h
 */


#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 30


//enum of symbols for stack and analysis
typedef enum
{
    S_LS,     	//1
    S_GT,     	//2
    S_LSEQ,   	//3
    S_GTEQ,   	//4
    S_EQ,     	//5
    S_NEQ,    	//6
    S_PLUS,   	//7
    S_MINUS,  	//8
    S_MUL,    	//9
    S_DIV,    	//10
    S_IDIV,   	//11
    S_LBR,    	//12
    S_RBR,    	//13
	S_ASSIGN, 	//14
	S_ID,		//15
	S_INT,		//16
	S_STR,		//17
	S_FLOAT,	//18
	S_NONE,		//19
    S_DOLLAR,  	//20
	S_STOP,		//21
	S_NONTERM,	//22
    S_UNDEF     //23
} symbols;

// Struct for int stack
typedef struct {
	int arr[MAX_STACK_SIZE];
	int top;
} tStack;

/**
 * @brief Initializes the stack
 * @param s stack
 * @return 0 if successful, 99 if null
 */
int stack_init (tStack* s);

/**
 * @brief Checks if the stack is empty
 * @param s stack
 * @return 1 if empty, 0 if not, 99 if null
 */
int stack_empty (tStack* s);

/**
 * @brief Checks if the stack is full
 * @param s stack
 * @return 1 if full, 0 if not, 99 if null
 */
int stack_full (tStack* s);

/**
 * @brief Rewrites x with the value at the top of the stack
 * @param s stack
 * @param x int to store the top in
 * @return 0 if successful, 1 if the stack is empty, 99 if null
 */
int stack_top (tStack* s, int* x);

/**
 * @brief Pops the stack
 * @param s stack
 * @return 0 if successful, 1 if the stack is empty, 99 if null
 */
int stack_pop (tStack* s);

/**
 * @brief Pushes x to the stack
 * @param s stack
 * @param x int to push
 * @return 0 if successful, 1 if the stack is full, 99 if null
 */
int stack_push (tStack* s, int x);

//----------------------------------------------------------INT STACK END--------------------------------------
//----------------------------------------------------------STACK/LIST FOR EXPRESSIONS------------------------------

// More like list item
typedef struct stack_item
{
	symbols symbol; // Symbol enum
	struct stack_item *next;
} stack_item_t;

// Top of the stack, basically pointer to the beginning of a list
typedef struct stack_top
{
	stack_item_t *top;
} stack_top_t;

// Stack init
void init(stack_top_t* stack);

// Stack push
int push(stack_top_t* stack, symbols symbol);

// Stack pop
void pop(stack_top_t* stack);

// Inserts new stop symbol in the stack
int insert_stop_symbol(stack_top_t* stack);

// Returns first terminal from the stack
stack_item_t* find_terminal(stack_top_t* stack);

// Frees the stack
void stack_free(stack_top_t* stack);


#endif // STACK_H_INCLUDED
