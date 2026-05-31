#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Função: initStack
// Objetivo: Inicializa uma pilha nova, colocando o seu topo a -1.
// -----------------------------------------------------------------------------
void initStack(Stack *s) {
    s->topo = -1;
}

// -----------------------------------------------------------------------------
// Função: isEmpty
// Objetivo: Verifica se a pilha está vazia.
// -----------------------------------------------------------------------------
int isEmpty(Stack *s) {
    return s->topo == -1;
}

// -----------------------------------------------------------------------------
// Função: meter
// Objetivo: Insere (Push) uma carta no topo da pilha.
// -----------------------------------------------------------------------------
void meter(Stack *s, Carta c) {
    if (s->topo >= MAX_CARTAS_STACK - 1) {
        printf("Erro: stack cheia\n");
        exit(1);
    }

    s->topo++;
    s->cartas[s->topo] = c;
}

// -----------------------------------------------------------------------------
// Função: tirar
// Objetivo: Remove (Pop) e devolve a carta do topo.
// -----------------------------------------------------------------------------
Carta tirar(Stack *s) {
    if (isEmpty(s)) {
        printf("Erro: stack vazia\n");
        exit(1);
    }

    s->topo--;
    return s->cartas[s->topo + 1];
}

// -----------------------------------------------------------------------------
// Função: verTopo
// Objetivo: Lê a carta do topo da pilha sem a remover (Peek).
// -----------------------------------------------------------------------------
Carta verTopo(Stack *s) {
    if (isEmpty(s)) {
        printf("Erro: stack vazia\n");
        exit(1);
    }

    return s->cartas[s->topo];
}
// -----------------------------------------------------------------------------
// Função: size
// Objetivo: Calcula a quantidade de cartas atualmente na pilha.
// -----------------------------------------------------------------------------
int size(Stack *s) {
    return s->topo + 1;
}
