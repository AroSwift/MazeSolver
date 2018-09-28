#pragma once

struct Stack {
  // Ensure stack is large enough to handle sizable mazes
   void *myStack[1000];
   int top;
};

void initStack(Stack *s);
void push(Stack *s, void *data);
void *pop(Stack *s);
bool isEmpty(Stack *s);