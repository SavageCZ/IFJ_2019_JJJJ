/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Kopáček Jiří    xkopac06
 *          Pojsl Jakub	    xpojsl00
 *          Sasín Jonáš	    xsasin05
 *
 *      File: generator.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "generator.h"


void generate_main_body()
{
    printf("\n.IFJcode19");
    printf("\nDEFVAR GF@exp_result");
    printf("\nDEFVAR GF@%%operand_1");
    printf("\nDEFVAR GF@%%operand_2");
    printf("\nDEFVAR GF@%%type_o1");
    printf("\nDEFVAR GF@%%type_o2");
    printf("\nJUMP $$main");
    add_inputs();
    add_inputf();
    add_inputi();
    add_print();
    add_len();
    add_substr();
    add_ord();
    add_chr();        
    gen_type_check_arithmetic();
    gen_type_check_arithmetic_plus();
    gen_type_check_relation();
    gen_zero_div_check();
    gen_int_div();
    gen_float_div();
    gen_stack_top_to_true_false();    
    gen_type_check_eq_neq();
    gen_code_compare_strings();
    printf("\nLABEL $$main");
}

void add_inputs()
{
    printf("\
\n LABEL $inputs\
\n PUSHFRAME\
\n DEFVAR LF@%%retval\
\n MOVE LF@%%retval nil@nil\
\n READ LF@%%retval string\
\n POPFRAME\
\n RETURN\
");

}

void add_inputf()
{
    printf("\
\n LABEL $inputf\
\n PUSHFRAME\
\n DEFVAR LF@%%retval\
\n MOVE LF@%%retval nil@nil\
\n READ LF@%%retval float\
\n POPFRAME\
\n RETURN\
");

}

void add_inputi()
{
    printf("\
\n LABEL $inputi\
\n PUSHFRAME\
\n DEFVAR LF@%%retval\
\n MOVE LF@%%retval nil@nil\
\n READ LF@%%retval int\
\n POPFRAME\
\n RETURN\
");

}

void add_print()
{
    printf("\
\n LABEL $print\
\n PUSHFRAME\
\n DEFVAR LF@%%retval\
\n MOVE LF@%%retval nil@nil\
\n DEFVAR LF@par1type\
\n TYPE LF@par1type LF@%%1\
\n JUMPIFNEQ $print$isnotnone LF@par1type string@nil\
\n MOVE LF@%%1 string@None\
\n LABEL $print$isnotnone\
\n WRITE LF@%%1\
\n POPFRAME\
\n RETURN\
");

}

void add_len()
{
    printf("\
\n LABEL $len\
\n PUSHFRAME\
\n DEFVAR LF@%%retval\
\n MOVE LF@%%retval nil@nil\
\n DEFVAR LF@param1\
\n MOVE LF@param1 LF@%%1\
\n DEFVAR LF@param1$type\
\n TYPE LF@param1$type LF@param1\
\n JUMPIFEQ $len$if$typestring$true LF@param1$type string@string\
\n EXIT int@4\
\n LABEL $len$if$typestring$true\
\n STRLEN LF@%%retval LF@param1\
\n POPFRAME\
\n RETURN\
");

}

void add_substr()
{
    printf("\
\n LABEL $substr\
\n PUSHFRAME\
\n DEFVAR LF@%%retval\
\n MOVE LF@%%retval nil@nil\
\n DEFVAR LF@param1\
\n MOVE LF@param1 LF@%%1\
\n DEFVAR LF@param2\
\n MOVE LF@param2 LF@%%2\
\n DEFVAR LF@param2$type\
\n TYPE LF@param2$type LF@param2\
\n DEFVAR LF@param3\
\n MOVE LF@param3 LF@%%3\
\n DEFVAR LF@param3$type\
\n TYPE LF@param3$type LF@param3\
\n JUMPIFEQ $substr$if$type2int$true LF@param2$type string@int\
\n EXIT int@4\
\n LABEL $substr$if$type2int$true\
\n JUMPIFEQ $substr$if$type3int$true LF@param3$type string@int\
\n EXIT int@4\
\n LABEL $substr$if$type3int$true\
\n DEFVAR LF@length\
\n DEFVAR LF@tmp\
\n CREATEFRAME\
\n DEFVAR TF@%%1\
\n MOVE TF@%%1 LF@%%1\
\n CALL $len\
\n MOVE LF@length TF@%%retval\
\n CLEARS\
\n PUSHS LF@param2\
\n PUSHS int@0\
\n LTS\
\n PUSHS LF@param2\
\n PUSHS LF@length\
\n GTS\
\n PUSHS LF@param3\
\n PUSHS int@0\
\n LTS\
\n ORS\
\n ORS\
\n POPS LF@tmp\
\n CLEARS\
\n JUMPIFEQ $substr$return LF@tmp bool@true\
\n MOVE LF@%%retval string@\
\n DEFVAR LF@maxn\
\n ADD LF@maxn LF@param2 LF@param3\
\n PUSHS LF@maxn\
\n PUSHS LF@length\
\n GTS\
\n POPS LF@tmp\
\n CLEARS\
\n JUMPIFEQ $substr$inrange LF@tmp bool@false\
\n MOVE LF@maxn LF@length\
\n LABEL $substr$inrange\
\n PUSHS LF@param2\
\n PUSHS LF@maxn\
\n LTS\
\n POPS LF@tmp\
\n JUMPIFEQ $substr$return LF@tmp bool@false\
\n GETCHAR LF@tmp LF@param1 LF@param2\
\n CONCAT LF@%%retval LF@%%retval LF@tmp\
\n ADD LF@param2 LF@param2 int@1\
\n JUMP $substr$inrange\
\n LABEL $substr$return\
\n POPFRAME\
\n RETURN\
");
}

void add_ord()
{
    printf("\
\n LABEL $ord\
\n PUSHFRAME\
\n DEFVAR LF@%%retval\
\n MOVE LF@%%retval nil@nil\
\n DEFVAR LF@param1\
\n MOVE LF@param1 LF@%%1\
\n DEFVAR LF@param1$type\
\n TYPE LF@param1$type LF@param1\
\n DEFVAR LF@param2\
\n MOVE LF@param2 LF@%%2\
\n DEFVAR LF@param2$type\
\n TYPE LF@param2$type LF@param2\
\n JUMPIFEQ $ord$if$type1string$true LF@param1$type string@string\
\n EXIT int@4\
\n LABEL $ord$if$type1string$true\
\n JUMPIFEQ $ord$if$type2int$true LF@param2$type string@int\
\n EXIT int@4\
\n LABEL $ord$if$type2int$true\
\n DEFVAR LF@strlen\
\n STRLEN LF@strlen LF@param1\
\n SUB LF@strlen LF@strlen int@1\
\n DEFVAR LF@cmpres1\
\n DEFVAR LF@cmpres2\
\n LT LF@cmpres1 LF@param2 int@0\
\n GT LF@cmpres2 LF@param2 LF@strlen\
\n OR LF@cmpres1 LF@cmpres1 LF@cmpres2\
\n JUMPIFEQ $ord$notininterval LF@cmpres1 bool@true\
\n STRI2INT LF@%%retval LF@param1 LF@param2\
\n LABEL $ord$notininterval\
\n POPFRAME\
\n RETURN\
");
}

void add_chr()
{
        printf("\
\n LABEL $chr\
\n PUSHFRAME\
\n DEFVAR LF@%%retval\
\n MOVE LF@%%retval nil@nil\
\n DEFVAR LF@param1\
\n MOVE LF@param1 LF@%%1\
\n DEFVAR LF@param1$type\
\n TYPE LF@param1$type LF@param1\
\n JUMPIFEQ $chr$if$typeint$true LF@param1$type string@int\
\n EXIT int@4\
\n LABEL $chr$if$typeint$true\
\n DEFVAR LF@cmpres1\
\n DEFVAR LF@cmpres2\
\n LT LF@cmpres1 LF@param1 int@0\
\n GT LF@cmpres2 LF@param1 int@255\
\n OR LF@cmpres1 LF@cmpres1 LF@cmpres2\
\n JUMPIFEQ $chr$ininterval LF@cmpres1 bool@false\
\n EXIT int@4\
\n LABEL $chr$ininterval\
\n INT2CHAR LF@%%retval LF@param1\
\n POPFRAME\
\n RETURN\
");

}


void gen_tf_defvar(int param_id) {
    printf("\nDEFVAR TF@%%%d", param_id);
}

void gen_defvar(char* var_name, bool in_function)
{
    if(in_function) {
        printf("\nDEFVAR LF@%s", var_name);
    }
    else {
        printf("\nDEFVAR GF@%s", var_name);
    }
}

void gen_move_exp_res (char* dest, bool in_function)
{
    printf("\nPOPS GF@exp_result");

    if(in_function) {
        printf("\nMOVE LF@%s GF@exp_result", dest);
    }
    else {
        printf("\nMOVE GF@%s GF@exp_result", dest);
    }
}

int gen_move_arg(int param_id, Token *token, bool local, bool id)
{
    //string to store the adress of param
    char* source;

    //if arg is id, we need to determinate if its local or global
    if (id) {

        source = malloc((32 + token->attribute->length) * sizeof(char));

        if(source == NULL) {
            return 99;
        }

        if (local) {
            sprintf(source, "LF@");
            strcat(source, token->attribute->str);
        }
        else {
            sprintf(source, "GF@");
            strcat(source, token->attribute->str);
        }
    }
    //if arg is not id, we need to convert the constant to ifj format and state its type
    else {

        source = token_to_ifjcode_val(token);
        if(source == NULL) {
            return 99;
        }

    }
    //move arg to temporary frame
    gen_move_arg_print(param_id, source);
    free(source);

    return 0;
}

void gen_move_arg_print(int param_id, char* source)
{
    printf("\nMOVE TF@%%%d %s", param_id, source);
}

void gen_def_move_param (char* param_name, int param_id)
{
    printf("\nDEFVAR LF@%s", param_name);
    printf("\nMOVE LF@%s LF@%%%d", param_name, param_id);
}

void gen_function_start(char* fun_name)
{
    printf("\nJUMP $jump_fun_%s", fun_name);
    printf("\nLABEL $%s", fun_name);
    printf("\nPUSHFRAME");
    printf("\nDEFVAR LF@%%retval");
    printf("\nMOVE LF@%%retval nil@nil");
}

void gen_function_end(bool end_of_fun_body, char* fun_name)
{
    //exiting fun via return - get the return value
    if (!end_of_fun_body) {
        printf("\nPOPS LF@%%retval");
    }

    printf("\nPOPFRAME");
    printf("\nRETURN");

    //exiting fun body without return - label the end of function
    if (end_of_fun_body) {
        printf("\nLABEL $jump_fun_%s", fun_name);
    }
}

void gen_call_fun(char* fun_name) {
    printf("\nCALL $%s", fun_name);
    printf("\nMOVE GF@exp_result TF@%%retval");
}

void gen_if(int cond_id)
{   
    printf("\nCALL $stack$top$to$true$false$");
    printf("\nPOPS GF@exp_result");
    printf("\nJUMPIFEQ $else_label_%d GF@exp_result bool@false", cond_id);
}

void gen_else(int cond_id)
{
    printf("\nJUMP $if_end_%d", cond_id);
    printf("\nLABEL $else_label_%d", cond_id);
}

void gen_if_end(int cond_id)
{
    printf("\nLABEL $if_end_%d", cond_id);
}

void gen_while(int cycle_id)
{   
    printf("\nCALL $stack$top$to$true$false$");
    printf("\nPOPS GF@exp_result");
    printf("\nJUMPIFEQ $while_end_%d GF@exp_result bool@false", cycle_id);
}

void gen_while_end(int cycle_id)
{
    printf("\nJUMP $while_%d", cycle_id);
    printf("\nLABEL $while_end_%d", cycle_id);
}

int gen_push_operand(Token token, int is_global)
{
    if(token.type == TOKEN_IDENTIFIER)
    {
        if(is_global == 1)
        {
            printf("\nPUSHS GF@%s", token.attribute->str);
        }
        else
        {
             printf("\nPUSHS LF@%s", token.attribute->str);
        }
    }
    else
    {
        char * new_string = token_to_ifjcode_val(&token);
        if(new_string == NULL)
        {
            return ERROR_INTERNAL;
        }
        else
        {
            printf("\nPUSHS %s", new_string);
            free(new_string);
        }
                            
    }
    return 0;
}

void gen_operation(symbols symbol)
{
    //switch according to operation
    switch (symbol)
    {
    case S_LS:
        printf("\nCALL $type$control$relation$");
        printf("\nLTS");
        break;

    case S_GT:
        printf("\nCALL $type$control$relation$");
        printf("\nGTS");
        break;

    case S_LSEQ:
        printf("\nCALL $type$control$relation$");
        printf("\nPOPS GF@%%operand_1");
		printf("\nPOPS GF@%%operand_2");
		printf("\nPUSHS GF@%%operand_2");
		printf("\nPUSHS GF@%%operand_1");
		printf("\nLTS");
		printf("\nPUSHS GF@%%operand_2");
		printf("\nPUSHS GF@%%operand_1");
		printf("\nEQS");
		printf("\nORS");
		break;

    case S_GTEQ:
        printf("\nCALL $type$control$relation$");
        printf("\nPOPS GF@%%operand_1");
		printf("\nPOPS GF@%%operand_2");
		printf("\nPUSHS GF@%%operand_2");
		printf("\nPUSHS GF@%%operand_1");
		printf("\nGTS");
		printf("\nPUSHS GF@%%operand_2");
		printf("\nPUSHS GF@%%operand_1");
		printf("\nEQS");
		printf("\nORS");
		break;

    case S_EQ:
        printf("\nCALL $type$control$eq$neq$");
        printf("\nEQS");
        break;

    case S_NEQ:
        printf("\nCALL $type$control$eq$neq$");
        printf("\nEQS");
        printf("\nNOTS");
        break;

    case S_PLUS: 
        printf("\nCALL $type$control$arithmetic$plus$");
        break;

    case S_MINUS:
        printf("\nCALL $type$control$arithmetic$");
        printf("\nSUBS");
        break;

    case S_MUL:
        printf("\nCALL $type$control$arithmetic$");
        printf("\nMULS");
        break;

    case S_DIV:
        printf("\nCALL $type$control$arithmetic$");
        printf("\nCALL $zero$division$check$");
        printf("\nCALL $float$division$fun$");
        break;

    case S_IDIV:
        printf("\nCALL $type$control$arithmetic$");
        printf("\nCALL $zero$division$check$");
        printf("\nCALL $int$division$fun$");
        break;

    default:
        break;
    }

}

char* token_to_ifjcode_val(Token *token)
{
    // Allocates the new string
    char *new_var = malloc((32 + token->attribute->length) * sizeof(char));
    if (new_var == NULL)
    {
        return NULL;
    }
    // Variables for string copying
    int i = 0;
    int pos = 7;
    int length = 32 + token->attribute->length;
    char c;
    // Changes the value based on the token type
    switch(token->type)
    {
        case TOKEN_NUM:
            sprintf(new_var, "int@%d", (int)strtol(token->attribute->str, NULL, 10));
            break;
        case TOKEN_NUM_DEC:
        case TOKEN_NUM_EXP:
            sprintf(new_var, "float@%a", strtod(token->attribute->str, NULL));
            break;
        case TOKEN_KEYWORD:
            if (strcmp(token->attribute->str, "None") == 0)
            {
                sprintf(new_var, "nil@nil");
            } else
            {
                free(new_var);
                return NULL;
            }
            break;
        case TOKEN_STRING:
            sprintf(new_var, "string@");
            // Copies the string
            while (token->attribute->str[i] != 0)
            {
                c = token->attribute->str[i];
                // If the string is getting too long, reallocates
                if (pos > length - 8)
                {
                    length += 64;
                    new_var = realloc(new_var, length * sizeof(char));
                    if (new_var == NULL)
                    {
                        return NULL;
                    }
                }
                // If one of the characters requires an escape sequence
                if (c <= 32 || c == 35 || c == 92)
                {
                    char tmp[6];
                    sprintf(tmp, "\\%03d", c);
                    new_var[pos] = tmp[0];
                    new_var[pos + 1] = tmp[1];
                    new_var[pos + 2] = tmp[2];
                    new_var[pos + 3] = tmp[3];
                    pos += 4;
                } else
                {
                    new_var[pos] = c;
                    pos++;
                }
                i++;
            }
            new_var[pos] = '\0';
            break;
        default:
            free(new_var);
            return NULL;
    }
    return new_var;
}


void gen_zero_div_check()
{    
    printf("\
    \nLABEL $zero$division$check$\
    \nPOPS GF@%%operand_1\
	\nTYPE GF@%%type_o1 GF@%%operand_1\
    \nJUMPIFNEQ $not$an$int$zero$ GF@%%type_o1 string@int\
    \nJUMPIFNEQ $not$a$zero$ GF@%%operand_1 int@0\
    \nEXIT int@9\
    \nLABEL $not$an$int$zero$\
    \nJUMPIFNEQ $not$a$zero$ GF@%%operand_1 float@0x0p+0\
    \nEXIT int@9\
    \nLABEL $not$a$zero$\
    \nPUSHS GF@%%operand_1\
    \nRETURN\
    ");
}


void gen_int_div()
{    
    printf("\
    \nLABEL $int$division$fun$\
    \nPOPS GF@%%operand_1\
	\nPOPS GF@%%operand_2\
	\nTYPE GF@%%type_o1 GF@%%operand_1\
	\nTYPE GF@%%type_o2 GF@%%operand_2\
	\nJUMPIFEQ $operand$1$is$int$4$ GF@%%type_o1 string@int\
    \nEXIT int@4\
    \nLABEL $operand$1$is$int$4$\
	\nJUMPIFNEQ $operand$2$is$not$int$4$ GF@%%type_o2 string@int\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nIDIVS\
    \nRETURN\
    \nLABEL $operand$2$is$not$int$4$\
    \nEXIT int@4\
    ");
}

void gen_float_div()
{
     printf("\
    \nLABEL $float$division$fun$\
    \nPOPS GF@%%operand_1\
	\nPOPS GF@%%operand_2\
	\nTYPE GF@%%type_o1 GF@%%operand_1\
	\nTYPE GF@%%type_o2 GF@%%operand_2\
	\nJUMPIFEQ $operand$1$is$int$5$ GF@%%type_o1 string@int\
	\nJUMPIFEQ $operand$1$is$float$5$ GF@%%type_o1 string@float\
    \nEXIT int@4\
    \nLABEL $operand$1$is$int$5$\
	\nJUMPIFNEQ $operand$2$is$not$int$5$ GF@%%type_o2 string@int\
    \nINT2FLOAT GF@%%operand_1 GF@%%operand_1\
    \nINT2FLOAT GF@%%operand_2 GF@%%operand_2\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nDIVS\
    \nRETURN\
    \nLABEL $operand$2$is$not$int$5$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$5$ GF@%%type_o2 string@float\
    \nINT2FLOAT GF@%%operand_1 GF@%%operand_1\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nDIVS\
    \nRETURN\
    \nLABEL $operand$1$is$float$5$\
	\nJUMPIFNEQ $operand$2$is$not$float$5$ GF@%%type_o2 string@float\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nDIVS\
    \nRETURN\
    \nLABEL $operand$2$is$not$float$5$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$5$ GF@%%type_o2 string@int\
    \nINT2FLOAT GF@%%operand_2 GF@%%operand_2\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nDIVS\
    \nRETURN\
    \nLABEL $uncompatible$unconvertable$types$5$\
    \nEXIT int@4\
    ");
}

void gen_type_check_arithmetic()
{
    printf("\
    \nLABEL $type$control$arithmetic$\
    \nPOPS GF@%%operand_1\
	\nPOPS GF@%%operand_2\
	\nTYPE GF@%%type_o1 GF@%%operand_1\
	\nTYPE GF@%%type_o2 GF@%%operand_2\
	\nJUMPIFEQ $operand$1$is$int$ GF@%%type_o1 string@int\
	\nJUMPIFEQ $operand$1$is$float$ GF@%%type_o1 string@float\
    \nEXIT int@4\
    \nLABEL $operand$1$is$int$\
	\nJUMPIFNEQ $operand$2$is$not$int$ GF@%%type_o2 string@int\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $operand$2$is$not$int$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$ GF@%%type_o2 string@float\
    \nINT2FLOAT GF@%%operand_1 GF@%%operand_1\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $operand$1$is$float$\
	\nJUMPIFNEQ $operand$2$is$not$float$ GF@%%type_o2 string@float\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $operand$2$is$not$float$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$ GF@%%type_o2 string@int\
    \nINT2FLOAT GF@%%operand_2 GF@%%operand_2\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $uncompatible$unconvertable$types$\
    \nEXIT int@4\
    ");
}

void gen_type_check_eq_neq()
{
    printf("\
    \nLABEL $type$control$eq$neq$\
    \nPOPS GF@%%operand_1\
	\nPOPS GF@%%operand_2\
	\nTYPE GF@%%type_o1 GF@%%operand_1\
	\nTYPE GF@%%type_o2 GF@%%operand_2\
	\nJUMPIFEQ $operand$1$is$int$9$ GF@%%type_o1 string@int\
	\nJUMPIFEQ $operand$1$is$float$9$ GF@%%type_o1 string@float\
	\nJUMPIFEQ $operand$1$is$nil$9$ GF@%%type_o1 string@nil\
	\nJUMPIFEQ $operand$1$is$string$9$ GF@%%type_o1 string@string\
	\nJUMPIFEQ $operand$1$is$bool$9$ GF@%%type_o1 string@bool\
    \nEXIT int@4\
    \nLABEL $operand$1$is$int$9$\
	\nJUMPIFNEQ $operand$2$is$not$int$9$ GF@%%type_o2 string@int\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $operand$2$is$not$int$9$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$9$ GF@%%type_o2 string@float\
    \nINT2FLOAT GF@%%operand_1 GF@%%operand_1\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $operand$1$is$float$9$\
	\nJUMPIFNEQ $operand$2$is$not$float$9$ GF@%%type_o2 string@float\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $operand$2$is$not$float$9$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$9$ GF@%%type_o2 string@int\
    \nINT2FLOAT GF@%%operand_2 GF@%%operand_2\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $operand$1$is$string$9$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$9$ GF@%%type_o2 string@string\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nCALL $lexicographicall$string$value$\
    \nRETURN\
    \nLABEL $operand$1$is$bool$9$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$9$ GF@%%type_o2 string@bool\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $operand$1$is$nil$9$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$9$ GF@%%type_o2 string@nil\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $uncompatible$unconvertable$types$9$\
	\nPUSHS int@0\
	\nPUSHS int@1\
    \nRETURN\
    ");
}

void gen_stack_top_to_true_false()
{
    printf("\
    \nLABEL $stack$top$to$true$false$\
    \nPOPS GF@%%operand_1\
	\nTYPE GF@%%type_o1 GF@%%operand_1\
	\nJUMPIFEQ $operand$1$is$int$8$ GF@%%type_o1 string@int\
	\nJUMPIFEQ $operand$1$is$float$8$ GF@%%type_o1 string@float\
	\nJUMPIFEQ $operand$1$is$string$8$ GF@%%type_o1 string@string\
	\nJUMPIFEQ $operand$1$is$false$ GF@%%type_o1 string@nil\
    \nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $operand$1$is$int$8$\
	\nJUMPIFEQ $operand$1$is$false$ GF@%%operand_1 int@0\
	\nPUSHS bool@true\
    \nRETURN\
    \nLABEL $operand$1$is$float$8$\
	\nJUMPIFEQ $operand$1$is$false$ GF@%%operand_1 float@0x0p+0\
	\nPUSHS bool@true\
    \nRETURN\
    \nLABEL $operand$1$is$string$8$\
	\nJUMPIFEQ $operand$1$is$false$ GF@%%operand_1 string@nil\
	\nPUSHS bool@true\
    \nRETURN\
	\nLABEL $operand$1$is$false$\
	\nPUSHS bool@false\
    \nRETURN\
    ");
}

void gen_type_check_relation()
{
    printf("\
    \nLABEL $type$control$relation$\
    \nPOPS GF@%%operand_1\
	\nPOPS GF@%%operand_2\
	\nTYPE GF@%%type_o1 GF@%%operand_1\
	\nTYPE GF@%%type_o2 GF@%%operand_2\
	\nJUMPIFEQ $operand$1$is$int$2$ GF@%%type_o1 string@int\
	\nJUMPIFEQ $operand$1$is$float$2$ GF@%%type_o1 string@float\
	\nJUMPIFEQ $operand$1$is$string$2$ GF@%%type_o1 string@string\
    \nEXIT int@4\
    \nLABEL $operand$1$is$int$2$\
	\nJUMPIFNEQ $operand$2$is$not$int$2$ GF@%%type_o2 string@int\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $operand$2$is$not$int$2$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$2$ GF@%%type_o2 string@float\
    \nINT2FLOAT GF@%%operand_1 GF@%%operand_1\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
	\nLABEL $operand$1$is$float$2$\
	\nJUMPIFNEQ $operand$2$is$not$float$2$ GF@%%type_o2 string@float\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $operand$2$is$not$float$2$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$2$ GF@%%type_o2 string@int\
    \nINT2FLOAT GF@%%operand_2 GF@%%operand_2\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
	\nLABEL $operand$1$is$string$2$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$2$ GF@%%type_o2 string@string\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
    \nCALL $lexicographicall$string$value$\
    \nRETURN\
    \nLABEL $uncompatible$unconvertable$types$2$\
    \nEXIT int@4\
    ");
}

void gen_type_check_arithmetic_plus()
{
    printf("\
    \nLABEL $type$control$arithmetic$plus$\
    \nPOPS GF@%%operand_1\
	\nPOPS GF@%%operand_2\
	\nTYPE GF@%%type_o1 GF@%%operand_1\
	\nTYPE GF@%%type_o2 GF@%%operand_2\
	\nJUMPIFEQ $operand$1$is$int$3$ GF@%%type_o1 string@int\
	\nJUMPIFEQ $operand$1$is$float$3$ GF@%%type_o1 string@float\
	\nJUMPIFEQ $operand$1$is$string$3$ GF@%%type_o1 string@string\
    \nEXIT int@4\
    \nLABEL $operand$1$is$int$3$\
	\nJUMPIFNEQ $operand$2$is$not$int$3$ GF@%%type_o2 string@int\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
	\nADDS\
    \nRETURN\
	\nLABEL $operand$2$is$not$int$3$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$3$ GF@%%type_o2 string@float\
    \nINT2FLOAT GF@%%operand_1 GF@%%operand_1\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
	\nADDS\
    \nRETURN\
	\nLABEL $operand$1$is$float$3$\
	\nJUMPIFNEQ $operand$2$is$not$float$3$ GF@%%type_o2 string@float\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
	\nADDS\
    \nRETURN\
	\nLABEL $operand$2$is$not$float$3$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$3$ GF@%%type_o2 string@int\
    \nINT2FLOAT GF@%%operand_2 GF@%%operand_2\
	\nPUSHS GF@%%operand_2\
	\nPUSHS GF@%%operand_1\
	\nADDS\
    \nRETURN\
	\nLABEL $operand$1$is$string$3$\
	\nJUMPIFNEQ $uncompatible$unconvertable$types$3$ GF@%%type_o2 string@string\
    \nCONCAT GF@%%operand_1 GF@%%operand_2 GF@%%operand_1\
	\nPUSHS GF@%%operand_1\
    \nRETURN\
    \nLABEL $uncompatible$unconvertable$types$3$\
    \nEXIT int@4\
    ");
}

void gen_code_compare_strings()
{
    printf("\
    \nLABEL $lexicographicall$string$value$\
    \nPUSHFRAME\
    \nPOPS GF@%%operand_1\
	\nPOPS GF@%%operand_2\
    \nDEFVAR LF@%%len$str1\
    \nDEFVAR LF@%%len$str2\
    \nDEFVAR LF@%%char$1\
    \nDEFVAR LF@%%char$2\
    \nDEFVAR LF@%%counter\
    \nMOVE LF@%%counter int@-1\
    \nCREATEFRAME\
    \nDEFVAR TF@%%1\
    \nDEFVAR TF@%%2\
    \nMOVE TF@%%1 GF@%%operand_1\
    \nCALL $len\
    \nMOVE LF@%%len$str1 TF@%%retval\
    \nCLEARS\
    \nCREATEFRAME\
    \nDEFVAR TF@%%1\
    \nDEFVAR TF@%%2\
    \nMOVE TF@%%1 GF@%%operand_2\
    \nCALL $len\
    \nMOVE LF@%%len$str2 TF@%%retval\
    \nCLEARS\
    \nLABEL $start$of$cycle$\
    \nADD LF@%%counter LF@%%counter int@1\
    \nJUMPIFEQ $end$of$cycle$ LF@%%len$str1 LF@%%counter\
    \nJUMPIFEQ $end$of$cycle$ LF@%%len$str2 LF@%%counter\
    \nCREATEFRAME\
    \nDEFVAR TF@%%1\
    \nDEFVAR TF@%%2\
    \nMOVE TF@%%1 GF@%%operand_1\
    \nMOVE TF@%%2 LF@%%counter\
    \nCALL $ord\
    \nMOVE LF@%%char$1 TF@%%retval\
    \nCLEARS\
    \nCREATEFRAME\
    \nDEFVAR TF@%%1\
    \nDEFVAR TF@%%2\
    \nMOVE TF@%%1 GF@%%operand_2\
    \nMOVE TF@%%2 LF@%%counter\
    \nCALL $ord\
    \nMOVE LF@%%char$2 TF@%%retval\
    \nCLEARS\
    \nJUMPIFEQ $start$of$cycle$ LF@%%char$1 LF@%%char$2\
    \nPUSHS LF@%%char$1\
    \nPUSHS LF@%%char$2\
    \nGTS\
    \nPOPS GF@exp_result\
    \nJUMPIFNEQ $char$2$is$GT$ GF@exp_result bool@true\
    \nPUSHS int@1\
    \nPUSHS int@0\
    \nPOPFRAME\
    \nRETURN\
    \nLABEL $char$2$is$GT$\
    \nPUSHS int@0\
    \nPUSHS int@1\
    \nPOPFRAME\
    \nRETURN\
    \nLABEL $end$of$cycle$\
    \nJUMPIFNEQ $lens$are$not$same$ LF@%%len$str1 LF@%%len$str2\
    \nPUSHS int@0\
    \nPUSHS int@0\
    \nPOPFRAME\
    \nRETURN\
    \nLABEL $lens$are$not$same$\
    \nPUSHS LF@%%len$str1\
    \nPUSHS LF@%%len$str2\
    \nGTS\
    \nPOPS GF@exp_result\
    \nJUMPIFNEQ $len$2$is$GT$ GF@exp_result bool@true\
    \nPUSHS int@1\
    \nPUSHS int@0\
    \nPOPFRAME\
    \nRETURN\
    \nLABEL $len$2$is$GT$\
    \nPUSHS int@0\
    \nPUSHS int@1\
    \nPOPFRAME\
    \nRETURN\
    ");
}