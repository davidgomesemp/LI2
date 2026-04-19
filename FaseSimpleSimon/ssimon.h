#ifndef SSIMON_H
#define SSIMON_H

#include "card.h"
#include "stack.h"

typedef struct {
    Stack colunas[10];
    int fundacoes;
} SimonState;


// -----------------------------------------------------------------------------
// Função: inicializarSimon
// Objetivo: Inicializa o baralho e prepara as colunas.
// Parâmetros: estado - Estado do jogo.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void inicializarSimon(SimonState *estado);

// -----------------------------------------------------------------------------
// Função: distribuirCartas
// Objetivo: Coloca as cartas do baralho nas colunas conforme a distribuição inicial.
// Parâmetros: estado - Estado do jogo; deck - Array com o baralho baralhado.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void distribuirCartas(SimonState *estado, int deck[]);

// -----------------------------------------------------------------------------
// Função: blocoValido
// Objetivo: Verifica se um conjunto de cartas está ordenado e tem o mesmo naipe.
// Parâmetros: bloco - Array de cartas; qtd - Quantidade de cartas no array.
// Retorno: 1 se o bloco for válido, 0 caso contrário.
// -----------------------------------------------------------------------------
int blocoValido(Carta bloco[], int qtd);

// -----------------------------------------------------------------------------
// Função: validaDestino
// Objetivo: Verifica se a base de um bloco de cartas pode ser colocada sobre a carta do topo de uma coluna de destino.
// Parâmetros: colunaDest - Ponteiro para a coluna onde se quer mover as cartas; cartaBase - A primeira carta do bloco que se pretende mover.
// Retorno: 1 se o movimento for permitido, 0 caso contrário.
// -----------------------------------------------------------------------------
int validaDestino(Stack *colunaDest, Carta cartaBase);

// -----------------------------------------------------------------------------
// Função: indicesValidos
// Objetivo: Verifica se os índices das colunas e a quantidade de cartas são permitidos.
// Parâmetros: estado - Estado do jogo; orig - Coluna origem; dest - Coluna destino; qtd - Nº cartas.
// Retorno: 1 se os dados forem válidos, 0 caso contrário.
// -----------------------------------------------------------------------------
int indicesValidos(SimonState *estado, int orig, int dest, int qtd);

// -----------------------------------------------------------------------------
// Função: indicesValidos
// Objetivo: Verifica se os índices das colunas e a quantidade de cartas são permitidos.
// Parâmetros: estado - Estado do jogo; orig - Coluna origem; dest - Coluna destino; qtd - Nº cartas.
// Retorno: 1 se os dados forem válidos, 0 caso contrário.
// -----------------------------------------------------------------------------
int podeMover(SimonState *estado, int orig, int dest, int qtd);

// -----------------------------------------------------------------------------
// Função: moverCartas
// Objetivo: Transfere um bloco de cartas de uma coluna para outra.
// Parâmetros: estado - Estado do jogo; orig - Coluna de origem; dest - Coluna de destino; qtd - Número de cartas.
// Retorno: 1 se o movimento for realizado com sucesso.
// -----------------------------------------------------------------------------
int moverCartas(SimonState *estado, int orig, int dest, int qtd);

// -----------------------------------------------------------------------------
// Função: verificarFundacao
// Objetivo: Verifica se existe uma sequência completa de Rei a Ás do mesmo naipe e remove-a.
// Parâmetros: estado - Estado do jogo; col - A coluna onde a jogada ocorreu.
// Retorno: Nenhum (atualiza o contador de fundações e a coluna se necessário).
// -----------------------------------------------------------------------------
void verificarFundacao(SimonState *estado, int col);

// -----------------------------------------------------------------------------
// Função: verificaVitoriaSimon
// Objetivo: Determina se o jogador completou as 4 sequências necessárias para ganhar.
// Parâmetros: estado - Estado do jogo.
// Retorno: 1 se o jogador ganhou, 0 caso contrário.
// -----------------------------------------------------------------------------
int verificaVitoriaSimon(SimonState *estado);

#endif

