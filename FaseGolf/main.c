#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "card.h"
#include "stack.h"
#include "golf.h"
#include "display.h"

/*
* Gere o loop de interação com o utilizador durante o jogo.
* return 0 se o utilizador desistir ('Q'), 1 se o utilizador vencer.
*/
int cicloDeJogo(GolfState *estado) {
    char opcao;
    while (1) {
        mostrarTabuleiro(estado);
        printf("\nComandos: A-G = colunas | P = deck | Q = sair\n");
        scanf(" %c", &opcao);
        opcao = toupper(opcao);
        if (opcao == 'Q') return 0;

        realizarJogada(estado, opcao);

        if (verificaVitoria(estado)) {
            printf("PARABÉNS, GANHASTE!\n");
            return 1;
        }
    }
}

//Ponto de entrada do programa. Configura o jogo e inicia o loop principal.
int main() {
    GolfState estado;
    srand(time(NULL));
    inicializarJogo(&estado);
    cicloDeJogo(&estado);
    return 0;
}
