
#ifndef CARD_H
#define CARD_H

// Estrutura que define as propriedades de uma carta de jogar.
typedef struct {
    int value; // Valor numerico da carta (0 = As, 1 = 2, ..., 12 = Rei).
    char *naipe;  // era char naipe mas agora queremos usar simbolos entao precisamos usar arrays
    char *prnt; // String alocada dinamicamente para impressao em formato UTF-8.
} Carta;

// Inicializa e retorna uma estrutura Carta com base num indice inteiro de 0 a 51.
Carta getCard(int c);
// Baralha um array de inteiros.
void shuffleDeck(int *deck, int size);
// Retorna o valor numerico de uma dada carta.
int getValue(Carta c);
// Retorna o carater correspondente ao naipe de uma dada carta.
char *getSuite(Carta c);


#endif
