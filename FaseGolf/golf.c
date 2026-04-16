#include <stdio.h>
#include <stdlib.h>
#include "golf.h"
#include "card.h"
#include "stack.h"

//Cria uma representação numérica de um baralho standard (0 a 51) para ser baralhado.
void criarBaralhoTemp(int baralho_temp[]) {
    for (int i = 0; i < 52; i++) {
        baralho_temp[i] = i;
    }
}

//Prepara as estruturas de dados (stacks), garantindo que as 7 colunas, o baralho e o descarte começam vazios.
void inicializarPilhas(GolfState *estado) {
    for (int i = 0; i < 7; i++) {
        initStack(&(estado->pilha[i]));
    }
    initStack(&(estado->deck));
    initStack(&(estado->discard));
}

//Distribui as primeiras 35 cartas do baralho baralhado pelas 7 colunas de jogo (5 cartas por coluna).
void distribuirPilhas(GolfState *estado, int baralho_temp[], int *k) {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 5; j++) {
            meter(&(estado->pilha[i]), getCard(baralho_temp[*k]));
            (*k)++;
        }
    }
}

//Coloca as cartas restantes no monte de compra e vira a primeira carta para iniciar a pilha de descarte.
void criarDeckEDiscard(GolfState *estado, int baralho_temp[], int *k) {
    while (*k < 51) {
        meter(&(estado->deck), getCard(baralho_temp[*k]));
        (*k)++;
    }
    meter(&(estado->discard), getCard(baralho_temp[*k]));
}

//Função principal que orquestra todo o setup: cria, baralha, inicializa as pilhas e distribui as cartas.
void inicializarJogo(GolfState *estado) {
    int baralho_temp[52];
    int k = 0;

    criarBaralhoTemp(baralho_temp);
    shuffleDeck(baralho_temp, 52);
    inicializarPilhas(estado);
    distribuirPilhas(estado, baralho_temp, &k);
    criarDeckEDiscard(estado, baralho_temp, &k);
}
//Move a carta do topo do baralho de reserva para a pilha de descarte.
int puxarDoBaralho(GolfState *estado) {
    if (!isEmpty(&(estado->deck))) {
        meter(&(estado->discard), tirar(&(estado->deck)));
        return 1;
    }
    return 0;
}

//Tenta mover a carta do topo de uma coluna específica para o descarte.
int jogarDaColuna(GolfState *estado, int idx) {
    if (isEmpty(&(estado->pilha[idx]))) return 0;

    Carta cartaCol = verTopo(&(estado->pilha[idx]));
    Carta cartaDesc = verTopo(&(estado->discard));

    if (podeJogar(cartaCol, cartaDesc)) {
        meter(&(estado->discard), tirar(&(estado->pilha[idx])));
        return 1;
    }
    return 0;
}

/* Gere a jogada do utilizador (P para Deck ou A-G para Colunas).*/
int realizarJogada(GolfState *estado, char opcao) {
    if (opcao == 'P') return puxarDoBaralho(estado);
    int idx = (opcao >= 'A' && opcao <= 'G') ? (opcao - 'A') : -1;
    if (idx != -1) return jogarDaColuna(estado, idx);
    return 0;
}

//Verifica se o jogador limpou todas as colunas do tabuleiro.
int verificaVitoria(GolfState *estado) {
    for(int i = 0; i < 7; i++) {
        if (!isEmpty(&(estado->pilha[i]))){
           return 0;
        }
    }
    return 1;
}
