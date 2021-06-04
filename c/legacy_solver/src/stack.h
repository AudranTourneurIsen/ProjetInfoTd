#pragma once
#include <stdio.h>

#define Max 10
typedef char stack_entry;

typedef struct Stack{
    int Top;
    stack_entry entry[Max];
}Stack;

void newStack(Stack *s);
int isFull(Stack s);
int isEmpty(Stack s);
void push(stack_entry e,Stack *s);
void pull(stack_entry *e, Stack *s);
