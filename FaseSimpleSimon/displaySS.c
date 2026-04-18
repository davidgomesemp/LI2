#include <stdio.h>
#include <string.h>
#include "displaySS.h"
#include "ssimon.h" 
#include "stack.h"
#include "card.h"

// -----------------------------------------------------------------------------
// Função: imprimirCarta
// Objetivo: Imprime a carta no terminal, aplicando cor vermelha a Copas e Ouros.
// Parâmetros: c - Carta a ser impressa.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void imprimirCarta(Carta c) {
    int vermelho = (strcmp(c.naipe, "♥") == 0 || strcmp(c.naipe, "♦") == 0);
    if (vermelho) {
        printf("\033[31m%s\033[0m", c.prnt);
    } else {
        printf("%s", c.prnt);
    }
}

// -----------------------------------------------------------------------------
// Função: imprimirFundacoes
// Objetivo: Imprime marcadores visuais no topo do ecrã para os naipes completos.
// Parâmetros: estado - Estado do jogo.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void imprimirFundacoes(SimonState *estado) {
    printf("NAIPES COMPLETOS: ");
    for (int i = 0; i < 4; i++) {
        if (i < estado->fundacoes) {
            printf("[★]\t"); // Naipe completo
        } else {
            printf("[ ]\t"); // Por completar
        }
    }
    printf("\n\n");
}

// -----------------------------------------------------------------------------
// Função: obterAlturaMaxima
// Objetivo: Calcula qual das 10 colunas tem o maior número de cartas no momento.
// Parâmetros: estado - Estado do jogo.
// Retorno: Inteiro com a altura máxima (número de cartas na maior coluna).
// -----------------------------------------------------------------------------
int obterAlturaMaxima(SimonState *estado) {
    int max = 0;
    for (int i = 0; i < 10; i++) {
        if (size(&(estado->colunas[i])) > max) {
            max = size(&(estado->colunas[i]));
        }
    }
    return max;
}

// -----------------------------------------------------------------------------
// Função: imprimirGrade
// Objetivo: Desenha a matriz de cartas de forma alinhada, percorrendo linha a linha.
// Parâmetros: estado - Estado do jogo.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void imprimirGrade(SimonState *estado) {
    int maxLinhas = obterAlturaMaxima(estado); 

    for (int l = 0; l < maxLinhas; l++) {
        for (int k = 0; k < 10; k++) { 
            if (l <= estado->colunas[k].topo) {
                imprimirCarta(estado->colunas[k].cartas[l]);
                printf("\t");
            } else {
                printf("  \t"); // Espaçamento para manter as colunas alinhadas
            }
        }
        printf("\n"); 
    }
}

// -----------------------------------------------------------------------------
// Função: mostrarTabuleiro
// Objetivo: Apresenta o cabeçalho do jogo, fundações, identificadores e a grelha.
// Parâmetros: estado - Estado do jogo.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void mostrarTabuleiro(SimonState *estado) {
    char letras[10] = {'A','B','C','D','E','F','G','H','I','J'};
    
    printf("========================================================\n");
    printf("                      SIMPLE SIMON                      \n");
    printf("========================================================\n\n");
    
    imprimirFundacoes(estado);
    
    for (int m = 0; m < 10; m++) {
        printf("[%c] \t", letras[m]);
    }
    printf("\n");
    
    imprimirGrade(estado);
}
