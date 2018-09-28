#include <iostream>
#include "stack.h"
using namespace std;

void initStack(Stack *s) {
  s->top = -1;
}

void push(Stack *s, void *data) {
   s->top++;
   s->myStack[s->top] = data; // Copy data
}

void *pop(Stack *s) {
  void *retVal = s->myStack[s->top];
  s->top--;
  return(retVal);
}

bool isEmpty(Stack *s) {
  return(s->top == -1);
}
