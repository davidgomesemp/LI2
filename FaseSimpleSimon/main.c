#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ssimon.h"
#include "displaySS.h"
#include "time.h"
#include "stdlib.h"


// -----------------------------------------------------------------------------
// Função: letraParaIndice
// Objetivo: Converte uma letra entre A e J no índice correspondente da coluna.
// Parâmetros: letra - Letra introduzida pelo utilizador.
// Retorno: Índice da coluna entre 0 e 9, ou -1 se a letra for inválida.
// -----------------------------------------------------------------------------
int letraParaIndice(char letra)
{
    letra = toupper(letra);

    if (letra < 'A' || letra > 'J')
        return -1;

    return letra - 'A';
}

// -----------------------------------------------------------------------------
// Função: mostrarAjuda
// Objetivo: Apresenta ao utilizador o formato dos comandos disponíveis.
// Parâmetros: Nenhum.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void mostrarAjuda()
{
    printf("Comandos:\n");
    printf("  A D 1   -> move 1 carta da coluna A para a D\n");
    printf("  B F 3   -> move 3 cartas da coluna B para a F\n");
    printf("  sair    -> termina o jogo\n\n");
}

// -----------------------------------------------------------------------------
// Função: parseMovimento
// Objetivo: Extrai origem, destino e quantidade de uma linha de comando.
// Parâmetros: linha - String introduzida pelo utilizador;
//             orig - Ponteiro para guardar a coluna origem;
//             dest - Ponteiro para guardar a coluna destino;
//             qtd  - Ponteiro para guardar a quantidade de cartas.
// Retorno: 1 se o comando estiver bem formatado, 0 caso contrário.
// -----------------------------------------------------------------------------
int parseMovimento(char *linha, int *orig, int *dest, int *qtd)
{
    char o;
    char d;

    if (sscanf(linha, " %c %c %d", &o, &d, qtd) != 3)
        return 0;

    *orig = letraParaIndice(o);
    *dest = letraParaIndice(d);

    if (*orig == -1 || *dest == -1 || *qtd <= 0)
        return 0;

    return 1;
}

// -----------------------------------------------------------------------------
// Função: lerComando
// Objetivo: Lê um comando do utilizador e interpreta se é uma jogada, saída ou
//           comando inválido.
// Parâmetros: orig - Ponteiro para guardar a coluna origem;
//             dest - Ponteiro para guardar a coluna destino;
//             qtd  - Ponteiro para guardar a quantidade de cartas.
// Retorno: 1 se for uma jogada válida em formato;
//          0 se o utilizador quiser sair;
//         -1 se o comando for inválido.
// -----------------------------------------------------------------------------
int lerComando(int *orig, int *dest, int *qtd)
{
    char linha[100];

    printf("Comando: ");
    if (fgets(linha, sizeof(linha), stdin) == NULL)
        return 0;

    linha[strcspn(linha, "\n")] = '\0';

    if (strcmp(linha, "sair") == 0)
        return 0;

    if (!parseMovimento(linha, orig, dest, qtd))
        return -1;

    return 1;
}

// -----------------------------------------------------------------------------
// Função: processarJogada
// Objetivo: Verifica se uma jogada é permitida e executa-a se possível.
// Parâmetros: estado - Estado atual do jogo;
//             orig  - Índice da coluna origem;
//             dest  - Índice da coluna destino;
//             qtd   - Quantidade de cartas a mover.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void processarJogada(SimonState *estado, int orig, int dest, int qtd)
{
    if (podeMover(estado, orig, dest, qtd))
    {
        moverCartas(estado, orig, dest, qtd);
        printf("\nMovimento efetuado com sucesso!\n\n");
    }
    else
    {
        printf("\nJogada inválida!\n\n");
    }
}

// -----------------------------------------------------------------------------
// Função: executarComando
// Objetivo: Lê e processa um comando do utilizador.
// Parâmetros: estado - Estado atual do jogo.
// Retorno: 1 se o jogo deve continuar, 0 se o utilizador quiser terminar.
// -----------------------------------------------------------------------------
int executarComando(SimonState *estado)
{
    int orig;
    int dest;
    int qtd;
    int cmd;

    cmd = lerComando(&orig, &dest, &qtd);

    if (cmd == 0)
        return 0;

    if (cmd == -1)
    {
        printf("\nComando inválido! Usa o formato: A D 1\n\n");
    }
    else
    {
        processarJogada(estado, orig, dest, qtd);
    }

    return 1;
}

// -----------------------------------------------------------------------------
// Função: main
// Objetivo: Inicializa o jogo, apresenta o tabuleiro e processa comandos até
//           o utilizador ganhar ou sair.
// Parâmetros: Nenhum.
// Retorno: 0 no fim da execução.
// -----------------------------------------------------------------------------
int main()
{
    SimonState estado;

    srand(time(NULL));

    inicializarSimon(&estado);

    printf("=== SIMPLE SIMON ===\n\n");
    mostrarAjuda();

    while (!verificaVitoriaSimon(&estado))
    {
        mostrarTabuleiro(&estado);
        printf("\n");

        if (!executarComando(&estado))
        {
            printf("\nJogo terminado.\n");
            return 0;
        }
    }

    mostrarTabuleiro(&estado);
    printf("\nPARABÉNS! GANHASTE!\n");

    return 0;
}
