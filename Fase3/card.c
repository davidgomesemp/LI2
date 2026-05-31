#include "card.h"
#include <stdio.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Função: getCard
// Objetivo: Cria uma estrutura Carta calculando valor e naipe a partir do ID.
// -----------------------------------------------------------------------------
Carta getCard(int c) {
    char *suits[] = {"♠", "♥", "♦", "♣"};
    char *values = "A23456789TJQK";
    Carta carta;
    int indice = c % 52;
    int suit = indice / 13;
    int value = indice % 13;

    carta.value = value;
    carta.naipe = suits[suit];
    carta.prnt = malloc(8 * sizeof(char));

    if (carta.prnt == NULL) {
        printf("Erro: sem memoria para criar carta\n");
        exit(1);
    }

    sprintf(carta.prnt, "%c%s", values[value], suits[suit]);
    return carta;
}

// -----------------------------------------------------------------------------
// Função: getValue
// Objetivo: Extrai o valor facial (0 a 12) da carta.
// -----------------------------------------------------------------------------
int getValue(Carta c) {
    return c.value;
}

// -----------------------------------------------------------------------------
// Função: getSuite
// Objetivo: Extrai o naipe (0 a 3) da carta.
// -----------------------------------------------------------------------------
char *getSuite(Carta c) {
    return c.naipe;
}

// -----------------------------------------------------------------------------
// Função: shuffleDeck
// Objetivo: Baralha aleatoriamente um vetor de IDs de cartas.
// -----------------------------------------------------------------------------
void shuffleDeck(int *deck, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = deck[i];

        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// -----------------------------------------------------------------------------
// Função: podeJogar
// Objetivo: Confirma se os parâmetros internos da carta são legais.
// -----------------------------------------------------------------------------
int podeJogar(Carta a, Carta b) {
    return abs(a.value - b.value) == 1;
}
