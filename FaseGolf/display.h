#ifndef DISPLAY_H
#define DISPLAY_H

#include "golf.h"
#include "card.h"

// Imprime uma carta com cor ANSI para naipes vermelhos (♥ ♦).
void imprimirCarta(Carta c);

// Imprime o tabuleiro completo: discard, colunas e instruções.
void mostrarTabuleiro(GolfState *estado);

#endif

