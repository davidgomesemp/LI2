#include "jogo.h"
#include <stdio.h>
#include <string.h>
// -----------------------------------------------------------------------------
// Função: distribuirCartasPilha
// Objetivo: Coloca as primeiras cartas do baralho numa pilha durante o setup.
// Parâmetros: pilha - Destino; deck - Baralho; idxBaralho - Índice; qtd - Quantidade.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void distribuirCartasPilha(Stack *pilha, int *deck, int *idxDeck, int qtd) {
    for (int i = 0; i < qtd; i++) {
        meter(pilha, getCard(deck[(*idxDeck)++]));
    }
}

// -----------------------------------------------------------------------------
// Função: inicializarEstadoJogo
// Objetivo: Cria o tabuleiro inicial de acordo com as regras de INIT da paciência.
// Parâmetros: e - Estado a preencher; p - Regras lidas.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void inicializarEstadoJogo(EstadoJogo *estado, Paciencia *p) {
    int totalCartas = p->baralhos * 52;
    int deck[MAX_CARTAS_STACK];
    int idxDeck = 0;
    for (int i = 0; i < totalCartas; i++) deck[i] = i % 52;
    shuffleDeck(deck, totalCartas);
    estado->nPilhas = p->nInits;
    for (int i = 0; i < p->nInits; i++) {
        initStack(&estado->pilhas[i]);
        distribuirCartasPilha(&estado->pilhas[i], deck, &idxDeck, p->inits[i].ncartas);
    }
}

// -----------------------------------------------------------------------------
// Função: initHistorico
// Objetivo: Prepara a estrutura do Histórico para iniciar gravação do Undo.
// Parâmetros: h - Histórico a limpar.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void initHistorico(Historico *h) {
    h->total = 0;
}

// -----------------------------------------------------------------------------
// Função: guardarEstado
// Objetivo: Copia o estado do tabuleiro para o histórico (Snapshot).
// Parâmetros: h - Histórico; e - Estado atual a gravar.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void guardarEstado(Historico *h, const EstadoJogo *estado) {
    if (h->total < MAX_HISTORICO) {
        h->estados[h->total++] = *estado;
    }
}

// -----------------------------------------------------------------------------
// Função: desfazerJogada
// Objetivo: Restaura o tabuleiro para a versão guardada anteriormente no histórico.
// Parâmetros: h - Histórico; e - Estado do jogo a reescrever.
// Retorno: 1 se desfez com sucesso, 0 se não havia jogadas.
// -----------------------------------------------------------------------------
int desfazerJogada(Historico *h, EstadoJogo *estado) {
    if (h->total > 0) {
        *estado = h->estados[--h->total];
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Função: salvarPilha
// Objetivo: Escreve no ficheiro de Save os dados de uma única pilha.
// Parâmetros: f - Ficheiro aberto; pilha - Pilha a gravar.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void salvarPilha(FILE *f, const Stack *s) {
    fprintf(f, "%d\n", s->topo);
    for (int j = 0; j <= s->topo; j++) {
        fprintf(f, "%d %s\n", s->cartas[j].value, s->cartas[j].naipe);
    }
}

// -----------------------------------------------------------------------------
// Função: salvarJogo
// Objetivo: Grava todas as pilhas e o cabeçalho num ficheiro (Save).
// Parâmetros: e - Estado atual; ficheiro - Caminho do save.
// Retorno: 1 sucesso, 0 erro.
// -----------------------------------------------------------------------------
int salvarJogo(const EstadoJogo *estado, const char *ficheiro) {
    FILE *f = fopen(ficheiro, "w");
    if (!f) return 0;
    fprintf(f, "%d\n", estado->nPilhas);
    for (int i = 0; i < estado->nPilhas; i++) salvarPilha(f, &estado->pilhas[i]);
    fclose(f);
    return 1;
}

// -----------------------------------------------------------------------------
// Função: suitStrToIndex
// Objetivo: Converte o símbolo string de um naipe lido no load para índice.
// Parâmetros: naipe - String com o símbolo.
// Retorno: Índice do naipe (0 a 3).
// -----------------------------------------------------------------------------
static int suitStrToIndex(const char *naipe) {
    if (strcmp(naipe, "♥") == 0) return 1;
    if (strcmp(naipe, "♦") == 0) return 2;
    if (strcmp(naipe, "♣") == 0) return 3;
    return 0;
}

// -----------------------------------------------------------------------------
// Função: carregarPilha
// Objetivo: Lê cartas de um ficheiro e reconstrói as memórias de uma pilha.
// Parâmetros: f - Ficheiro de leitura; pilha - Pilha destino.
// Retorno: 1 sucesso, 0 erro no ficheiro.
// -----------------------------------------------------------------------------
static void carregarPilha(FILE *f, Stack *s) {
    int topo, val;
    char naipe[10];
    fscanf(f, "%d", &topo);
    initStack(s);
    for (int j = 0; j <= topo; j++) {
        fscanf(f, "%d %s", &val, naipe);
        int idx = suitStrToIndex(naipe) * 13 + val;
        meter(s, getCard(idx));
    }
}

// -----------------------------------------------------------------------------
// Função: carregarJogo
// Objetivo: Reconstrói o tabuleiro completo a partir de um ficheiro de Load.
// Parâmetros: e - Estado a preencher; ficheiro - Ficheiro de Save.
// Retorno: 1 sucesso, 0 não encontrado.
// -----------------------------------------------------------------------------
int carregarJogo(EstadoJogo *estado, const char *ficheiro) {
    FILE *f = fopen(ficheiro, "r");
    if (!f) return 0;
    fscanf(f, "%d", &estado->nPilhas);
    for (int i = 0; i < estado->nPilhas; i++) carregarPilha(f, &estado->pilhas[i]);
    fclose(f);
    return 1;
}
