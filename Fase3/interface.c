#include "interface.h"
#include <stdio.h>
#include <string.h>

#define F_BRANCO "\x1b[47m"
#define T_VERMELHO "\x1b[31m"
#define T_PRETO "\x1b[30m"
#define RESET "\x1b[0m"

// -----------------------------------------------------------------------------
// Função: imprimirCarta
// Objetivo: Imprime uma única carta com as cores ANSI corretas.
// -----------------------------------------------------------------------------
static void imprimirCarta(Carta c) {
    int isRed = (strcmp(c.naipe, "♥") == 0 || strcmp(c.naipe, "♦") == 0);
    const char* cor = isRed ? T_VERMELHO : T_PRETO;
    const char* vals[] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};

    printf("%s%s[ %2s%s ]%s", F_BRANCO, cor, vals[c.value], c.naipe, RESET);
}

// -----------------------------------------------------------------------------
// Função: desenharPilhaHorizontal
// Objetivo: Imprime todas as cartas de uma pilha na mesma linha (Stock/Fund).
// -----------------------------------------------------------------------------
static void desenharPilhaHorizontal(const Stack *s) {
    if (isEmpty((Stack *)s)) {
        printf("[     ]\n");
    } else {
        for (int j = 0; j <= s->topo; j++) {
            imprimirCarta(s->cartas[j]);
            printf(" ");
        }
        printf("\n");
    }
}

// -----------------------------------------------------------------------------
// Função: desenharZonaSuperior
// Objetivo: Imprime as fundações, stock e descarte, alinhados à esquerda.
// Retorno: 1 se imprimiu alguma coisa, 0 se o jogo não tem zona superior.
// -----------------------------------------------------------------------------
static int desenharZonaSuperior(const EstadoJogo *est, const Paciencia *p) {
    int temConteudo = 0;
    for (int i = 0; i < est->nPilhas; i++) {
        if (strcmp(p->inits[i].tipo, "TAB") != 0) {
            temConteudo = 1;
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "%d[%s]", i, p->inits[i].tipo);
            printf("%-10s: ", buffer);
            desenharPilhaHorizontal(&est->pilhas[i]);
        }
    }
    return temConteudo;
}

// -----------------------------------------------------------------------------
// Função: obterMaxAltura
// Objetivo: Descobre qual é a coluna TAB mais alta para saber quantas
//           linhas a cascata precisa de imprimir.
// -----------------------------------------------------------------------------
static int obterMaxAltura(const EstadoJogo *est, const Paciencia *p) {
    int max = 0;
    for (int i = 0; i < est->nPilhas; i++) {
        if (strcmp(p->inits[i].tipo, "TAB") == 0) {
            int alturaAtual = size((Stack *)&est->pilhas[i]);
            if (alturaAtual > max) max = alturaAtual;
        }
    }
    return max;
}

// -----------------------------------------------------------------------------
// Função: desenharCabecalhoTab
// Objetivo: Imprime os nomes das colunas TAB perfeitamente espaçados a 9.
// -----------------------------------------------------------------------------
static void desenharCabecalhoTab(const EstadoJogo *est, const Paciencia *p) {
    for (int i = 0; i < est->nPilhas; i++) {
        if (strcmp(p->inits[i].tipo, "TAB") == 0) {
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "%d[%s]", i, p->inits[i].tipo);
            printf("%-7s  ", buffer);
        }
    }
    printf("\n");
}

// -----------------------------------------------------------------------------
// Função: desenharLinhaCascata
// Objetivo: Imprime apenas um "andar" de cartas ao longo das colunas TAB.
// -----------------------------------------------------------------------------
static void desenharLinhaCascata(const EstadoJogo *est, const Paciencia *p, int nivel) {
    for (int i = 0; i < est->nPilhas; i++) {
        if (strcmp(p->inits[i].tipo, "TAB") == 0) {
            Stack *s = (Stack *)&est->pilhas[i];
            if (nivel < size(s)) {
                imprimirCarta(s->cartas[nivel]);
                printf("  ");
            } else {
                printf("         "); // Espaço invisível de 9 de largura
            }
        }
    }
    printf("\n");
}

// -----------------------------------------------------------------------------
// Função: mostrarTabuleiro
// Objetivo: Função principal da interface. Complexidade reduzida ao orquestrar
//           as funções de impressão modulares.
// -----------------------------------------------------------------------------
void mostrarTabuleiro(const EstadoJogo *estado, const Paciencia *p) {
    printf("\n==== %s ==================================================\n\n", p->nomeJogo);

    if (desenharZonaSuperior(estado, p)) {
        printf("\n-----------------------------------------------------------------\n\n");
    }

    desenharCabecalhoTab(estado, p);

    int max_altura = obterMaxAltura(estado, p);
    for (int nivel = 0; nivel < max_altura; nivel++) {
        desenharLinhaCascata(estado, p, nivel);
    }

    printf("\n=================================================================\n");
}

// -----------------------------------------------------------------------------
// Função: lerComando
// Objetivo: Aguarda o input do jogador e processa a string na estrutura Comando.
// -----------------------------------------------------------------------------
Comando lerComando(void) {
    Comando cmd = {'q', 0, 0, 0, ""};
    char linha[128];
    printf("Opcoes: [m]over O D Q | [d]ica | [s]ave arq | [l]oad arq | [u]ndo | [q]uit\n");
    printf("Comando: ");
    if (!fgets(linha, sizeof(linha), stdin)) return cmd;

    sscanf(linha, " %c", &cmd.tipo);
    if (cmd.tipo == 'm') {
        sscanf(linha, " m %d %d %d", &cmd.orig, &cmd.dest, &cmd.qtd);
    } else if (cmd.tipo == 's' || cmd.tipo == 'l') {
        sscanf(linha, " %*c %s", cmd.ficheiro);
    }
    return cmd;
}
