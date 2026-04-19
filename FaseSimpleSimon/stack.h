#ifndef STACK_H
#define STACK_H

#include "card.h"

// Estrutura que define as propriedades de uma pilha de cartas (logica LIFO).
typedef struct {
    Carta cartas[52]; // Array estatico que guarda as cartas (maximo de 52).
    int topo;         // Indice da carta que esta no topo da pilha (-1 significa que esta vazia).
} Stack;

// -----------------------------------------------------------------------------
// Função: meter
// Objetivo: Adiciona uma nova carta ao topo da pilha (Push).
// Parâmetros: s - Apontador para a pilha; c - Carta a ser adicionada.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void meter(Stack *s, Carta c);

// -----------------------------------------------------------------------------
// Função: tirar
// Objetivo: Remove e retorna a carta que esta no topo da pilha (Pop).
// Parâmetros: s - Apontador para a pilha.
// Retorno: A Carta removida do topo da pilha.
// -----------------------------------------------------------------------------
Carta tirar(Stack *s);

// -----------------------------------------------------------------------------
// Função: verTopo
// Objetivo: Retorna a carta do topo sem a remover da pilha (Peek).
// Parâmetros: s - Apontador para a pilha.
// Retorno: A Carta que se encontra no topo da pilha.
// -----------------------------------------------------------------------------
Carta verTopo(Stack *s);

// -----------------------------------------------------------------------------
// Função: isEmpty
// Objetivo: Verifica se a pilha esta vazia.
// Parâmetros: s - Apontador para a pilha.
// Retorno: 1 (verdadeiro) se a pilha estiver vazia, ou 0 (falso) caso contrario.
// -----------------------------------------------------------------------------
int isEmpty(Stack *s);

// -----------------------------------------------------------------------------
// Função: size
// Objetivo: Retorna a quantidade atual de cartas guardadas na pilha.
// Parâmetros: s - Apontador para a pilha.
// Retorno: Inteiro correspondente ao numero de cartas na pilha.
// -----------------------------------------------------------------------------
int size(Stack *s);

// -----------------------------------------------------------------------------
// Função: initStack
// Objetivo: Inicializa a pilha recem-criada, colocando o topo a -1.
// Parâmetros: s - Apontador para a pilha a ser inicializada.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void initStack(Stack *s);

#endif
