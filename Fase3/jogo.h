#ifndef JOGO_H
#define JOGO_H

#include "engine.h"
#include "parser.h"

#define MAX_HISTORICO 500

typedef struct {
    EstadoJogo estados[MAX_HISTORICO];
    int total;
} Historico;

void inicializarEstadoJogo(EstadoJogo *estado, Paciencia *p);
void initHistorico(Historico *h);
void guardarEstado(Historico *h, const EstadoJogo *estado);
int desfazerJogada(Historico *h, EstadoJogo *estado);
int salvarJogo(const EstadoJogo *estado, const char *ficheiro);
int carregarJogo(EstadoJogo *estado, const char *ficheiro);

#endif
