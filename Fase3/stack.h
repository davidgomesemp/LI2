#ifndef STACK_H
#define STACK_H

#include "card.h"

#define MAX_CARTAS_STACK 208

typedef struct {
    Carta cartas[MAX_CARTAS_STACK];
    int topo;
} Stack;

void meter(Stack *s, Carta c);
Carta tirar(Stack *s);
Carta verTopo(Stack *s);
int isEmpty(Stack *s);
int size(Stack *s);
void initStack(Stack *s);

#endif
