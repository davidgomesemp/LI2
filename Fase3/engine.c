#include "engine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Função: flagMais
// Objetivo: Valida se é permitido mover mais do que uma carta (flag '+').
// Parâmetros: qtd - Cartas a mover; flags - String de regras.
// Retorno: 1 se válido, 0 se inválido.
// -----------------------------------------------------------------------------
static int flagMais(int qtd, const char *flags) {
    if (qtd > 1 && strchr(flags, '+') == NULL) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: flagVazio
// Objetivo: Valida se o destino só pode receber cartas se estiver vazio (flag 'V').
// Parâmetros: dest - Pilha de destino; flags - String de regras.
// Retorno: 1 se válido, 0 se inválido.
// -----------------------------------------------------------------------------
static int flagVazio(Stack *dest, const char *flags) {
    if (strchr(flags, 'V') != NULL && !isEmpty(dest)) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: flagMesmoNaipe
// Objetivo: Valida se todas as cartas a mover são do mesmo naipe (flag 'm').
// Parâmetros: origem - Pilha de origem; qtd - Quantidade; flags - String de regras.
// Retorno: 1 se cumprir a regra, 0 caso contrário.
// -----------------------------------------------------------------------------
static int flagMesmoNaipe(Stack *origem, int qtd, const char *flags) {
    if (strchr(flags, 'm') == NULL) return 1;
    char *naipeBase = origem->cartas[origem->topo].naipe;
    for (int i = 1; i < qtd; i++) {
        if (strcmp(origem->cartas[origem->topo - i].naipe, naipeBase) != 0) return 0;
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Função: flagDecrescente
// Objetivo: Valida se as cartas estão em ordem decrescente (flag '[').
// Parâmetros: origem - Pilha de origem; qtd - Quantidade; flags - String de regras.
// Retorno: 1 se cumprir a regra, 0 caso contrário.
// -----------------------------------------------------------------------------
static int flagDecrescente(Stack *origem, int qtd, const char *flags) {
    if (strchr(flags, '[') == NULL) return 1;
    for (int i = 0; i < qtd - 1; i++) {
        int valBaixo = origem->cartas[origem->topo - i].value;
        int valCima = origem->cartas[origem->topo - i - 1].value;
        if (valCima != valBaixo + 1) return 0;
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Função: flagMenor
// Objetivo: Valida se a carta a mover é menor que a carta de destino (flag '<').
// Parâmetros: origem - Pilha base; qtd - Quantidade; dest - Destino; flags - Regras.
// Retorno: 1 se válido, 0 se inválido.
// -----------------------------------------------------------------------------
static int flagMenor(Stack *origem, int qtd, Stack *dest, const char *flags) {
    if (strchr(flags, '<') == NULL || isEmpty(dest)) return 1;
    int valMover = origem->cartas[origem->topo - qtd + 1].value;
    int valDest = dest->cartas[dest->topo].value;
    if (valMover != valDest - 1) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: flagAdjacente
// Objetivo: Valida se o valor tem diferença exata de 1 para o destino (flag '~').
// Parâmetros: origem - Pilha base; qtd - Quantidade; dest - Destino; flags - Regras.
// Retorno: 1 se for adjacente, 0 se não for.
// -----------------------------------------------------------------------------
static int flagAdjacente(Stack *origem, int qtd, Stack *dest, const char *flags) {
    if (strchr(flags, '~') == NULL || isEmpty(dest)) return 1;
    int valMover = origem->cartas[origem->topo - qtd + 1].value;
    int valDest = dest->cartas[dest->topo].value;
    if (abs(valMover - valDest) != 1) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: validarRegras
// Objetivo: Consolida todas as validações de flags para um determinado movimento.
// Parâmetros: origem - Pilha base; dest - Destino; qtd - Quantidade; flags - Regras.
// Retorno: 1 se respeitar as regras (ou tiver '*'), 0 caso contrário.
// -----------------------------------------------------------------------------
static int validarRegras(Stack *origem, Stack *dest, int qtd, const char *flags) {
    if (strchr(flags, '*') != NULL) return 1;
    if (!flagMais(qtd, flags)) return 0;
    if (!flagVazio(dest, flags)) return 0;
    if (!flagMesmoNaipe(origem, qtd, flags)) return 0;
    if (!flagDecrescente(origem, qtd, flags)) return 0;
    if (!flagMenor(origem, qtd, dest, flags)) return 0;
    if (!flagAdjacente(origem, qtd, dest, flags)) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: verificarUmaVitoria
// Objetivo: Verifica se uma regra de vitória (WIN) específica foi cumprida.
// Parâmetros: estado - Estado do jogo; p - Regras; regraIdx - Índice da regra.
// Retorno: 1 se cumprida, 0 caso contrário.
// -----------------------------------------------------------------------------
static int verificarUmaVitoria(const EstadoJogo *estado, const Paciencia *p, int regraIdx) {
    const char *tipoAlvo = p->wins[regraIdx].tipo;
    int numDesejado = p->wins[regraIdx].ncartas;
    for (int j = 0; j < estado->nPilhas; j++) {
        if (strcmp(p->inits[j].tipo, tipoAlvo) == 0) {
            Stack *pilhaAtual = (Stack *)&estado->pilhas[j];
            if (size(pilhaAtual) != numDesejado) return 0;
        }
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Função: buscarDestinoAuto
// Objetivo: Procura um destino válido para uma regra automática e empacota o resultado.
// Parâmetros: estado - Jogo; p - Regras; r - Índice da regra; o - Índice de origem.
// Retorno: Inteiro com a jogada (Origem/Destino/Qtd), ou -1 se não houver movimento.
// -----------------------------------------------------------------------------
static int buscarDestinoAuto(EstadoJogo *estado, const Paciencia *p, int r, int o) {
    Stack *origem = &estado->pilhas[o];
    for (int d = 0; d < estado->nPilhas; d++) {
        if (o != d && strcmp(p->inits[d].tipo, p->autos[r].destino) == 0) {
            Stack *dest = &estado->pilhas[d];
            for (int qtd = size(origem); qtd >= 1; qtd--) {
                if (validarRegras(origem, dest, qtd, p->autos[r].flags)) {
                    return (o * 10000) + (d * 100) + qtd;
                }
            }
        }
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Função: tentarAutoUmaRegra
// Objetivo: Testa uma regra automática percorrendo todas as pilhas possíveis.
// Parâmetros: estado - Jogo; p - Regras; r - Índice da regra AUTO.
// Retorno: Inteiro com a jogada, ou -1 se não for aplicável.
// -----------------------------------------------------------------------------
static int tentarAutoUmaRegra(EstadoJogo *estado, const Paciencia *p, int r) {
    for (int o = 0; o < estado->nPilhas; o++) {
        if (strcmp(p->inits[o].tipo, p->autos[r].origem) == 0 && !isEmpty(&estado->pilhas[o])) {
            int res = buscarDestinoAuto(estado, p, r, o);
            if (res != -1) return res;
        }
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Função: moverCartasFisicamente
// Objetivo: Tira cartas de uma pilha para outra, mantendo a ordem com pilha auxiliar.
// Parâmetros: origem - Pilha de saída; dest - Pilha de entrada; qtd - Quantidade.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void moverCartasFisicamente(Stack *origem, Stack *dest, int qtd) {
    Stack temp;
    initStack(&temp);
    for (int i = 0; i < qtd; i++) meter(&temp, tirar(origem));
    for (int i = 0; i < qtd; i++) meter(dest, tirar(&temp));
}

// -----------------------------------------------------------------------------
// Função: validarEMover
// Objetivo: Valida a legalidade de um movimento manual e efetua-o.
// Parâmetros: estado - Jogo; p - Regras; idxOrigem - Origem; idxDestino - Destino; qtd - Qtd.
// Retorno: 1 se legal e executado, 0 caso contrário.
// -----------------------------------------------------------------------------
int validarEMover(EstadoJogo *estado, const Paciencia *p, int idxOrigem, int idxDestino, int qtd) {
    Stack *origem = &estado->pilhas[idxOrigem];
    Stack *dest = &estado->pilhas[idxDestino];
    if (size(origem) < qtd || qtd <= 0) return 0;
    for (int i = 0; i < p->nMovs; i++) {
        if (strcmp(p->inits[idxOrigem].tipo, p->movs[i].origem) == 0 && strcmp(p->inits[idxDestino].tipo, p->movs[i].destino) == 0) {
            if (validarRegras(origem, dest, qtd, p->movs[i].flags)) {
                moverCartasFisicamente(origem, dest, qtd);
                return 1;
            }
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Função: executarCicloAuto
// Objetivo: Processa um ciclo de todas as regras automáticas disponíveis.
// Parâmetros: estado - Jogo atual; p - Regras da paciência.
// Retorno: 1 se fez algum movimento, 0 se bloqueou.
// -----------------------------------------------------------------------------
static int executarCicloAuto(EstadoJogo *estado, const Paciencia *p) {
    for (int i = 0; i < p->nAutos; i++) {
        int acao = tentarAutoUmaRegra(estado, p, i);
        if (acao != -1) {
            int qtd = acao % 100;
            int d = (acao / 100) % 100;
            int o = acao / 10000;
            moverCartasFisicamente(&estado->pilhas[o], &estado->pilhas[d], qtd);
            return 1;
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Função: processarMovimentosAutomaticos
// Objetivo: Executa regras AUTO em cadeia até estabilizar o tabuleiro.
// Parâmetros: estado - Estado do jogo; p - Regras da paciência.
// Retorno: 1 se moveu alguma carta, 0 se nada alterou.
// -----------------------------------------------------------------------------
int processarMovimentosAutomaticos(EstadoJogo *estado, const Paciencia *p) {
    int mexeuAlgumaCoisa = 0;
    while (executarCicloAuto(estado, p)) {
        mexeuAlgumaCoisa = 1;
    }
    return mexeuAlgumaCoisa;
}

// -----------------------------------------------------------------------------
// Função: verificarVitoria
// Objetivo: Verifica se o tabuleiro cumpre todos os requisitos de vitória.
// Parâmetros: estado - Estado do jogo; p - Regras da paciência.
// Retorno: 1 se venceu, 0 se continua a jogar.
// -----------------------------------------------------------------------------
int verificarVitoria(const EstadoJogo *estado, const Paciencia *p) {
    if (p->nWins == 0) return 0;
    for (int i = 0; i < p->nWins; i++) {
        if (!verificarUmaVitoria(estado, p, i)) return 0;
    }
    return 1;
}
