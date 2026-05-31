#ifndef JOGO_H
#define JOGO_H

#include "parser.h"
#include "stack.h"
#include "card.h"   

#define MAX_HISTORICO 50 

// -----------------------------------------------------------------------------
// Estrutura: EstadoJogo
// Objetivo: Representa o tabuleiro atual, guardando o número de pilhas e 
//           o conteúdo de cada uma delas.
// -----------------------------------------------------------------------------
typedef struct {
    Stack pilhas[MAX_PILHAS]; 
    int nPilhas;
} EstadoJogo;

// -----------------------------------------------------------------------------
// Estrutura: Historico
// Objetivo: Guarda um array com os estados de jogo anteriores para 
//           permitir o Undo (voltar atrás).
// -----------------------------------------------------------------------------
typedef struct {
    EstadoJogo estados[MAX_HISTORICO];
    int total;
} Historico;

// -----------------------------------------------------------------------------
// Função: inicializarEstadoJogo
// Objetivo: Prepara um novo jogo, baralha as cartas e distribui-as pelas pilhas.
// -----------------------------------------------------------------------------
void inicializarEstadoJogo(EstadoJogo *e, const Paciencia *p);

// -----------------------------------------------------------------------------
// Função: initHistorico
// Objetivo: Coloca o contador de jogadas guardadas a zero.
// -----------------------------------------------------------------------------
void initHistorico(Historico *h);

// -----------------------------------------------------------------------------
// Função: guardarEstado
// Objetivo: Tira uma fotografia do tabuleiro atual e guarda no histórico.
// -----------------------------------------------------------------------------
void guardarEstado(Historico *h, const EstadoJogo *e);

// -----------------------------------------------------------------------------
// Função: desfazerJogada
// Objetivo: Recupera a última fotografia guardada do tabuleiro.
// Retorno: 1 se conseguiu voltar atrás, 0 se o histórico estiver vazio.
// -----------------------------------------------------------------------------
int desfazerJogada(Historico *h, EstadoJogo *e);

// -----------------------------------------------------------------------------
// Função: salvarJogo
// Objetivo: Cria um ficheiro '.save' com o estado atual do jogo.
// Retorno: 1 se sucesso, 0 se falhar a criar o ficheiro.
// -----------------------------------------------------------------------------
int salvarJogo(const EstadoJogo *e, const char *caminhoPac, const char *ficheiro);

// -----------------------------------------------------------------------------
// Função: carregarJogo
// Objetivo: Abre um ficheiro '.save' e reconstrói as pilhas na memória.
// Retorno: 1 se sucesso, 0 se falhar a abrir ou ler.
// -----------------------------------------------------------------------------
int carregarJogo(EstadoJogo *e, char *nomePacLido, const char *ficheiro);

#endif
