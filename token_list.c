/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Pojsl Jakub	    xpojsl00
 *          Sasín Jonáš	    xsasin05
 *
 *      File: token_list.c
 */

#include "token_list.h"

void DLInitList (tDLList *L) {
    //list is clear
    L->First = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDLList *L) {

    //save the pointer to the first element
    tDLElemPtr pom = L->First;

    //delete the first element until there are none
    while (L->First != NULL) {
        //save the pointer to second
        L->First = L->First->rptr;
        //free the first elem
        string_free(pom->token.attribute);
        free(pom->token.attribute);
        free(pom);
        // save the first elem
        pom = L->First;
    }

    //set to the state after initialization
    L->First = NULL;
    L->Last = NULL;
}

void DLInsertLast(tDLList *L, Token *token_in, int *err_code) {

    //create new element
    tDLElemPtr vkladany;
    if ((vkladany = (tDLElemPtr) malloc(sizeof(struct tDLElem))) == NULL) {
        *err_code = 99;
        return;
    }

    //initialize the token
    init_token(&vkladany->token, err_code);
    if (*err_code == 99) {
        return;
    }
    //copy the token type and attribute
    vkladany->token.type = token_in->type;
    if(string_copy(token_in->attribute, vkladany->token.attribute) != 0) {
        *err_code = 99;
        return;
    }

    //if there are no elements in the list, the new elem becomes first and last at the same time
    if (L->First == NULL) {
        L->First = vkladany;
        L->Last = vkladany;
        vkladany->rptr = NULL;
    }
    //else we insert the element at the end of the list
    else {
        L->Last->rptr = vkladany;
        L->Last = vkladany;
        vkladany->rptr = NULL;
    }
}

void DLDeleteFirst (tDLList *L) {
    //we check if the list contains any elements
    if (L->First == NULL)
        return;
    //save the pointer to the second elem
    tDLElemPtr druhy = L->First->rptr;
    //free the first elem
    string_free(L->First->token.attribute);
    free(L->First->token.attribute);
    free(L->First);
    //the second elem becomes the first one
    L->First = druhy;
}

//function to initialize new token
void init_token(Token *token, int *error_code){

        token->type = TOKEN_UNDEFINED;
        token->attribute = (tString*) malloc(sizeof(tString));
        if (token->attribute == NULL)
        {
            *error_code = 99;
            return;
        }
        if (string_init(token->attribute) != 0) {
            *error_code = 99;
            return;
        }
}
