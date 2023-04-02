/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Kopáček Jiří    xkopac06
 *
 *      File: scanner.h
 */


#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "custom_string.h"
#include "stack.h"

/**
 * @enum States
 */
typedef enum
{
    // Start
    stSTART,

    // IDs
    stID_KEYWORD,

    // Number
    stNUM,
    stNUM_ZERO,
    stNUM_POINT,
    stNUM_DEC,
    stNUM_E,
    stNUM_SIGN,
    stNUM_EXP,

    // String
    stSTRING_UNFINISHED,
    stSTRING_ESCAPE,
    stSTRING_ESCAPE_HEX,
    stSTRING_ESCAPE_HEX_HALF,
    //stSTRING,

    // Doc string
    stDOC_STRING_A,
    stDOC_STRING_B,
    stDOC_STRING_UNFINISHED,
    stDOC_STRING_ESCAPE,
    stDOC_STRING_UNFINISHED_A,
    stDOC_STRING_UNFINISHED_B,
    //stDOC_STRING,

    // Comment
    stCOMMENT,

    // Indentation
    stINDENTATION_COUNT,

    // Special characters
    //stCOLON,
    //stCOMMA
    //stLBRACKET,
    //stRBRACKET,

    // Operators
    //stPLUS,
    //stMINUS,
    //stMULTI,
    stINT_DIV,
    //stFLOAT_DIV,

    // Comparators
    stGREATER,
    //stGREATER_EQUAL,
    stLESSER,
    //stLESSER_EQUAL,
    stASSIGN,
    //stEQUAL,
    stEXCL,
    //stNOT_EQUAL,

} tState;


/**
 * @enum Token types
 */
typedef enum
{
    TOKEN_IDENTIFIER, //0
    TOKEN_KEYWORD,

    TOKEN_NUM, //2
    TOKEN_NUM_DEC,
    TOKEN_NUM_EXP,

    TOKEN_STRING, //5

    TOKEN_DOC_STRING, //6

    //TOKEN_COMMENT,

    TOKEN_INDENT, //7
    TOKEN_DEDENT, //8

    TOKEN_COLON, //9
    TOKEN_COMMA,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,

    TOKEN_PLUS, //13
    TOKEN_MINUS,
    TOKEN_MULTI,
    TOKEN_INT_DIV,
    TOKEN_FLOAT_DIV,

    TOKEN_GREATER, //18
    TOKEN_GREATER_EQUAL,
    TOKEN_LESSER, //20
    TOKEN_LESSER_EQUAL,
    TOKEN_ASSIGN, //22
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL, //24

    TOKEN_EOL, // 25
    TOKEN_EOF, //26
    TOKEN_UNDEFINED
} tokenType;

/**
 * @struct Token struct
 */
typedef struct
{
    tokenType type;
    tString *attribute;
} Token;

/**
 * @var Stack for indentation counting
 */
tStack indent_stack;

/**
 * @var Equals 1 if currently dedenting, 0 otherwise
 */
int state_dedenting;

/**
 * @var Indentation value that is being searched for in the stack
 */
int searched_value;

/**
 * @var Equals 1 when EOF has been found, 0 otherwise
 */
int eof_reached;

/**
 * @brief Checks if the character is a letter
 * @param c Checked character
 * @return 1 if letter, 0 otherwise
 */
int isCharAlpha(char c);

/**
 * @brief Checks if the character is a digit
 * @param c Checked character
 * @return 1 if digit, 0 otherwise
 */
 int isCharDigit(char c);

/**
 * @brief Checks if the string is a keyword
 * @param s Checked string
 * @return 1 if keyword, 0 otherwise
 */
int isStringKeyword(tString *s);

/**
 * @brief Finishes all required operations before returning a token
 * @param token Current token
 * @return 0
 */
int finalize(Token *token);

/**
 * @brief Tries to find the searched indentation value on the stack
 * @param token Current token
 * @return 0 if found, 1 if empty, -1 and dedent token if not found
 */
int dedent_stack(Token *token);

/**
 * @brief Removes one value and generates dedent, or generates eof if empty stack
 * @param token Current token
 * @return 0
 */
int empty_stack(Token *token);

/**
 * @brief Reads the next token
 * @param token Reference to the token to rewrite with a new one
 * @return 0 if success, 1 if lex_error, 99 if internal error
 */
int get_token(Token *token, FILE *file);

#endif // SCANNER_H_INCLUDED
