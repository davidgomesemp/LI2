#ifndef STACK_H
#define STACK_H

#include "card.h"

// Estrutura que define as propriedades de uma pilha de cartas (logica LIFO).
typedef struct {
    Carta cartas[52]; // Array estatico que guarda as cartas (maximo de 52).
    int topo;         // Indice da carta que esta no topo da pilha (-1 significa que esta vazia).
} Stack;

// Adiciona uma nova carta ao topo da pilha (Push).
void meter(Stack *s, Carta c);
// Remove e retorna a carta que esta no topo da pilha (Pop).
Carta tirar(Stack *s);
// Retorna a carta do topo sem a remover da pilha (Peek).
Carta verTopo(Stack *s);
// Verifica se a pilha esta vazia (retorna 1 se sim, 0 se nao).
int isEmpty(Stack *s);
// Retorna a quantidade atual de cartas guardadas na pilha.
int size(Stack *s);
// Inicializa a pilha recem-criada, colocando o topo a -1.
void initStack(Stack *s);

#endif
