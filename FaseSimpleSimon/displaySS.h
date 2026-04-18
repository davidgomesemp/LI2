#ifndef DISPLAYSS_H
#define DISPLAYSS_H

#include "ssimon.h"
#include "card.h"

// -----------------------------------------------------------------------------
// Função: imprimirCarta
// Objetivo: Imprime a carta no terminal, aplicando cor vermelha a Copas e Ouros.
// Parâmetros: c - Carta a ser impressa.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void imprimirCarta(Carta c);

// -----------------------------------------------------------------------------
// Função: imprimirFundacoes
// Objetivo: Imprime marcadores visuais no topo do ecrã para os naipes completos.
// Parâmetros: estado - Apontador para o estado atual do jogo.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void imprimirFundacoes(SimonState *estado);

// -----------------------------------------------------------------------------
// Função: obterAlturaMaxima
// Objetivo: Calcula qual das 10 colunas tem o maior número de cartas no momento.
// Parâmetros: estado - Apontador para o estado atual do jogo.
// Retorno: Inteiro com a altura máxima (número de cartas na maior coluna).
// -----------------------------------------------------------------------------
int obterAlturaMaxima(SimonState *estado);

// -----------------------------------------------------------------------------
// Função: imprimirGrade
// Objetivo: Desenha a matriz de cartas de forma alinhada, percorrendo linha a linha.
// Parâmetros: estado - Apontador para o estado atual do jogo.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void imprimirGrade(SimonState *estado);

// -----------------------------------------------------------------------------
// Função: mostrarTabuleiro
// Objetivo: Apresenta o cabeçalho do jogo, fundações, identificadores e a grelha.
// Parâmetros: estado - Apontador para o estado atual do jogo.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void mostrarTabuleiro(SimonState *estado);

#endif
