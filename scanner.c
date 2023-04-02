/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Kopáček Jiří    xkopac06
 *
 *      File: scanner.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

#define KEYWORD_COUNT 7

tStack indent_stack;
// 1 if the stack is being emptied
int state_dedenting = 0;
// stores the searched indentation value
int searched_value = 0;
// 1 if eof has been reached and the stack is being emptied
int eof_reached = 0;

// Reserved keywords that must not be used as a variable name
char* keywords[KEYWORD_COUNT] =
{
    "def", "else", "if", "None", "pass", "return", "while"
};

// If the last token was EOL, indentation will not be ignored
// If file begins with EOL, the EOL is ignored
int last_token_eol = 1;

int isCharAlpha(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        return 1;
    }
    return 0;
}

int isCharDigit(char c)
{
    if (c >= '0' && c <= '9')
    {
        return 1;
    }
    return 0;
}

int isStringKeyword(tString *s)
{
    if (s == NULL)
    {
        return 0;
    }

    int result;
    for (int i = 0; i < KEYWORD_COUNT; i++)
    {
        result = string_compare_char(s, keywords[i]);
        if (result == 0 && result != -99)
        {
            return 1;
        }
    }
    return 0;
}

int finalize(Token *token)
{
    if (token->type == TOKEN_EOL)
    {
        last_token_eol = 1;
    } else
    {
        last_token_eol = 0;
    }
    return 0;
}

int dedent_stack(Token *token)
{
    int stackTop = 0;
    stack_top(&indent_stack, &stackTop);
    if (stackTop == searched_value)
    {
        // If the value is found, exits dedenting state and continues reading
        state_dedenting = 0;
        searched_value = 0;
        return 0;
    } else if (stackTop == 0)
    {
        // If 0 is reached before finding the searched value, returns lexical error
        return 1;
    } else
    {
        // If the value is not found, pops and generates dedent
        token->type = TOKEN_DEDENT;
        stack_pop(&indent_stack);
        return -1;
    }
}

int empty_stack(Token *token)
{
    int stackTop = 0;
    stack_top(&indent_stack, &stackTop);
    if (stackTop == 0)
    {
        // Generates EOF token once the stack has been emptied
        token->type = TOKEN_EOF;
        return 0;
    } else
    {
        // Generates dedent token for every stack pop until the stack is emptied
        token->type = TOKEN_DEDENT;
        stack_pop(&indent_stack);
        return 0;
    }
}

int get_token(Token *token, FILE *file)
{
    // Initializes all required variables
    char c;
    tState state = stSTART;
    token->type = TOKEN_UNDEFINED;
    string_clear(token->attribute);
    int indentation_counter = 0;
    int hex_char_counter = 0;

    // If eof has been reached, empties the stack while generating dedents
    // Once the stack has been emptied, generates EOF
    if (eof_reached == 1)
    {
        empty_stack(token);
        return finalize(token);
    }

    // If last token was end of line, a new line is being read
    // When a new line is being read, check the indentation first
    if (last_token_eol == 1)
    {
        state = stINDENTATION_COUNT;
    }

    // If the scanner is currently in the dedenting state, dedent instead of reading from the file
    // Tries to find the searched value on the stack
    // (-1)Generates DEDENT for each stack_pop
    // (1)If the stack is emptied without finding the searched value, returns lexical error
    // (0)If the value is found, continues reading
    if (state_dedenting == 1)
    {
        int return_code = 0;
        return_code = dedent_stack(token);
        if (return_code == 1)
        {
            return 1;
        } else if (return_code == -1)
        {
            return finalize(token);
        }
    }

    // Reads the input
    while (file)
    {
        c = getc(file);
        switch(state)
        {
            // State: start
            case stSTART:
                if (isCharAlpha(c) || c == '_') // a...Z
                {
                    state = stID_KEYWORD;
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                }
                else if (c == '0') // 0
                {
                    state = stNUM_ZERO;
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                }
                else if (c >= '1' && c <= '9') // 1...9
                {
                    state = stNUM;
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                }
                else if (c == '\'') // '
                {
                    state = stSTRING_UNFINISHED;
                }
                else if (c == '\"') // "
                {
                    state = stDOC_STRING_A;
                }
                else if (c == '#')
                {
                    state = stCOMMENT;
                }
                else if (c == ' ') // space
                {
                    // If the last token was end of line, starts counting the current indentation
                    if (last_token_eol == 1)
                    {
                        state = stINDENTATION_COUNT;
                        indentation_counter++;
                    } else
                    {
                        state = stSTART;
                    }
                }
                else if (c == '\t' || c == '\n') // tab or eol
                {
                    if (c == '\n')
                    {
                        // Multiple EOLs are ignored
                        if (last_token_eol == 0)
                        {
                            token->type = TOKEN_EOL;
                            return finalize(token);
                        }
                        else {
                            state = stINDENTATION_COUNT;
                            continue;
                        }
                    }
                    state = stSTART;
                }
                else if (c == ':')
                {
                    token->type = TOKEN_COLON;
                    return finalize(token);
                }
                else if (c == ',')
                {
                    token->type = TOKEN_COMMA;
                    return finalize(token);
                }
                else if (c == '(')
                {
                    token->type = TOKEN_LBRACKET;
                    return finalize(token);
                }
                else if (c == ')')
                {
                    token->type = TOKEN_RBRACKET;
                    return finalize(token);
                }
                else if (c == '+')
                {
                    token->type = TOKEN_PLUS;
                    return finalize(token);
                }
                else if (c == '-')
                {
                    token->type = TOKEN_MINUS;
                    return finalize(token);
                }
                else if (c == '*')
                {
                    token->type = TOKEN_MULTI;
                    return finalize(token);
                }
                else if (c == '/')
                {
                    state = stINT_DIV;
                }
                else if (c == '>')
                {
                    state = stGREATER;
                }
                else if (c == '<')
                {
                    state = stLESSER;
                }
                else if (c == '=')
                {
                    state = stASSIGN;
                }
                else if (c == '!')
                {
                    state = stEXCL;
                }
                else if (c == EOF)
                {
                    eof_reached = 1;
                    // Inserts EOL before EOF if there is none
                    if (last_token_eol != 1)
                    {
                        token->type = TOKEN_EOL;
                        return finalize(token);
                    } else
                    {
                        // Pops the stack and generates dedent if the stack is not empty
                        // If the stack is empty, generates eof
                        eof_reached = 1;
                        empty_stack(token);
                        return finalize(token);
                    }
                }
                else
                {
                    // Lexical error
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                }
                break;
            // End of stStart
            // stID_KEYWORD
            case stID_KEYWORD:
                if (isCharAlpha(c) || isCharDigit(c) || c == '_') // Alphanum, _
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stID_KEYWORD;
                } else // ASCII - Alphanum, _
                {
                    ungetc(c, file);
                    if (isStringKeyword(token->attribute) == 1)
                    {
                        token->type = TOKEN_KEYWORD;
                    } else
                    {
                        token->type = TOKEN_IDENTIFIER;
                    }
                    return finalize(token);
                }
                break;
            // End of stID_KEYWORD
            //
            // NUMBER SECTION
            //
            // stNUM_ZERO
            case stNUM_ZERO:
                if (c == '.')
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stNUM_POINT;
                } else if (c == 'E' || c == 'e')
                {
                    if (string_append_char(token->attribute, 'e') != 0)
                    {
                        return 99;
                    }
                    state = stNUM_E;
                } else if (isCharDigit(c))
                {
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                } else
                {
                    ungetc(c, file);
                    token->type = TOKEN_NUM;
                    return finalize(token);
                }
                break;
            // End of stNUM_ZERO
            // stNUM
            case stNUM:
                if (isCharDigit(c))
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stNUM;
                } else if (c == '.')
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stNUM_POINT;
                } else if (c == 'E' || c == 'e')
                {
                    if (string_append_char(token->attribute, 'e') != 0)
                    {
                        return 99;
                    }
                    state = stNUM_E;
                } else
                {
                    ungetc(c, file);
                    token->type = TOKEN_NUM;
                    return finalize(token);
                }
                break;
            // End of stNUM
            // stNUM_POINT
            case stNUM_POINT:
                if (isCharDigit(c))
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stNUM_DEC;
                } else
                {
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                }
                break;
            // End of stNUM_POINT
            // stNUM_DEC
            case stNUM_DEC:
                if (isCharDigit(c))
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stNUM_DEC;
                } else if (c == 'E' || c == 'e')
                {
                    if (string_append_char(token->attribute, 'e') != 0)
                    {
                        return 99;
                    }
                    state = stNUM_E;
                } else
                {
                    ungetc(c, file);
                    token->type = TOKEN_NUM_DEC;
                    return finalize(token);
                }
                break;
            // End of stNUM_DEC
            // stNUM_E
            case stNUM_E:
                if (c == '+')
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stNUM_SIGN;
                } else if (c == '-')
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stNUM_SIGN;
                } else if (isCharDigit(c))
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stNUM_EXP;
                } else
                {
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                }
                break;
            // End of stNUM_E
            // stNUM_SIGN
            case stNUM_SIGN:
                if (isCharDigit(c))
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stNUM_EXP;
                } else
                {
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                }
                break;
            // End of stNUM_SIGN
            // stNUM_EXP
            case stNUM_EXP:
                if (isCharDigit(c))
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stNUM_EXP;
                } else
                {
                    ungetc(c, file);
                    token->type = TOKEN_NUM_EXP;
                    return finalize(token);
                }
                break;
            // End of stNUM_EXP
            //
            // STRING SECTION
            //
            // stSTRING_UNFINISHED
            case stSTRING_UNFINISHED:
                if (c == '\'')
                {
                    token->type = TOKEN_STRING;
                    return finalize(token);
                } else if (c == '\\')
                {
                    state = stSTRING_ESCAPE;
                } else if (c > 31)
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stSTRING_UNFINISHED;
                } else
                {
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                }
                break;
            // End of stSTRING_UNFINISHED
            // stString_ESCAPE
            case stSTRING_ESCAPE:
                if (c == 'x')
                {
                    state = stSTRING_ESCAPE_HEX;
                } else if (c == '\"')
                {
                    if (string_append_char(token->attribute, '\"') != 0)
                    {
                        return 99;
                    }
                    state = stSTRING_UNFINISHED;
                } else if (c == '\'')
                {
                    if (string_append_char(token->attribute, '\'') != 0)
                    {
                        return 99;
                    }
                    state = stSTRING_UNFINISHED;
                } else if (c == 'n')
                {
                    if (string_append_char(token->attribute, '\n') != 0)
                    {
                        return 99;
                    }
                    state = stSTRING_UNFINISHED;
                } else if (c == 't')
                {
                    if (string_append_char(token->attribute, '\t') != 0)
                    {
                        return 99;
                    }
                    state = stSTRING_UNFINISHED;
                } else if (c == '\\')
                {
                    if (string_append_char(token->attribute, '\\') != 0)
                    {
                        return 99;
                    }
                    state = stSTRING_UNFINISHED;
                } else if (c > 31) // Undefined escape sequence
                {
                    if (string_append_char(token->attribute, '\\') != 0)
                    {
                        return 99;
                    }
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stSTRING_UNFINISHED;
                } else
                {
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                }
                break;
            // End of stSTRING_ESCAPE
            // stSTRING_ESCAPE_HEX
            case stSTRING_ESCAPE_HEX:
                if (isCharDigit(c))
                {
                    hex_char_counter = (16 * (c - 48)); // Reduces the value of c to the range 0...9
                    state = stSTRING_ESCAPE_HEX_HALF;
                } else if (c >= 'a' && c <= 'f')
                {
                    hex_char_counter = (16 * (c - 87)); // Reduces the value of c to the range 10...15
                    state = stSTRING_ESCAPE_HEX_HALF;
                } else if (c >= 'A' && c <= 'F')
                {
                    hex_char_counter = (16 * (c - 55)); // Reduces the value of c to the range 10...15
                    state = stSTRING_ESCAPE_HEX_HALF;
                } else
                {
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                }
                break;
            // End of stSTRING_ESCAPE_HEX
            // stSTRING_ESCAPE_HEX_HALF
            case stSTRING_ESCAPE_HEX_HALF:
                if (isCharDigit(c))
                {
                    hex_char_counter += (c - 48); // Reduces the value of c to the range 0...9
                    state = stSTRING_UNFINISHED;
                } else if (c >= 'a' && c <= 'f')
                {
                    hex_char_counter += (c - 87); // Reduces the value of c to the range 10...15
                    state = stSTRING_UNFINISHED;
                } else if (c >= 'A' && c <= 'F')
                {
                    hex_char_counter += (c - 55); // Reduces the value of c to the range 10...15
                    state = stSTRING_UNFINISHED;
                } else
                {
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                }
                // Appends the character represented by hexadecimal number and resets the counter
                if (string_append_char(token->attribute, hex_char_counter) != 0)
                {
                    return 99;
                }
                hex_char_counter = 0;
                break;
            // End of stSTRING_ESCAPE_HEX_HALF
            //
            // DOC_STRING SECTION
            //
            // stDOC_STRING_A
            case stDOC_STRING_A:
                if (c == '\"')
                {
                    state = stDOC_STRING_B;
                } else
                {
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                }
                break;
            // End of stDOC_STRING_A
            // stDOC_STRING_B
            case stDOC_STRING_B:
                if (c == '\"')
                {
                    state = stDOC_STRING_UNFINISHED;
                } else
                {
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                }
                break;
            // End of stDOC_STRING_B
            // stDOC_STRING_UNFINISHED
            case stDOC_STRING_UNFINISHED:
                if (c == '\"')
                {
                    state = stDOC_STRING_UNFINISHED_A;
                } else if (c == '\\')
                {
                    state = stDOC_STRING_ESCAPE;
                } else if (c > 31 || c == '\n')
                {
                    if (string_append_char(token->attribute, c) != 0)
                    {
                        return 99;
                    }
                    state = stDOC_STRING_UNFINISHED;
                } else
                {
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                }
                break;
            // End of stDOC_STRING_UNFINISHED
            // stDOC_STRING_ESCAPE
            case stDOC_STRING_ESCAPE:
                if (c == '\"')
                {
                    // Appends only " if the sequence is \"
                    if (string_append_char(token->attribute, '\"') != 0)
                    {
                        return 99;
                    }
                    state = stDOC_STRING_UNFINISHED;
                } else
                {
                    // Appends \ if the sequence is \X, ungets X and returns to the normal state
                    if (string_append_char(token->attribute, '\\') != 0)
                    {
                        return 99;
                    }
                    ungetc(c, file);
                    state = stDOC_STRING_UNFINISHED;
                }
                break;
            // End of st_DOC_STRING_ESCAPE
            // stDOC_STRING_UNFINISHED_A
            case stDOC_STRING_UNFINISHED_A:
                if (c == '\"')
                {
                    state = stDOC_STRING_UNFINISHED_B;
                } else
                {
                    // Appends the " and returns to the normal docstring state
                    if (string_append_char(token->attribute, '\"') != 0)
                    {
                        return 99;
                    }
                    ungetc(c, file);
                    state = stDOC_STRING_UNFINISHED;
                }
                break;
            // End of stDOC_STRING_UNFINISHED_A
            // stDOC_STRING_UNFINISHED_B
            case stDOC_STRING_UNFINISHED_B:
                if (c == '\"')
                {
                    /*
                    // If the last token was EOL, this docstring is a multiline comment
                    if (last_token_eol == 1)
                    {
                        // Ignores it
                        string_clear(token->attribute);
                        // Makes sure the rest of the line after the docstring is whitespace only
                        while (file)
                        {
                            c = getc(file);
                            if (c == ' ')
                            {
                                continue;
                            } else if (c == '\n' || c == -1)
                            {
                                ungetc(c, file);
                                break;
                            } else
                            {
                                token->type = TOKEN_UNDEFINED;
                                return 1;
                            }
                        }
                        state = stSTART;
                    } else
                    {
                        // Otherwise this docstring is a string literal
                        token->type = TOKEN_STRING;
                        return finalize(token);
                    }
                    */
                    token->type = TOKEN_STRING;
                    return finalize(token);
                } else
                {
                    // Appends " 2 times and returns to the normal docstring state
                    if (string_append_char(token->attribute, '\"') != 0)
                    {
                        return 99;
                    }
                    if (string_append_char(token->attribute, '\"') != 0)
                    {
                        return 99;
                    }
                    ungetc(c, file);
                    state = stDOC_STRING_UNFINISHED;
                }
                break;
            // End of stDOC_STRING_UNFINISHED_B
            //
            // COMMENT SECTION
            //
            // stCOMMENT
            case stCOMMENT:
                if (c != EOF && c != '\n')
                {
                    state = stCOMMENT;
                } else
                {
                    ungetc(c, file);
                    state = stSTART;
                }
                break;
            // End of stCOMMENT
            //
            // INDENTATION SECTION
            //
            // stINDENTATION_COUNT
            case stINDENTATION_COUNT:
                if (c == ' ')
                {
                    indentation_counter++;
                    state = stINDENTATION_COUNT;
                } else if (c == '#' || c == '\n' || c == -1 /*|| c == '"'*/)
                {
                    // Ignores the indentation if the line is empty or commented
                    ungetc(c, file);
                    indentation_counter = 0;
                    state = stSTART;
                } else
                {
                    ungetc(c, file);
                    // Checks if the indent count is greater than the top of the stack
                    int stackTop = 0;
                    stack_top(&indent_stack, &stackTop);
                    // If greater, pushes indent count to the stack and generates indent
                    if (indentation_counter > stackTop)
                    {
                        stack_push(&indent_stack, indentation_counter);
                        token->type = TOKEN_INDENT;
                        return finalize(token);
                    } else if (indentation_counter == stackTop)
                    {
                        // If equal, resets the indentation counter, does not generate a token
                        // Returns to stSTART
                        indentation_counter = 0;
                        state = stSTART;
                    } else
                    {
                        // If lesser, starts dedenting phase and returns 1 dedent
                        state_dedenting = 1;
                        searched_value = indentation_counter;
                        stack_pop(&indent_stack);
                        token->type = TOKEN_DEDENT;
                        return finalize(token);
                    }
                }
                break;
            // End of stINDENTATION_COUNT
            //
            // MISCELLANEOUS  SECTION
            //
            // stINT_DIV
            case stINT_DIV:
                if (c == '/')
                {
                    token->type = TOKEN_INT_DIV;
                    return finalize(token);
                } else
                {
                    ungetc(c, file);
                    token->type = TOKEN_FLOAT_DIV;
                    return finalize(token);
                }
                break;
            // End of stINT_DIV
            // stGREATER
            case stGREATER:
                if (c == '=')
                {
                    token->type = TOKEN_GREATER_EQUAL;
                    return finalize(token);
                } else
                {
                    ungetc(c, file);
                    token->type = TOKEN_GREATER;
                    return finalize(token);
                }
                break;
            // End of stGREATER
            // stLESSER
            case stLESSER:
                if (c == '=')
                {
                    token->type = TOKEN_LESSER_EQUAL;
                    return finalize(token);
                } else
                {
                    ungetc(c, file);
                    token->type = TOKEN_LESSER;
                    return finalize(token);
                }
                break;
            // End of stLESSER
            // stASSIGN
            case stASSIGN:
                if (c == '=')
                {
                    token->type = TOKEN_EQUAL;
                    return finalize(token);
                } else
                {
                    ungetc(c, file);
                    token->type = TOKEN_ASSIGN;
                    return finalize(token);
                }
                break;
            // End of stASSIGN
            // stEXCL
            case stEXCL:
                if (c == '=')
                {
                    token->type = TOKEN_NOT_EQUAL;
                    return finalize(token);
                } else
                {
                    token->type = TOKEN_UNDEFINED;
                    return 1;
                }
        } // End of switch
    }

    return 0;
}
