#include "stack.h"

void newStack(Stack *s) {
    s->Top = 0;
}
int isFull(Stack s) {
    return (s.Top == Max ? 1 : 0);
}
int isEmpty(Stack s) {
    return (s.Top == Max ? 1 : 0);
}
void push(stack_entry e, Stack *s) {
    if (!isFull(*s))
        s->entry[s->Top++] = e;
    else
        printf("Error : Stack Overflow\n");
}
void pull(stack_entry *e, Stack *s) {
    if(!isEmpty(*s))
        *e = s->entry[s->Top--];
    else
        printf("Error : Stack Underflow\n");
}