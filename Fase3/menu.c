#include "menu.h"
#include <stdio.h>
#include <string.h>

// -----------------------------------------------------------------------------
// Função: mostrarOpcoes
// Objetivo: Mostra a lista de jogos encontrados no terminal.
// Parâmetros: ficheiros - Array de nomes; total - Quantidade de ficheiros.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void mostrarOpcoes(char ficheiros[][MAX_CAMINHO], int total) {
    printf("\nPaciencias disponiveis:\n");
    for (int i = 0; i < total; i++) {
        printf("%d - %s\n", i + 1, ficheiros[i]);
    }
    printf("0 - Voltar\n");
}

// -----------------------------------------------------------------------------
// Função: lerOpcao
// Objetivo: Lê a escolha do utilizador e limpa o buffer do teclado.
// Parâmetros: total - Número máximo de opções.
// Retorno: Opção escolhida ou -1 para inválida.
// -----------------------------------------------------------------------------
static int lerOpcao(int total) {
    int opcao;
    printf("Escolha: ");
    if (scanf("%d", &opcao) != 1) return -1;
    
    // A MAGIA AQUI: Limpa o 'Enter' fantasma do teclado!
    while(getchar() != '\n'); 
    
    if (opcao < 0 || opcao > total) return -1;
    return opcao;
}

// -----------------------------------------------------------------------------
// Função: escolherPaciencia
// Objetivo: Orquestra o menu inicial listando ficheiros de uma pasta.
// Parâmetros: pasta - Diretório; caminhoEscolhido - String preenchida com o caminho.
// Retorno: 1 se escolha válida, 0 para sair, -1 erro.
// -----------------------------------------------------------------------------
int escolherPaciencia(const char *pasta, char caminhoEscolhido[MAX_CAMINHO]) {
    char ficheiros[MAX_PILHAS][MAX_CAMINHO];
    int total = listarPaciencias(pasta, ficheiros, MAX_PILHAS);
    int opcao;
    if (total <= 0) return total;
    mostrarOpcoes(ficheiros, total);
    opcao = lerOpcao(total);
    if (opcao <= 0) return 0;
    strncpy(caminhoEscolhido, ficheiros[opcao - 1], MAX_CAMINHO - 1);
    caminhoEscolhido[MAX_CAMINHO - 1] = '\0';
    return 1;
}
