#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "display.h"
#include "stack.h"
#include "card.h"

/* Imprime uma carta individual, aplicando cor vermelha se necessário. */
void imprimirCarta(Carta c) {
    /* Verifica se o naipe é Copas ou Ouros para definir a cor no terminal */
    int vermelho = (strcmp(c.naipe, "♥") == 0 || strcmp(c.naipe, "♦") == 0);
    if (vermelho) {
        printf("\033[31m%s\033[0m", c.prnt);
    } else {
        printf("%s", c.prnt);
    }
}

/* Função auxiliar para imprimir a grelha de cartas (5 linhas x 7 colunas). */
void imprimirGrade(GolfState *estado) {
    for (int l = 0; l < 5; l++) {
        for (int k = 0; k < 7; k++) {
            /* Se a linha 'l' existir na pilha 'k', imprime a carta, senão imprime vazio */
            if (l <= estado->pilha[k].topo) {
                imprimirCarta(estado->pilha[k].cartas[l]);
                printf("\t");
            } else {
                printf("--\t");
            }
        }
        printf("\n");
    }
}

/* Função principal de exibição que organiza o cabeçalho e chama a grade. */
void mostrarTabuleiro(GolfState *estado) {
    char letras[7] = {'A','B','C','D','E','F','G'};
    printf("==============================\n GOLF SOLITARIO \n==============================\n\n");
    printf("DISCARD: ");
    imprimirCarta(verTopo(&(estado->discard)));
    printf("\n\n");
    for (int m = 0; m < 7; m++) {
        printf("[%c] \t", letras[m]);
    }
    printf("\n");
    imprimirGrade(estado);
}
