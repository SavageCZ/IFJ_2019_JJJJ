/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Diviš Jan       xdivis12
 *          Pojsl Jakub	    xpojsl00
 *
 *      File: precedential_analysis.h
 */

#include <stdio.h>
#include <stdlib.h>
#include "generator.h"

//enum of symbol indexes for prec. table
typedef enum
{
    LS,     //1
    GT,     //2
    LSEQ,   //3
    GTEQ,   //4
    EQ,     //5
    NEQ,    //6
    PLUS,   //7
    MINUS,  //8
    MUL,    //9
    DIV,    //10
    IDIV,   //11
    LBR,    //12
    RBR,    //13
	TERM,	//14
	ASSIGN, //15
    DOLLAR  //16

} Prec_table_symbol;

int reduce();
Prec_table_symbol get_symbol_index(symbols symbol);
symbols get_symbol(Token* token);
int symbol_count();
void pop_n(int n, stack_top_t* stack);
int expression(prog_data* data);
