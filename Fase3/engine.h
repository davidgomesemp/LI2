#ifndef ENGINE_H
#define ENGINE_H

#include "parser.h"
#include "stack.h"

// A estrutura que guarda o tabuleiro real (pilhas de cartas)
typedef struct {
    Stack pilhas[MAX_PILHAS];
    int nPilhas;
} EstadoJogo;

// 1. Verifica se a jogada manual é válida segundo as regras e, se for, move as cartas.
int validarEMover(EstadoJogo *estado, const Paciencia *p, int idxOrigem, int idxDestino, int qtd);

// 2. Procura e executa movimentos automáticos (comandos AUTO).
int processarMovimentosAutomaticos(EstadoJogo *estado, const Paciencia *p);

// 3. Verifica se as condições de vitória (comandos WIN) foram atingidas.
int verificarVitoria(const EstadoJogo *estado, const Paciencia *p);

#endif
