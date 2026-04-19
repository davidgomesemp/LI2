#include "ssimon.h"
#include "card.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------------
// Função: distribuirCartas
// Objetivo: Coloca as cartas do baralho nas colunas conforme a distribuição inicial.
// Parâmetros: estado - Estado do jogo; deck - Array com o baralho baralhado.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void distribuirCartas(SimonState *estado, int deck[]) {
    int dist[10] = {8, 8, 8, 7, 6, 5, 4, 3, 2, 1};
    int k = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < dist[i]; j++) {
            meter(&(estado->colunas[i]), getCard(deck[k++]));
        }
    }
}

// -----------------------------------------------------------------------------
// Função: inicializarSimon
// Objetivo: Inicializa o baralho e prepara as colunas.
// Parâmetros: estado - Estado do jogo.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void inicializarSimon(SimonState *estado) {
    int deck[52];
    for (int i = 0; i < 52; i++) deck[i] = i;
    shuffleDeck(deck, 52);
    estado->fundacoes = 0;
    for (int i = 0; i < 10; i++) initStack(&(estado->colunas[i]));
    distribuirCartas(estado, deck);
}

// -----------------------------------------------------------------------------
// Função: blocoValido
// Objetivo: Verifica se um conjunto de cartas está ordenado e tem o mesmo naipe.
// Parâmetros: bloco - Array de cartas; qtd - Quantidade de cartas no array.
// Retorno: 1 se o bloco for válido, 0 caso contrário.
// -----------------------------------------------------------------------------
int blocoValido(Carta bloco[], int qtd) {
    for (int i = 1; i < qtd; i++) {
    if (getValue(bloco[i]) != getValue(bloco[i-1]) + 1 ||
            strcmp(getSuite(bloco[i]), getSuite(bloco[i-1])) != 0) {
            return 0;
        }
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Função: validaDestino
// Objetivo: Verifica se a base de um bloco de cartas pode ser colocada sobre a carta do topo de uma coluna de destino.
// Parâmetros: colunaDest - Ponteiro para a coluna onde se quer mover as cartas; cartaBase - A primeira carta do bloco que se pretende mover.
// Retorno: 1 se o movimento for permitido, 0 caso contrário.
// -----------------------------------------------------------------------------
int validaDestino(Stack *colunaDest, Carta cartaBase) {
    if (isEmpty(colunaDest)) return 1;
    Carta topoDest = verTopo(colunaDest);
    return (getValue(topoDest) == getValue(cartaBase) + 1);
}

// -----------------------------------------------------------------------------
// Função: indicesValidos
// Objetivo: Verifica se os índices das colunas e a quantidade de cartas são permitidos.
// Parâmetros: estado - Estado do jogo; orig - Coluna origem; dest - Coluna destino; qtd - Nº cartas.
// Retorno: 1 se os dados forem válidos, 0 caso contrário.
// -----------------------------------------------------------------------------
int indicesValidos(SimonState *estado, int orig, int dest, int qtd) {
    if (orig < 0 || orig > 9 || dest < 0 || dest > 9 || orig == dest) return 0;
    if (size(&(estado->colunas[orig])) < qtd || qtd <= 0) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: podeMover
// Objetivo: Valida se um movimento de cartas entre colunas respeita as regras do jogo.
// Parâmetros: estado - Estado do jogo; orig - Coluna de origem; dest - Coluna de destino; qtd - Número de cartas.
// Retorno: 1 se o movimento for permitido, 0 caso contrário.
// -----------------------------------------------------------------------------
int podeMover(SimonState *estado, int orig, int dest, int qtd) {
    if (!indicesValidos(estado, orig, dest, qtd)) return 0;
    Carta bloco[52];
    for (int i = 0; i < qtd; i++) bloco[i] = tirar(&(estado->colunas[orig]));
    int ok = blocoValido(bloco, qtd);
    if (ok) ok = validaDestino(&(estado->colunas[dest]), bloco[qtd-1]);
    for (int i = qtd - 1; i >= 0; i--) meter(&(estado->colunas[orig]), bloco[i]);
    return ok;
}

// -----------------------------------------------------------------------------
// Função: moverCartas
// Objetivo: Transfere um bloco de cartas de uma coluna para outra.
// Parâmetros: estado - Estado do jogo; orig - Coluna de origem; dest - Coluna de destino; qtd - Número de cartas.
// Retorno: 1 se o movimento for realizado com sucesso.
// -----------------------------------------------------------------------------
int moverCartas(SimonState *estado, int orig, int dest, int qtd) {
    Stack temp;
    initStack(&temp);
    for (int i = 0; i < qtd; i++) meter(&temp, tirar(&(estado->colunas[orig])));
    for (int i = 0; i < qtd; i++) meter(&(estado->colunas[dest]), tirar(&temp));
    verificarFundacao(estado, dest);
    return 1;
}

// -----------------------------------------------------------------------------
// Função: verificarFundacao
// Objetivo: Verifica se existe uma sequência completa de Rei a Ás do mesmo naipe e remove-a.
// Parâmetros: estado - Estado do jogo; col - A coluna onde a jogada ocorreu.
// Retorno: Nenhum (atualiza o contador de fundações e a coluna se necessário).
// -----------------------------------------------------------------------------
void verificarFundacao(SimonState *estado, int col) {
    if (size(&(estado->colunas[col])) < 13) return;
    Carta bloco[13];
    for (int i = 0; i < 13; i++) bloco[i] = tirar(&(estado->colunas[col]));
    if (blocoValido(bloco, 13) && getValue(bloco[0]) == 1) {
        estado->fundacoes++;
    } else {
        for (int i = 12; i >= 0; i--) meter(&(estado->colunas[col]), bloco[i]);
    }
}

// -----------------------------------------------------------------------------
// Função: verificaVitoriaSimon
// Objetivo: Determina se o jogador completou as 4 sequências necessárias para ganhar.
// Parâmetros: estado - Estado do jogo.
// Retorno: 1 se o jogador ganhou, 0 caso contrário.
// -----------------------------------------------------------------------------
int verificaVitoriaSimon(SimonState *estado) {
    return (estado->fundacoes == 4);
}

