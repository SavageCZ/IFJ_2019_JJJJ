/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Kopáček Jiří    xkopac06
 *          Pojsl Jakub	    xpojsl00
 *          Sasín Jonáš	    xsasin05
 *
 *      File: parser.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

//macros for checking keywords
#define DEF string_compare_char(data->token.attribute, "def") == 0
#define ELSE string_compare_char(data->token.attribute, "else") == 0
#define IF string_compare_char(data->token.attribute, "if") == 0
#define NONE string_compare_char(data->token.attribute, "None") == 0
#define PASS string_compare_char(data->token.attribute, "pass") == 0
#define RETURN string_compare_char(data->token.attribute, "return") == 0
#define WHILE string_compare_char(data->token.attribute, "while") == 0

//macro to check if the token is a value
#define IS_VALUE(token)             \
token.type == TOKEN_STRING ||       \
token.type == TOKEN_NUM ||          \
token.type == TOKEN_NUM_DEC ||      \
token.type == TOKEN_NUM_EXP ||      \
token.type == TOKEN_IDENTIFIER

//macro to check if token could be beginning of an expression (without the id option)
#define IS_EXPR(token)              \
token.type == TOKEN_STRING ||       \
token.type == TOKEN_NUM ||          \
token.type == TOKEN_NUM_DEC ||      \
token.type == TOKEN_NUM_EXP ||      \
token.type == TOKEN_LBRACKET

//macro to check if token is an operator
#define IS_OP(token)                \
token.type == TOKEN_PLUS ||         \
token.type == TOKEN_MINUS ||        \
token.type == TOKEN_MULTI ||        \
token.type == TOKEN_INT_DIV ||      \
token.type == TOKEN_FLOAT_DIV ||    \
token.type == TOKEN_GREATER ||      \
token.type == TOKEN_GREATER_EQUAL || \
token.type == TOKEN_LESSER  ||      \
token.type == TOKEN_LESSER_EQUAL || \
token.type == TOKEN_EQUAL ||        \
token.type == TOKEN_NOT_EQUAL


//checks if we can use the statement/statement_fun rule
#define IS_STATEMENT_FUN (IF || WHILE || PASS || RETURN || IS_VALUE(data->token) || IS_EXPR(data->token))
#define IS_STATEMENT (IF || WHILE || PASS || IS_VALUE(data->token) || IS_EXPR(data->token))

//getting token and checking if lex err didnt occur
#define GET_TOKEN(data)                         \
err = get_token(&(data->token), data->file);    \
if (err != 0) { return err; }

//checking if we got the expected token type
#define CHECK_TOKEN_TYPE(data, token_type)   \
if(data->token.type != token_type) {   \
    return SYNTAX_ERR; }

//checking if token is expected keyword
#define CHECK_KEYWORD(data, keyword)   \
if(string_compare_char(data->token.attribute, keyword) != 0) {   \
    return SYNTAX_ERR; }


// <program> rule
int program(prog_data* data) {

    //error number stored
    int err = 0;
    //if there is no loaded token, get one
    if(data->token_loaded != true) {
        GET_TOKEN(data)
        data->token_loaded = true;
    }

    //<program> -> <def_function> <program>
    if (DEF) {

        err = def_function(data);

        if(err != 0) {
            return err;
        }

        return program(data);
    }

    //<program> -> <statement> <program>
    if (IS_STATEMENT)
    {

        err = statement(data);

        if (err != 0) {
            return err;
        }

        return program(data);
    }

    //<program> -> EOF
    CHECK_TOKEN_TYPE(data, TOKEN_EOF)

    return SYNTAX_OK;

}

// <statement> rule
int statement(prog_data* data)
{
    int err = 0;

    // <statement> -> <expression> EOL <statement>
    if(IS_EXPR(data->token))
    {
        //EXPRESSION
        //we add the whole expression until eol is found into List
        while(data->token.type != TOKEN_EOL)
        {
            DLInsertLast(&data->expression_list, &data->token, &err);
            if (err != 0) {
                return err;
            }
            GET_TOKEN(data)
        }

        //we analyse and generate the expression
        err = expression(data);   //precedential analysis
        if (err != 0) {
            return err;
        }
        DLDisposeList(&data->expression_list);

        //GENERATE
        gen_move_exp_res("exp_result", false);

        CHECK_TOKEN_TYPE(data, TOKEN_EOL)
        GET_TOKEN(data)
        //recursive statement-rule call
        return statement(data);
    }

// <statement> -> id <idwhat> EOL <statement>
    if(data->token.type == TOKEN_IDENTIFIER)
    {

        err = idwhat(data);

        if(err != 0) {
            return err;
        }

        CHECK_TOKEN_TYPE(data, TOKEN_EOL)

        GET_TOKEN(data)

        return statement(data);
    }

//<statement> -> if <expression> : EOL INDENT <statement> DEDENT else : EOL INDENT <statement> DEDENT
    if(IF)
    {
        data->if_count++;
        int if_count = data->if_count; //we store the if_count in case nested-if would occur

        //EXPRESSION
        //we add the whole expression until colon is found into List
        GET_TOKEN(data)
        if (!(IS_VALUE(data->token) || IS_EXPR(data->token)))
            return SYNTAX_ERR;

        while(data->token.type != TOKEN_COLON)
        {
            if(data->token.type == TOKEN_EOL)
                break;
            DLInsertLast(&data->expression_list, &data->token, &err);
            if (err != 0) {
                return err;
            }
            GET_TOKEN(data)
        }

        //analyse and generate the expression
        err = expression(data);   //precedential analysis
        if (err != 0) {
            return err;
        }
        DLDisposeList(&data->expression_list);

        //GENERATE
        gen_if(if_count);

        CHECK_TOKEN_TYPE(data, TOKEN_COLON)

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_EOL)

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_INDENT)

        GET_TOKEN(data)

        //recursive calling of statement rule
        if (IS_STATEMENT)
        {
            err = statement(data);

            if (err != 0) {
                return err;
            }
        }
        else {
            return SYNTAX_ERR;
        }

        CHECK_TOKEN_TYPE(data, TOKEN_DEDENT)

        GET_TOKEN(data)

        CHECK_KEYWORD(data, "else")

        //GENERATE
        gen_else(if_count);

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data,TOKEN_COLON)

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_EOL)

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_INDENT)

        GET_TOKEN(data)

        //recursive statement call
        if (IS_STATEMENT)
        {

            err = statement(data);

            if (err != 0) {
                return err;
            }
        }
        else {
            return SYNTAX_ERR;
        }

        CHECK_TOKEN_TYPE(data, TOKEN_DEDENT)

        //GENERATE
        gen_if_end(if_count);

        GET_TOKEN(data)
        return statement(data);
    }

    //<statement> -> while <expression> : EOL INDENT <statement> DEDENT <statement>
    if(WHILE)
    {

        data->while_count++;
        int while_count = data->while_count; //we store the while_count in case of nested-cycle occurrence

        //EXPRESSION
        //we add the whole expression until colon is found into List
        GET_TOKEN(data)
        if (!(IS_VALUE(data->token) || IS_EXPR(data->token)))
            return SYNTAX_ERR;

        while(data->token.type != TOKEN_COLON)
        {
            if(data->token.type == TOKEN_EOL)
                break;
            DLInsertLast(&data->expression_list, &data->token, &err);
            if (err != 0) {
                return err;
            }
            GET_TOKEN(data)
        }

        //GENERATE
        printf("\nLABEL $while_%d", while_count);

        //analyse and generate the expression
        err = expression(data);   //precedential analysis
        if (err != 0) {
            return err;
        }
        DLDisposeList(&data->expression_list);

        //GENERATE
        gen_while(while_count);

        CHECK_TOKEN_TYPE(data, TOKEN_COLON)

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_EOL)

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_INDENT)

        GET_TOKEN(data)

        //recursive statement call
        if (IS_STATEMENT)
        {

            err = statement(data);

            if (err != 0) {
                return err;
            }
        }
        else {
            return SYNTAX_ERR;
        }

        CHECK_TOKEN_TYPE(data, TOKEN_DEDENT)

        //GENERATE
        gen_while_end(while_count);

        GET_TOKEN(data)
        return statement(data);
    }

    //<statement> -> pass EOL <statement>
    if(PASS)
    {
        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_EOL)

        GET_TOKEN(data)

        return statement(data);
    }

    //<statement> -> Ɛ
    return SYNTAX_OK;

}

// <statement_fun> rule
int statement_fun(prog_data* data)
{
    int err = 0;

    //<statement_fun> -> <expression> EOL <statement_fun>
    if(IS_EXPR(data->token))
    {
        //EXPRESSION
        //we add the whole expression, until eol is found, into List
        while(data->token.type != TOKEN_EOL)
        {
            DLInsertLast(&data->expression_list, &data->token, &err);
            if (err != 0) {
                return err;
            }
            GET_TOKEN(data)
        }

        //analyse and generate the expression
        err = expression(data);   //precedential analysis
        if (err != 0) {
            return err;
        }
        DLDisposeList(&data->expression_list);

        //GENERATE
        gen_move_exp_res("exp_result", false);

        CHECK_TOKEN_TYPE(data, TOKEN_EOL)
        GET_TOKEN(data)
        //recursive statement call
        return statement_fun(data);
    }

    //<statement_fun> -> id <idwhat> EOL <statement_fun>
    if(data->token.type == TOKEN_IDENTIFIER)
    {

        err = idwhat(data);
        if(err != 0) {
            return err;
        }

        CHECK_TOKEN_TYPE(data,TOKEN_EOL)

        GET_TOKEN(data)
        //recursive statement call
        return statement_fun(data);
    }

//<statement_fun> -> if <expression> : EOL INDENT <statement_fun> DEDENT else : EOL INDENT <statement_fun> DEDENT
    if(IF)
    {

        data->if_count++;
        int if_count = data->if_count;

        //EXPRESSION
        //we add the whole expression until colon is found into List
        GET_TOKEN(data)
        if (!(IS_VALUE(data->token) || IS_EXPR(data->token)))
            return SYNTAX_ERR;

        while(data->token.type != TOKEN_COLON)
        {
            if(data->token.type == TOKEN_EOL)
                break;
            DLInsertLast(&data->expression_list, &data->token, &err);
            if (err != 0) {
                return err;
            }
            GET_TOKEN(data)
        }

        //analyse and generate the expression
        err = expression(data);   //precedential analysis
        if (err != 0) {
            return err;
        }
        DLDisposeList(&data->expression_list);

        //GENERATE
        gen_if(if_count);

        CHECK_TOKEN_TYPE(data, TOKEN_COLON)

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_EOL)

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_INDENT)

        GET_TOKEN(data)

        //recursive statement call
        if (IS_STATEMENT_FUN)
        {

            err = statement_fun(data);
            if (err != 0) {
                return err;
            }
        }
        else {
            return SYNTAX_ERR;
        }

        CHECK_TOKEN_TYPE(data, TOKEN_DEDENT)

        GET_TOKEN(data)

        CHECK_KEYWORD(data, "else")

        //GENERATE
        gen_else(if_count);

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data,TOKEN_COLON)

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_EOL)

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_INDENT)

        GET_TOKEN(data)

        if (IS_STATEMENT_FUN)
        {

            err = statement_fun(data);
            if (err != 0) {
                return err;
            }
        }
        else {
            return SYNTAX_ERR;
        }

        CHECK_TOKEN_TYPE(data, TOKEN_DEDENT)

        //GENERATE
        gen_if_end(if_count);

        GET_TOKEN(data)
        return statement_fun(data);
    }

    //<statement_fun> -> while <expression> : EOL INDENT <statement_fun> DEDENT <statement_fun>
    if(WHILE)
    {
        data->while_count++;
        int while_count = data->while_count;
        //EXPRESSION
        //we add the whole expression until colon is found into List
        GET_TOKEN(data)
        if (!(IS_VALUE(data->token) || IS_EXPR(data->token)))
            return SYNTAX_ERR;

        while(data->token.type != TOKEN_COLON)
        {
            if(data->token.type == TOKEN_EOL)
                break;
            DLInsertLast(&data->expression_list, &data->token, &err);
            if (err != 0) {
                return err;
            }
            GET_TOKEN(data)
        }

        //GENERATE
        printf("\nLABEL $while_%d", while_count);

        err = expression(data);   //precedential analysis
        if (err != 0) {
            return err;
        }
        DLDisposeList(&data->expression_list);

        //GENERATE
        gen_while(while_count);

        CHECK_TOKEN_TYPE(data, TOKEN_COLON)

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_EOL)

        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_INDENT)

        GET_TOKEN(data)

        if (IS_STATEMENT_FUN)
        {

            err = statement_fun(data);
            if (err != 0) {
                return err;
            }
        }
        else {
            return SYNTAX_ERR;
        }

        CHECK_TOKEN_TYPE(data, TOKEN_DEDENT)

        //GENERATE
        gen_while_end(while_count);

        GET_TOKEN(data)
        return statement_fun(data);
    }

    //<statement_fun> -> pass EOL <statement_fun>
    if(PASS)
    {
        GET_TOKEN(data)

        CHECK_TOKEN_TYPE(data, TOKEN_EOL)

        GET_TOKEN(data)
        return statement_fun(data);
    }

    //<statement_fun> -> return <return_value> EOL <statement_fun>
    if(RETURN)
    {
        GET_TOKEN(data)

        err = return_value(data);
        //return's expression value is the top of the stack
        if(err != 0) {
            return err;
        }

        //GENERATE
        //we dont need to know the fun name cause we dont generate the jump-over-fun LABEL when we only return
        gen_function_end(false, "none");

        CHECK_TOKEN_TYPE(data, TOKEN_EOL)

        GET_TOKEN(data)

        return statement_fun(data);
    }

    //<statement_fun> -> Ɛ
    return SYNTAX_OK;
}

//<def_function> rule
int def_function(prog_data* data)
{
    //error number stored
    int err = 0;
    //entering the function body
    data->in_function = true;

    //if the function was used before, we need to check if the param count is the same
    bool used_before = false;

    GET_TOKEN(data)

    CHECK_TOKEN_TYPE(data, TOKEN_IDENTIFIER)

    // sem control
    //function already defined
    if(symtable_search_function(&data->global_table, data->token.attribute->str, &data->current_fun_data) == 0)
    {
        //the fun is in symtable because it was used before its definition -> its being defined now
        if (data->current_fun_data->defined == false) {

            data->current_fun_data->defined = true;
            used_before = true;
        }
        //if it was defined before, its redefinition
        else {
            return SEM_UNDEF_ERR;
        }
    }
    //prom with same name already exist - fun name cant be the same
    else if(symtable_search_variable(&data->global_table, data->token.attribute->str, &data->current_fun_data) == 0)
    {
        return SEM_UNDEF_ERR;
    }
    //add function to symtable
    else {
        err = symtable_create_function(&data->global_table, data->token.attribute->str);
        if (err != 0)
            return err;

        //saving pointer to symdata of this function for counting params
        symtable_search_function(&data->global_table, data->token.attribute->str, &data->current_fun_data);
        //function has just been defined
        data->current_fun_data->defined = true;
    }

    //GENERATE
    //saving the function name
    char* fun_name = malloc((32 + data->token.attribute->length) * sizeof(char));
    strcpy(fun_name, data->token.attribute->str);
    gen_function_start(fun_name);

    GET_TOKEN(data)

    CHECK_TOKEN_TYPE(data, TOKEN_LBRACKET)

    //saving the old param count
    int paramCountBefore = data->current_fun_data->param_count;

    //set the current param count to 0
    data->current_fun_data->param_count = 0;

    //count params
    err = param(data);
    if (err != 0) {
        return err;
    }

    //if the function was called/defined before, check if the param count was the same
    if(used_before == true) {
        if (paramCountBefore != data->current_fun_data->param_count) {
            return SEM_PARAM_ERR;
        }
    }

    CHECK_TOKEN_TYPE(data, TOKEN_RBRACKET)

    GET_TOKEN(data)

    CHECK_TOKEN_TYPE(data, TOKEN_COLON)

    GET_TOKEN(data)

    CHECK_TOKEN_TYPE(data, TOKEN_EOL)

    GET_TOKEN(data)

    CHECK_TOKEN_TYPE(data, TOKEN_INDENT)

    GET_TOKEN(data)

    if (IS_STATEMENT_FUN)
    {

        err = statement_fun(data);
        if (err != 0) {
            return err;
        }
    }
    else {
        return SYNTAX_ERR;
    }

    CHECK_TOKEN_TYPE(data, TOKEN_DEDENT)

    //GENERATE
    gen_function_end(true, fun_name);

    //dispose the local table - local variables can no longer be used
    symtable_dispose(&data->local_table);

    //leaving the function body
    data->in_function = false;

    data->token_loaded = false;

    return SYNTAX_OK;
}

//<idwhat> rule
int idwhat(prog_data* data)
{
    //error number stored
    int err = 0;

    //storing the current token so we can look at another
    Token temp;
    init_token(&temp, &err);
    if (err != 0) {
        return err;
    }
    //we copy the token type and token attribute
    temp.type = data->token.type;
    if(string_copy(data->token.attribute, temp.attribute) != 0) {
        return ERROR_INTERNAL;
    }
    //token has been stored as temp

    GET_TOKEN(data)

    //if the next token is operator, we deal with an expression
    if(IS_OP(data->token))
    {
        //EXPRESSION
        //we add the whole expression until eol is found into List

        //WE NEED TO STORE THE PREVIOUS TOKEN FIRST
        DLInsertLast(&data->expression_list, &temp, &err);
        if (err != 0) {
            return err;
        }
        string_free(temp.attribute);
        free(temp.attribute);

        //storing rest of the expression
        while(data->token.type != TOKEN_EOL)
        {
            DLInsertLast(&data->expression_list, &data->token, &err);
            if (err != 0) {
                return err;
            }
            GET_TOKEN(data)
        }

        //analyse and generate the expression
        err = expression(data);   //precedential analysis
        if (err != 0) {
            return err;
        }
        DLDisposeList(&data->expression_list);

        //GENERATE
        //the result is no longer used
        gen_move_exp_res("exp_result", false);

        return SYNTAX_OK;
    }

    //<idwhat> -> = <assign>
    if (data->token.type == TOKEN_ASSIGN) {

        //cant define/assign a variable with the same name as an existing function
        if (symtable_search_function(&data->global_table, temp.attribute->str, &data->current_fun_data) == 0)
        {
            return SEM_UNDEF_ERR;
        }

        //we check the right side
        err = assign(data);
        if (err != 0) {
            return err;
        }
        //EXP RESULT SHOULD BE THE TOP OF THE STACK

        tSymdata *pom;

        //we add the new defined variable to local table if in_function or to global table if in main body
        if (data->in_function == true) {

            if (symtable_search_variable(&data->local_table, temp.attribute->str, &pom) != 0) {

                //store the variable in symtable
                err = symtable_create_variable(&data->local_table, temp.attribute->str);
                if (err != 0)
                    return err;

                //GENERATE
                gen_defvar(temp.attribute->str, data->in_function);
            }
            //GENERATE
            gen_move_exp_res(temp.attribute->str, true);

        }
        else {
            if (symtable_search_variable(&data->global_table, temp.attribute->str, &pom) != 0) {

                //store the variable in symtable
                err = symtable_create_variable(&data->global_table, temp.attribute->str);
                if (err != 0)
                    return err;

                //GENERATE
                gen_defvar(temp.attribute->str, data->in_function);
            }

            //GENERATE
            gen_move_exp_res(temp.attribute->str, false);
        }

        //free the temporary token
        string_free(temp.attribute);
        free(temp.attribute);

        return SYNTAX_OK;
    }

    //<idwhat> -> ( <term> )
    else if (data->token.type == TOKEN_LBRACKET) {

        //if the function was used before, we need to check the param count is the same
        bool used_before = false;

        //store the function name to generate the jump_over-label later on
        char* fun_name = malloc((32 + data->token.attribute->length) * sizeof(char));
        strcpy(fun_name, temp.attribute->str);

        //variable with the same name exists - not a function
        if (symtable_search_variable(&data->global_table, temp.attribute->str, &data->current_fun_data) == 0)
        {
            return SEM_UNDEF_ERR;
        }
        //function with the same name was used
        if (symtable_search_function(&data->global_table, temp.attribute->str, &data->current_fun_data) == 0)
        {
            used_before = true;
        }
        else {
            //1st use of func, we add it to symtable
            err = symtable_create_function(&data->global_table, temp.attribute->str);
            if (err != 0)
                return err;
            //saving pointer to symdata of this function for counting params
            symtable_search_function(&data->global_table, temp.attribute->str, &data->current_fun_data);
        }

        //if the function is "print", we process it individually
        if (strcmp(temp.attribute->str, "print") == 0) {

            err = print(data);
            if (err != 0) {
                return err;
            }
            //GENERATE line end after all print params are processed
            printf("\nWRITE string@\\010");

            CHECK_TOKEN_TYPE(data, TOKEN_RBRACKET)

            GET_TOKEN(data)

            return SYNTAX_OK;
        }

        //we no longer need the temp token
        string_free(temp.attribute);
        free(temp.attribute);

        //saving the old param count
        int paramCountBefore = data->current_fun_data->param_count;

        //set the current param count to 0
        data->current_fun_data->param_count = 0;

        //GENERATE
        printf("\nCREATEFRAME");

        //count and check the param count
        err = term(data);
        if (err != 0) {
            return err;
        }

        //if the function was called/defined before, check if the param count is the same
        if(used_before == true) {
            if (paramCountBefore != data->current_fun_data->param_count) {
                return SEM_PARAM_ERR;
            }
        }

        //GENERATE
        gen_call_fun(fun_name);

        CHECK_TOKEN_TYPE(data, TOKEN_RBRACKET)

        GET_TOKEN(data)

        return SYNTAX_OK;

    }

    //<idwhat> -> Ɛ  == is alone on the line - valid if defined
    else {
        tSymdata *pom;
        //check if var was defined
        if ((symtable_search_variable(&data->local_table, temp.attribute->str, &pom) != 0) &&
            (symtable_search_variable(&data->global_table, temp.attribute->str, &pom) != 0)) {

            string_free(temp.attribute);
            free(temp.attribute);
            return SEM_UNDEF_ERR;
        }

        string_free(temp.attribute);
        free(temp.attribute);

        return SYNTAX_OK;
    }

}

// <term> rule
int term(prog_data* data)
{
    //error number stored
    int err = 0;

    GET_TOKEN(data)

    tSymdata *pom;

    //<term> -> value <term_n>
    //<term> -> id <term_n>
    //<term> -> None <term_n>
    if (IS_VALUE(data->token) || NONE)
    {
        //if term/arg is id
        bool identifier = true;
        //if term/arg is in local frame
        bool local = true;

        //check if token is ID and check where it was defined, if local or global frame, or not defined at all
        if (data->token.type == TOKEN_IDENTIFIER) {
            if (symtable_search_variable(&data->local_table, data->token.attribute->str, &pom) == 0) {
                local = true;
            }
            else if (symtable_search_variable(&data->global_table, data->token.attribute->str, &pom) == 0) {
                local = false;
            }
            else {
                return SEM_UNDEF_ERR;
            }
        }
        else {
            identifier = false;
        }
        //increment param count for unique indexing params in temporary frame
        data->current_fun_data->param_count++;

        //push the param into temporary frame
        gen_tf_defvar(data->current_fun_data->param_count);

        //GENERATE
        err = gen_move_arg(data->current_fun_data->param_count, &(data->token), local, identifier);
        if (err != 0) {
            return err;
        }

        return term_n(data);
    }

    //<term> -> Ɛ
    return SYNTAX_OK;

}

// <term_n> rule
int term_n(prog_data* data)
{
    //error number stored
    int err = 0;

    GET_TOKEN(data)

    //<term_n> -> , <term>
    if(data->token.type == TOKEN_COMMA) {
        return term(data);
    }

    //<term_n> -> Ɛ
    return SYNTAX_OK;

}

// <term> rule - print version
int print(prog_data* data)
{
    //error number stored
    int err = 0;

    GET_TOKEN(data)

    tSymdata *pom;

    //GENERATE
    printf("\nCREATEFRAME");

    //<term> -> value <term_n>
    //<term> -> id <term_n>
    //<term> -> None <term_n>
    if (IS_VALUE(data->token) || NONE)
    {
        //if term is id
        bool identifier = true;
        //if term is in local frame
        bool local = true;

        //check if token is ID adn check where it was defined, if local or global frame, or not defined at all
        if (data->token.type == TOKEN_IDENTIFIER) {
            if (symtable_search_variable(&data->local_table, data->token.attribute->str, &pom) == 0) {
                local = true;
            }
            else if (symtable_search_variable(&data->global_table, data->token.attribute->str, &pom) == 0) {
                local = false;
            }
            else {
                return SEM_UNDEF_ERR;
            }
        }
        else {
            identifier = false;
        }

        //push the param into temporary frame
        gen_tf_defvar(1); //param id is always "1" since print func is always called with only one param
        err = gen_move_arg(1, &(data->token), local, identifier);
        if (err != 0) {
            return err;
        }

        //call the print function for every print param
        gen_call_fun("print");

        return print_n(data);
    }

    //<term> -> Ɛ
    return SYNTAX_OK;

}

// <term_n> rule - print
int print_n(prog_data* data)
{
    //error number stored
    int err = 0;

    GET_TOKEN(data)

    //<term_n> -> , <term>
    if(data->token.type == TOKEN_COMMA) {

        //GENERATE a space between every print param
        printf("\nWRITE string@\\032");
        return print(data);
    }

    //<term_n> -> Ɛ
    return SYNTAX_OK;

}

// <param> rule
int param(prog_data* data)
{
    //error number stored
    int err = 0;

    GET_TOKEN(data)

    tSymdata *pom;

    //<param> -> id
    if(data->token.type == TOKEN_IDENTIFIER) {
        //look in global table if function with the same name exists
       if(symtable_search_function(&data->global_table, data->token.attribute->str, &pom) == 0)
       {
           return SEM_UNDEF_ERR;
       }
       //look for var in local table (if previous param wasn't same name)
       else if (symtable_search_variable(&data->local_table, data->token.attribute->str, &pom) == 0)
       {
           return SEM_UNDEF_ERR;
       }
       //add param to local table
       else
       {
           err = symtable_create_variable(&data->local_table, data->token.attribute->str);
           if (err != 0)
               return err;
           //increment the param count
           data->current_fun_data->param_count++;

           //GENERATE
           gen_def_move_param(data->token.attribute->str, data->current_fun_data->param_count);
       }

        return param_n(data);
    }

    //<param> -> Ɛ
    return SYNTAX_OK;

}

// <param_n> rule
int param_n(prog_data* data)
{
    //error number stored
    int err = 0;

    GET_TOKEN(data)

    //<param_n> -> , <param>
    if(data->token.type == TOKEN_COMMA) {
        return param(data);
    }

    //<param_n> -> Ɛ
    return SYNTAX_OK;

}

//<assign> rule
int assign(prog_data* data) {

    //error number stored
    int err = 0;

    GET_TOKEN(data)

    //after '=' (assign token) we expect string, number, id, or l-bracket - expression
    if (!(IS_VALUE(data->token) || data->token.type == TOKEN_LBRACKET)) {
        return SYNTAX_ERR;
    }

    //<assign> -> <expression>
    if (data->token.type != TOKEN_IDENTIFIER) {

        //EXPRESSION
        //we add the whole expression until eol is found into List
        while(data->token.type != TOKEN_EOL)
        {
            DLInsertLast(&data->expression_list, &data->token, &err);
            if (err != 0) {
                return err;
            }
            GET_TOKEN(data)
        }

        err = expression(data);   //precedential analysis
        if (err != 0) {
            return err;
        }
        DLDisposeList(&data->expression_list);

        return SYNTAX_OK;
    }

    //storing the current token so we can look on the another
    Token temp;
    init_token(&temp, &err);
    if (err != 0) {
        return err;
    }
    //we copy the token type and attribute
    temp.type = data->token.type;
    if(string_copy(data->token.attribute, temp.attribute) != 0) {
        return ERROR_INTERNAL;
    }
    //token has been stored as temp

    GET_TOKEN(data)

    //<assign> -> id( <term> )
    //ID is a function
    if (data->token.type == TOKEN_LBRACKET) {

        //if the function was used before, we need to check the param count is the same
        bool used_before = false;

        //store the function name so the function call can be generated later
        char* fun_name = malloc((32 + data->token.attribute->length) * sizeof(char));
        strcpy(fun_name, temp.attribute->str);

        //variable with the same name exists
        if (symtable_search_variable(&data->global_table, temp.attribute->str, &data->current_fun_data) == 0)
        {
            return SEM_UNDEF_ERR;
        }
        //function with the same name was used
        if (symtable_search_function(&data->global_table, temp.attribute->str, &data->current_fun_data) == 0)
        {
            used_before = true;
        }
        else {
            //1st use of func, we add it to symtable
            err = symtable_create_function(&data->global_table, temp.attribute->str);
            if (err != 0)
                return err;
            //saving pointer to symdata of this function for counting params
            symtable_search_function(&data->global_table, temp.attribute->str, &data->current_fun_data);
        }

        //if the function is "print", we process it individually
        if (strcmp(temp.attribute->str, "print") == 0) {
            err = print(data);
            if (err != 0) {
                return err;
            }
            printf("\nWRITE string@\\010");

            //GENERATE
            /*after exiting <assign>, we want to have the result
             * of the right side on the top of the stack
             */
            printf("\nPUSHS GF@exp_result");

            CHECK_TOKEN_TYPE(data, TOKEN_RBRACKET)

            GET_TOKEN(data)

            return SYNTAX_OK;
        }

        //we no longer need the temp token
        string_free(temp.attribute);
        free(temp.attribute);

        //saving the old param count
        int paramCountBefore = data->current_fun_data->param_count;

        //set the current param count to 0
        data->current_fun_data->param_count = 0;

        //GENERATE
        printf("\nCREATEFRAME");

        //count and check the param count
        err = term(data);
        if (err != 0) {
            return err;
        }

        //if the function was called/defined before, check if the param count is the same
        if(used_before == true) {
            if (paramCountBefore != data->current_fun_data->param_count) {
                return SEM_PARAM_ERR;
            }
        }

        //GENERATE
        gen_call_fun(fun_name);

        //GENERATE
        /*after exiting <assign>, we want to have the result
         * of the right side on the top of the stack
         */
        printf("\nPUSHS GF@exp_result");

        CHECK_TOKEN_TYPE(data, TOKEN_RBRACKET)

        GET_TOKEN(data)

        return SYNTAX_OK;
    }
    //<assign> -> <expression>
    else
    {
        //EXPRESSION
        //we add the whole expression until eol is found into List

        //WE NEED TO STORE THE PREVIOUS TOKEN into the list FIRST
        DLInsertLast(&data->expression_list, &temp, &err);
        if (err != 0) {
            return err;
        }
        string_free(temp.attribute);
        free(temp.attribute);

        //storing the rest of the expression
        while(data->token.type != TOKEN_EOL)
        {
            DLInsertLast(&data->expression_list, &data->token, &err);
            if (err != 0) {
                return err;
            }
            GET_TOKEN(data)
        }

        //analyse and generate the expression
        err = expression(data);   //precedential analysis
        if (err != 0) {
            return err;
        }
        DLDisposeList(&data->expression_list);

        return SYNTAX_OK;
    }
}

//<return_value> rule
int return_value(prog_data* data)
{
    int err = 0;

    //<return_value> -> <expression>
    if (IS_VALUE(data->token) || IS_EXPR(data->token)) {
        //EXPRESSION
        //we add the whole expression until eol is found into List
        while(data->token.type != TOKEN_EOL)
        {
            DLInsertLast(&data->expression_list, &data->token, &err);
            if (err != 0) {
                return err;
            }
            GET_TOKEN(data)
        }

        //analyse and generate the expression
        err = expression(data);   //precedential analysis
        if (err != 0) {
            return err;
        }
        DLDisposeList(&data->expression_list);

        return SYNTAX_OK;
    }

    //<return_value> -> Ɛ

    //GENERATE
    printf("PUSHS nil@nil");
    return SYNTAX_OK;

}

int add_predefined(prog_data* data) {

    char* fce;
    int err = 0;

    //create a function in symtable and initialize its function data

    fce = "inputs";
    err = symtable_create_function(&data->global_table, fce);
    if (err != 0)
        return err;
    symtable_search_function(&data->global_table, fce, &data->current_fun_data);
    data->current_fun_data->param_count = 0;
    data->current_fun_data->defined = true;

    fce = "inputi";
    err = symtable_create_function(&data->global_table, fce);
    if (err != 0)
        return err;
    symtable_search_function(&data->global_table, fce, &data->current_fun_data);
    data->current_fun_data->param_count = 0;
    data->current_fun_data->defined = true;

    fce = "inputf";
    err = symtable_create_function(&data->global_table, fce);
    if (err != 0)
        return err;
    symtable_search_function(&data->global_table, fce, &data->current_fun_data);
    data->current_fun_data->param_count = 0;
    data->current_fun_data->defined = true;

    fce = "print";
    err = symtable_create_function(&data->global_table, fce);
    if (err != 0)
        return err;
    symtable_search_function(&data->global_table, fce, &data->current_fun_data);
    data->current_fun_data->param_count = 0;
    data->current_fun_data->defined = true;

    fce = "len";
    err = symtable_create_function(&data->global_table, fce);
    if (err != 0)
        return err;
    symtable_search_function(&data->global_table, fce, &data->current_fun_data);
    data->current_fun_data->param_count = 1;
    data->current_fun_data->defined = true;

    fce = "substr";
    err = symtable_create_function(&data->global_table, fce);
    if (err != 0)
        return err;
    symtable_search_function(&data->global_table, fce, &data->current_fun_data);
    data->current_fun_data->param_count = 3;
    data->current_fun_data->defined = true;

    fce = "ord";
    err = symtable_create_function(&data->global_table, fce);
    if (err != 0)
        return err;
    symtable_search_function(&data->global_table, fce, &data->current_fun_data);
    data->current_fun_data->param_count = 2;
    data->current_fun_data->defined = true;

    fce = "chr";
    err = symtable_create_function(&data->global_table, fce);
    if (err != 0)
        return err;
    symtable_search_function(&data->global_table, fce, &data->current_fun_data);
    data->current_fun_data->param_count = 1;
    data->current_fun_data->defined = true;

    return err;
}

//starts the analysis
int analyse()
{
    int err_code = 0;

    //INITIALISE EVERYTHING

    // Initialize indentation stack of the scanner
    stack_init(&indent_stack);
    stack_push(&indent_stack, 0);

    //create the structure to store parser data
    prog_data* Data;
    if ((Data = malloc(sizeof(prog_data))) == NULL) {
        return ERROR_INTERNAL;
    }

    //initialize the structure to store tokens
    init_token(&(Data->token), &err_code);
    if (err_code != 0)
        return err_code;

    //structure for expressions
    DLInitList(&Data->expression_list);

    Data->if_count=0;
    Data->while_count=0;

    Data->token_loaded = false;
    Data->in_function = false;
    if ((Data->current_fun_data = malloc(sizeof(tSymdata))) == NULL) {
        return ERROR_INTERNAL;
    }

    symtable_init(&Data->global_table);
    symtable_init(&Data->local_table);

    Data->file = stdin;

    //INITIALISATION COMPLETE

    //ad all predefined functions to symtable
    err_code = add_predefined(Data);
    if (err_code != 0) {
        return err_code;
    }

    //GENERATE
    generate_main_body();

    //STARTING FOR REAL
    //starting the recursive descent
    err_code = program(Data);

    //check if every function used in the program has been defined
    if (symtable_contains_undefined(&Data->global_table) != 0) {
        err_code = SEM_UNDEF_ERR;
    }

    //beauty print
    printf("\n");

    //dispose symtables
    symtable_dispose(&Data->global_table);
    symtable_dispose(&Data->local_table);

    free(Data);

    return err_code;

}

