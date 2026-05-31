#include "menu.h"
#include <stdio.h>
#include <string.h>

#define LARGURA 50

// -----------------------------------------------------------------------------
// Função: imprimirLinha
// Objetivo: Imprime uma linha horizontal de separação com '='.
// -----------------------------------------------------------------------------
static void imprimirLinha(void) {
    printf("==");
    for (int i = 0; i < LARGURA; i++) printf("=");
    printf("==\n");
}

// -----------------------------------------------------------------------------
// Função: imprimirTitulo
// Objetivo: Imprime o título centrado dentro de uma caixa ASCII.
// -----------------------------------------------------------------------------
static void imprimirTitulo(const char *titulo) {
    int len = (int)strlen(titulo);
    int padding = (LARGURA - len) / 2;
    if (padding < 0) padding = 0;
    imprimirLinha();
    printf("||%*s%s%*s||\n", padding, "", titulo, LARGURA - padding - len, "");
    imprimirLinha();
}

// -----------------------------------------------------------------------------
// Função: imprimirOpcao
// Objetivo: Imprime uma linha de opção formatada dentro da caixa ASCII.
// -----------------------------------------------------------------------------
static void imprimirOpcao(int idx, const char *texto) {
    char conteudo[128];
    snprintf(conteudo, sizeof(conteudo), "    [%d] -> %s", idx, texto);
    int len = (int)strlen(conteudo);
    int espacos = LARGURA - len;
    if (espacos < 0) espacos = 0;
    printf("||%s%*s||\n", conteudo, espacos, "");
}

// -----------------------------------------------------------------------------
// Função: lerOpcao
// Objetivo: Lê um inteiro do stdin de forma segura com fgets.
// -----------------------------------------------------------------------------
static int lerOpcao(const char *prompt) {
    int opcao = -1;
    char buf[32];
    printf("%s", prompt);
    fflush(stdout);
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    sscanf(buf, "%d", &opcao);
    imprimirLinha();
    return opcao;
}

// -----------------------------------------------------------------------------
// Função: desenharMenuPrincipal
// Objetivo: Desenha a caixa ASCII do menu principal com as opções.
// -----------------------------------------------------------------------------
static void desenharMenuPrincipal(void) {
    printf("\n");
    imprimirTitulo("PACIENCIA");
    printf("||%*s||\n", LARGURA, "");
    imprimirOpcao(0, "Escolher e Jogar uma Paciencia");
    imprimirOpcao(1, "Sair do Programa");
    printf("||%*s||\n", LARGURA, "");
}

// -----------------------------------------------------------------------------
// Função: interpretarOpcaoPrincipal
// Objetivo: Converte a opção lida do menu principal num código de retorno.
// -----------------------------------------------------------------------------
static int interpretarOpcaoPrincipal(int opcao) {
    if (opcao == 0) return 1;
    if (opcao == 1) return 0;
    return -1;
}

// -----------------------------------------------------------------------------
// Função: menuPrincipal
// Objetivo: Mostra o menu principal e lê a opção do utilizador.
// -----------------------------------------------------------------------------
static int menuPrincipal(void) {
    desenharMenuPrincipal();
    int opcao = lerOpcao("   Opcao Escolhida: ");
    return interpretarOpcaoPrincipal(opcao);
}

// -----------------------------------------------------------------------------
// Função: imprimirListaPaciencias
// Objetivo: Lista os ficheiros de paciência disponíveis na caixa ASCII.
// -----------------------------------------------------------------------------
static void imprimirListaPaciencias(char ficheiros[][MAX_CAMINHO], int total) {
    for (int i = 0; i < total; i++) {
        const char *barra = strrchr(ficheiros[i], '/');
        const char *nome = barra ? barra + 1 : ficheiros[i];
        imprimirOpcao(i, nome);
    }
}

// -----------------------------------------------------------------------------
// Função: desenharMenuSelecao
// Objetivo: Desenha a caixa ASCII do menu de seleção de paciência.
// -----------------------------------------------------------------------------
static void desenharMenuSelecao(char ficheiros[][MAX_CAMINHO], int total) {
    printf("\n");
    imprimirTitulo("SELECIONE A PACIENCIA");
    printf("||%*s||\n", LARGURA, "");
    imprimirListaPaciencias(ficheiros, total);
    printf("||%*s||\n", LARGURA, "");
}

// -----------------------------------------------------------------------------
// Função: menuSelecionarPaciencia
// Objetivo: Mostra o submenu de seleção e lê a opção do utilizador.
// -----------------------------------------------------------------------------
static int menuSelecionarPaciencia(char ficheiros[][MAX_CAMINHO], int total) {
    desenharMenuSelecao(ficheiros, total);
    int opcao = lerOpcao("   Escolha o numero do jogo: ");
    if (opcao < 0 || opcao >= total) return -1;
    return opcao;
}

// -----------------------------------------------------------------------------
// Função: selecionarFicheiro
// Objetivo: Repete o submenu até o utilizador escolher uma opção válida.
// -----------------------------------------------------------------------------
static int selecionarFicheiro(const char *pasta, char caminhoEscolhido[MAX_CAMINHO]) {
    char ficheiros[MAX_PILHAS][MAX_CAMINHO];
    int total = listarPaciencias(pasta, ficheiros, MAX_PILHAS);
    if (total <= 0) return total;
    int opcao;
    do {
        opcao = menuSelecionarPaciencia(ficheiros, total);
    } while (opcao == -1);
    strncpy(caminhoEscolhido, ficheiros[opcao], MAX_CAMINHO - 1);
    caminhoEscolhido[MAX_CAMINHO - 1] = '\0';
    return 1;
}

// -----------------------------------------------------------------------------
// Função: escolherPaciencia
// Objetivo: Orquestra o menu inicial listando ficheiros de uma pasta.
// -----------------------------------------------------------------------------
int escolherPaciencia(const char *pasta, char caminhoEscolhido[MAX_CAMINHO]) {
    int decisao;
    do {
        decisao = menuPrincipal();
    } while (decisao == -1);
    if (decisao == 0) return 0;
    return selecionarFicheiro(pasta, caminhoEscolhido);
}
