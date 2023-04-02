/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Kopáček Jiří    xkopac06
 *          Pojsl Jakub	    xpojsl00
 *          Sasín Jonáš	    xsasin05
 *
 *      File: parser.h
 */


#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "symtable.h"
#include "token_list.h"
#include "generator.h"

///various errors that can occur during the analysis
#define LEX_OK			            0
#define SYNTAX_OK 					0
#define LEX_ERR         			1   //lexical analysis error
#define SYNTAX_ERR					2   //syntactic analysis error
#define SEM_UNDEF_ERR       		3   //undefined variable or function
#define SEM_TYPE_ERR    			4   //types in expression incompatible
#define SEM_PARAM_ERR    			5   //param count doesnt match in function call
#define SEM_ERR_OTHER               6   //other semantic errors
#define SEM_ZERO_ERR                9   //dividing by zero error
#define ERROR_INTERNAL             99   //internal compiler error

///the main structure for parser data distribution
typedef struct {
    tBSTNodePtr global_table;        /// Global table of symbols
    tBSTNodePtr local_table;        /// Local table of symbols

    FILE *file;     ///the file from which the scanner gets tokens - usually stdin

    Token token;     ///actual loaded token

    bool token_loaded;  ///tells us if new token has been loaded

    bool in_function;   ///tells us if the parser is currently in function body

    tDLList expression_list; ///list of tokens that form expression and is used by prec. analysis

    tSymdata *current_fun_data; ///pointer to current function data

    int if_count;       ///index of current if-condition to generate unique labels in ifjcode19
    int while_count;    ///index of current while-cycle to generate unique labels in ifjcode19

} prog_data;


/**
 * @brief Main parser function
 * @return Returns error code
 */
int analyse();

/**
 * functions representing the rules of grammar
 * Each function returns error code and uses structure prog_data
 * for the analysis and generating the code
 */
int program(prog_data* data);
int statement(prog_data* data);
int def_function(prog_data* data);
int term(prog_data* data);
int term_n(prog_data* data);
int print(prog_data* data);
int print_n(prog_data* data);
int param(prog_data* data);
int param_n(prog_data* data);
int statement_fun(prog_data* data);
int return_value(prog_data* data);
int idwhat(prog_data* data);
int assign(prog_data* data);

/**
 * @brief adds predefined functions into global table
 * @param data contains data needed for the analysis and generating the code
 * @return Returns error code
 */
int add_predefined(prog_data* data);

/**
 * @brief Main precedential analysis function
 * @param data contains data needed for the analysis and generating the code
 * @return Returns error code
 */
int expression(prog_data* data);


#endif // PARSER_H_INCLUDED
