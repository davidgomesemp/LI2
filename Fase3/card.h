#ifndef CARD_H
#define CARD_H

typedef struct {
    int value;
    char *naipe;
    char *prnt;
} Carta;

Carta getCard(int c);
void shuffleDeck(int *deck, int size);
int getValue(Carta c);
char *getSuite(Carta c);
int podeJogar(Carta a, Carta b);

#endif
