#include "interface.h"
#include <stdio.h>

// -----------------------------------------------------------------------------
// Função: mostrarUmaPilha
// Objetivo: Imprime uma pilha inteira no ecrã com índice e o seu tipo.
// Parâmetros: id - Índice da linha; tipo - Tipo da pilha; pilha - Cartas.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void mostrarUmaPilha(const EstadoJogo *estado, const Paciencia *p, int i) {
    Stack *s = (Stack *)&estado->pilhas[i];
    printf("%2d [%-8s]: ", i, p->inits[i].tipo);
    if (isEmpty(s)) {
        printf("(vazia)\n");
        return;
    }
    for (int j = 0; j <= s->topo; j++) {
        printf("%s ", s->cartas[j].prnt);
    }
    printf("\n");
}

// -----------------------------------------------------------------------------
// Função: mostrarTabuleiro
// Objetivo: Itera pelas pilhas do jogo e desenha a mesa de paciência completa.
// Parâmetros: e - Estado atual do jogo; p - Regras (para nomes e tipos).
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void mostrarTabuleiro(const EstadoJogo *estado, const Paciencia *p) {
    printf("\n==== %s ====\n", p->nomeJogo);
    for (int i = 0; i < estado->nPilhas; i++) {
        mostrarUmaPilha(estado, p, i);
    }
    printf("=====================\n");
}

// -----------------------------------------------------------------------------
// Função: lerComando
// Objetivo: Aguarda input do teclado e divide a string num comando interpretável.
// Parâmetros: Nenhum.
// Retorno: Estrutura Comando preenchida.
// -----------------------------------------------------------------------------
Comando lerComando(void) {
    Comando cmd = {'i', 0, 0, 0, ""};
    char linha[128];
    printf("Opcoes: [m]over O D Q | [s]ave arq | [l]oad arq | [u]ndo | [q]uit\n");
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
