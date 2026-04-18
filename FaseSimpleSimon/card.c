#include <stdlib.h>
#include <stdio.h>
#include "card.h"

// -----------------------------------------------------------------------------
// Função: getCard
// Objetivo: Criar uma carta (valor e naipe) a partir de um número de 0 a 51.
// Parâmetros: c - O número inteiro que representa a posição da carta no baralho.
// Retorno: Uma estrutura do tipo 'Carta' preenchida e pronta a usar.
// -----------------------------------------------------------------------------
Carta getCard(int c) {
        // Um array de strings (char*). Cada posição guarda o símbolo de um naipe.
        char *suits[] = {"♠", "♥", "♦", "♣"};
        
        // Uma string normal onde cada letra representa um valor (T é o 10).
        char *values = "A23456789TJQK";

        // A divisão por 13 dá-nos o grupo do naipe (0=Espadas, 1=Copas, 2=Ouros, 3=Paus).
        int suit = c / 13;
        
        // O resto da divisão por 13 (%) dá-nos a posição exata do valor dentro desse naipe.
        int value = c % 13;

        // Criamos a nossa variável 'carta' baseada na struct definida no card.h.
        Carta carta;
        carta.value = value;
        
        // Guardamos o símbolo do naipe (Nota: aqui assumes que no card.h o naipe é um char*).
        carta.naipe = suits[suit];
        
        // malloc: Pede ao computador para reservar 5 espaços na memória (bytes) para a string.
        // Precisamos de 5 porque o símbolo UTF-8 ocupa 3 bytes, o valor 1 byte, e o '\0' 1 byte.
        carta.prnt = (char*)malloc(5*sizeof(char));
        
        // sprintf: Funciona como o printf normal, mas em vez de imprimir no ecrã do terminal,
        // "imprime" (guarda) o texto formatado para dentro da variável carta.prnt.
        sprintf(carta.prnt, "%c%s", values[value], suits[suit]);

        return carta;
}

// -----------------------------------------------------------------------------
// Função: getValue
// Objetivo: Devolver apenas o número correspondente ao valor da carta.
// Parâmetros: c - A estrutura da Carta que queremos analisar.
// Retorno: O valor inteiro da carta (0 a 12).
// -----------------------------------------------------------------------------
int getValue(Carta c) {
        return c.value;
}

// -----------------------------------------------------------------------------
// Função: getSuite
// Objetivo: Devolver a string (texto) correspondente ao naipe da carta.
// Parâmetros: c - A estrutura da Carta que queremos analisar.
// Retorno: Um apontador para o texto do naipe (ex: "♠").
// -----------------------------------------------------------------------------
char *getSuite(Carta c) {
        return c.naipe;
}

// -----------------------------------------------------------------------------
// Função: shuffleDeck
// Objetivo: Baralhar as posições dos números dentro do array do baralho.
// Parâmetros: deck - O array com os números das cartas.
//             size - O tamanho do array (normalmente 52).
// -----------------------------------------------------------------------------
void shuffleDeck(int *deck, int size) {
        // Este ciclo percorre o array posição a posição.
        for (int i = 0; i < size; i++) {
                // rand(): Gera um número aleatório.
                // % (i + 1): Garante que o número gerado está entre 0 e i.
                int j = rand() % (i + 1);

                // As próximas três linhas fazem o "Swap" (troca).
                // Guardamos a carta atual numa variável temporária, pomos a carta
                // aleatória no lugar dela, e pomos a temporária no lugar da aleatória.
                int temp = deck[i];
                deck[i] = deck[j];
                deck[j] = temp;
        }
}

