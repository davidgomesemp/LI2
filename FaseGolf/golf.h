#ifndef GOLF_H
#define GOLF_H

#include "stack.h"
#include "card.h"

typedef struct {
      Stack pilha[7];
      Stack deck;
      Stack discard;
} GolfState;

void inicializarJogo (GolfState *estado);
void mostrarTabuleiro(GolfState *estado);
int realizarJogada (GolfState *estado, char opcao);
int verificaVitoria (GolfState *estado);

#endif
