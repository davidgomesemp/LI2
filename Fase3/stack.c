#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Função: initStack
// Objetivo: Inicializa uma pilha nova, colocando o seu topo a -1.
// Parâmetros: s - Apontador para a pilha.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void initStack(Stack *s) {
    s->topo = -1;
}

// -----------------------------------------------------------------------------
// Função: isEmpty
// Objetivo: Verifica se a pilha está vazia.
// Parâmetros: s - Apontador para a pilha.
// Retorno: 1 se vazia, 0 se tiver cartas.
// -----------------------------------------------------------------------------
int isEmpty(Stack *s) {
    return s->topo == -1;
}

// -----------------------------------------------------------------------------
// Função: meter
// Objetivo: Insere (Push) uma carta no topo da pilha.
// Parâmetros: s - Pilha; c - Carta a inserir.
// Retorno: Nenhum.
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
// Parâmetros: s - Pilha de onde retirar.
// Retorno: A carta removida.
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
// Parâmetros: s - Pilha a verificar.
// Retorno: A carta do topo.
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
// Parâmetros: s - Pilha a contabilizar.
// Retorno: O número de cartas (topo + 1).
// -----------------------------------------------------------------------------
int size(Stack *s) {
    return s->topo + 1;
}
