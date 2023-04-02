/**
 *      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
 *      Team 65, variant 1
 *
 *      Authors:
 *          Diviš Jan       xdivis12
 *
 *      File: stack.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"


int stack_init (tStack* s) {
    // The stack is empty when initialized, so the value of top is -1
    if (s == NULL)
    {
        return 99;
    } else
    {
        s->top = -1;
        return 0;
    }
}

int stack_empty (tStack* s) {
    // When the stack is empty, top should have the value -1
    if (s == NULL)
    {
        return 99;
    }
    return s->top == -1;
}

int stack_full (tStack* s) {
    // When the stack is full, top should be equal to the stack size - 1
    if (s == NULL)
    {
        return 99;
    }
    return s->top == (MAX_STACK_SIZE - 1);
}

int stack_top (tStack* s, int *x) {
    // Returns the int on the s.top index
    if (s == NULL)
    {
        return 99;
    }
    if (stack_empty(s))
    {
        return 1;
    } else
    {
        *x = s->arr[s->top];
        return 0;
    }
}

int stack_pop (tStack* s) {
    // Decrements the value of top making the previous character the new top
    if (s == NULL)
    {
        return 99;
    }
    if (stack_empty(s))
    {
        return 1;
    } else
    {
        s->top--;
        return 0;
    }
}


int stack_push (tStack* s, int x) {
    // Increments the value of top and places the new char on the new top index
    if (s == NULL)
    {
        return 99;
    }
    if (stack_full(s))
    {
        return 1;
    } else
    {
        s->top++;
        s->arr[s->top] = x;
        return 0;
    }
}

//---------------------------------INT STACK END----------------------

void init(stack_top_t* stack)
{
    stack->top = NULL;
}

int push(stack_top_t* stack, symbols symbol)
{
    stack_item_t *tmp = stack->top;

    stack->top = malloc(sizeof(stack_item_t));
    if (stack->top == NULL)
    {
        return 1;
    }
    stack->top->symbol = symbol;
    stack->top->next = tmp;
    return 0;
}

void pop(stack_top_t* stack)
{
    if (stack->top != NULL)
    {
        stack_item_t *tmp = stack->top;
        stack->top = tmp->next;

        free(tmp);
    }
    return;
}

int insert_stop_symbol(stack_top_t* stack)
{
    stack_item_t *tmp = stack->top;
    stack_item_t *prev_symbol = NULL;

    //Cycles through the stack
    while (tmp != NULL)
    {
        //If the correct placement is found
        if (tmp->symbol < S_STOP)
        {
            //inserts a new stop symbol
            stack_item_t *stop_symbol = malloc(sizeof(stack_item_t));
            if (stop_symbol == NULL)
            {
                return 1;
            }

            //If the correct placement is inside the stack
            if (prev_symbol != NULL)
            {
                //corrects pointers on the previous symbol
                stop_symbol->next = prev_symbol->next;
                prev_symbol->next = stop_symbol;
            } else {
                //otherwise corrects pointers to the top
                stop_symbol->next = stack->top;
                stack->top = stop_symbol;
            }
            stop_symbol->symbol = S_STOP;

            return 0;
        }
        else {
            prev_symbol = tmp;
            tmp = tmp->next;
        }
    }
    return 1;
}

stack_item_t* find_terminal(stack_top_t* stack)
{
    stack_item_t *tmp = stack->top;

    //Cycles through the stack
    while (tmp != NULL)
    {
        //If the correct symbol is found, returns it
        if (tmp->symbol < S_STOP)
        {
            break;
        }
        tmp = tmp->next;
    }
    return tmp;
}

void stack_free(stack_top_t* stack)
{
    while (stack->top != NULL)
    {
        pop(stack);
    }
}


