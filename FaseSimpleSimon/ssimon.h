#ifndef SSIMON_H
#define SSIMON_H

#include "card.h"
#include "stack.h"

typedef struct {
    Stack colunas[10];
    int fundacoes;
} SimonState;

void inicializarSimon(SimonState *estado);
void distribuirCartas(SimonState *estado, int deck[]);

int blocoValido(Carta bloco[], int qtd);

int validaDestino(Stack *colunaDest, Carta cartaBase);
int indicesValidos(SimonState *estado, int orig, int dest, int qtd);
int podeMover(SimonState *estado, int orig, int dest, int qtd);

int moverCartas(SimonState *estado, int orig, int dest, int qtd);

void verificarFundacao(SimonState *estado, int col);

int verificaVitoriaSimon(SimonState *estado);

#endif
