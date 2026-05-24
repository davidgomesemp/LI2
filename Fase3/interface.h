#ifndef INTERFACE_H
#define INTERFACE_H

#include "jogo.h"

typedef struct {
    char tipo;
    int orig, dest, qtd;
    char ficheiro[MAX_CAMINHO];
} Comando;

void mostrarTabuleiro(const EstadoJogo *estado, const Paciencia *p);
Comando lerComando(void);

#endif
