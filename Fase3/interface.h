#ifndef INTERFACE_H
#define INTERFACE_H

#include "jogo.h"
#include "parser.h"

// -----------------------------------------------------------------------------
// Estrutura: Comando
// Objetivo: Armazena de forma estruturada a ação que o jogador introduziu no
//           terminal, facilitando o processamento no ciclo de jogo (main).
// -----------------------------------------------------------------------------
typedef struct {
    char tipo;
    int orig;
    int dest;
    int qtd;
    char ficheiro[64];
} Comando;

// -----------------------------------------------------------------------------
// Função: mostrarTabuleiro
// Objetivo: Lê o estado atual das pilhas na memória e imprime o tabuleiro de
//           forma visual no terminal, respeitando o layout clássico (Zona
//           Superior e Cascata Inferior).
// -----------------------------------------------------------------------------
void mostrarTabuleiro(const EstadoJogo *estado, const Paciencia *p);

// -----------------------------------------------------------------------------
// Função: lerComando
// Objetivo: Pede ao jogador para introduzir uma ação no terminal, lê a string
//           digitada e extrai os valores para preencher a estrutura Comando
//           de forma segura.
// -----------------------------------------------------------------------------
Comando lerComando(void);

#endif
