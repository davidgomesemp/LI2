#include "engine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Função: corNaipe
// Objetivo: Transforma o símbolo do naipe num valor numérico (1=Vermelho, 0=Preto)
// -----------------------------------------------------------------------------
static int corNaipe(const char *naipe) {
    if (strcmp(naipe, "♥") == 0 || strcmp(naipe, "♦") == 0) return 1;
    return 0;
}

// -----------------------------------------------------------------------------
// Função: checkMecanicas
// Objetivo: Avalia regras mecânicas (+, V) para reduzir complexidade.
// -----------------------------------------------------------------------------
static int checkMecanicas(Stack *origem, Stack *dest, int qtd, const char *f) {
    (void)origem;
    if (qtd > 1 && strchr(f, '+') == NULL) return 0;
    if (strchr(f, 'V') != NULL && !isEmpty(dest)) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: checkLimitesFundacao
// Objetivo: Avalia regras de limites da fundação (a, A, k, K).
// -----------------------------------------------------------------------------
static int checkLimitesFundacao(Stack *origem, int qtd, const char *f) {
    int vTopo = origem->cartas[origem->topo].value;
    int vBase = origem->cartas[origem->topo - qtd + 1].value;

    if (strchr(f, 'a') && vTopo != 0) return 0;
    if (strchr(f, 'A') && vBase != 0) return 0;
    if (strchr(f, 'k') && vTopo != 12) return 0;
    if (strchr(f, 'K') && vBase != 12) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: validarFlagsEspeciais
// Objetivo: Agrupa validações mecânicas e de limites.
// -----------------------------------------------------------------------------
static int validarFlagsEspeciais(Stack *origem, Stack *dest, int qtd, const char *f) {
    if (!checkMecanicas(origem, dest, qtd, f)) return 0;
    if (!checkLimitesFundacao(origem, qtd, f)) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: checkOrdemBloco
// Objetivo: Garante que as cartas a mover estão por ordem crescente ou decrescente.
// -----------------------------------------------------------------------------
static int checkOrdemBloco(Stack *origem, int qtd, const char *f) {
    if (strchr(f, '[')) {
        for (int i = 0; i < qtd - 1; i++) {
            if (origem->cartas[origem->topo - i - 1].value != origem->cartas[origem->topo - i].value + 1) return 0;
        }
    }
    if (strchr(f, ']')) {
        for (int i = 0; i < qtd - 1; i++) {
            if (origem->cartas[origem->topo - i - 1].value != origem->cartas[origem->topo - i].value - 1) return 0;
        }
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Função: checkMenorMaior
// Objetivo: Auxiliar de checkValorDestino. Testa apenas as flags '<' e '>'.
// -----------------------------------------------------------------------------
static int checkMenorMaior(int vBase, int vDest, const char *f) {
    if (strchr(f, '<') && vDest != vBase + 1) return 0;
    if (strchr(f, '>') && vDest != vBase - 1) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: checkValorDestino
// Objetivo: Testa as flags relacionais (<, >, ~). Complexidade reduzida para < 10.
// -----------------------------------------------------------------------------
static int checkValorDestino(Stack *origem, Stack *dest, int qtd, const char *f) {
    // Se não tem nenhuma destas flags, passa logo
    if (!strchr(f, '<') && !strchr(f, '>') && !strchr(f, '~')) return 1;

    // Se tem uma flag relacional, o destino não pode estar vazio
    if (isEmpty(dest)) return 0;

    int vBase = origem->cartas[origem->topo - qtd + 1].value;
    int vDest = dest->cartas[dest->topo].value;

    if (!checkMenorMaior(vBase, vDest, f)) return 0;
    if (strchr(f, '~') && abs(vBase - vDest) != 1) return 0;

    return 1;
}

// -----------------------------------------------------------------------------
// Função: validarFlagsValor
// Objetivo: Executa toda a matemática de valores das cartas.
// -----------------------------------------------------------------------------
static int validarFlagsValor(Stack *origem, Stack *dest, int qtd, const char *f) {
    if (!checkOrdemBloco(origem, qtd, f)) return 0;
    if (!checkValorDestino(origem, dest, qtd, f)) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: checkNaipeBloco
// Objetivo: Valida restrições de naipe ('m', 'x') dentro do bloco a mover.
// -----------------------------------------------------------------------------
static int checkNaipeBloco(Stack *origem, int qtd, const char *f) {
    if (strchr(f, 'm')) {
        for (int i = 0; i < qtd - 1; i++) {
            if (strcmp(origem->cartas[origem->topo - i].naipe, origem->cartas[origem->topo - i - 1].naipe) != 0) return 0;
        }
    }
    if (strchr(f, 'x')) {
        for (int i = 0; i < qtd - 1; i++) {
            if (strcmp(origem->cartas[origem->topo - i].naipe, origem->cartas[origem->topo - i - 1].naipe) == 0) return 0;
        }
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Função: checkNaipeDestino
// Objetivo: Valida restrições de naipe ('M', 'X') face ao destino.
// -----------------------------------------------------------------------------
static int checkNaipeDestino(Stack *origem, Stack *dest, int qtd, const char *f) {
    int testaDest = (strchr(f, 'M') || strchr(f, 'X'));
    if (testaDest && isEmpty(dest)) return 0;

    if (testaDest) {
        char *nBase = origem->cartas[origem->topo - qtd + 1].naipe;
        char *nDest = dest->cartas[dest->topo].naipe;
        if (strchr(f, 'M') && strcmp(nBase, nDest) != 0) return 0;
        if (strchr(f, 'X') && strcmp(nBase, nDest) == 0) return 0;
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Função: validarFlagsNaipe
// Objetivo: Orquestra todas as comparações de símbolos de naipes.
// -----------------------------------------------------------------------------
static int validarFlagsNaipe(Stack *origem, Stack *dest, int qtd, const char *f) {
    if (!checkNaipeBloco(origem, qtd, f)) return 0;
    if (!checkNaipeDestino(origem, dest, qtd, f)) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: checkCorBloco
// Objetivo: Valida restrições de cor ('c', 'd') no bloco a mover.
// -----------------------------------------------------------------------------
static int checkCorBloco(Stack *origem, int qtd, const char *f) {
    if (strchr(f, 'c')) {
        for (int i = 0; i < qtd - 1; i++) {
            if (corNaipe(origem->cartas[origem->topo - i].naipe) != corNaipe(origem->cartas[origem->topo - i - 1].naipe)) return 0;
        }
    }
    if (strchr(f, 'd')) {
        for (int i = 0; i < qtd - 1; i++) {
            if (corNaipe(origem->cartas[origem->topo - i].naipe) == corNaipe(origem->cartas[origem->topo - i - 1].naipe)) return 0;
        }
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Função: checkCorDestino
// Objetivo: Valida restrições de cor ('C', 'D') com o topo do destino.
// -----------------------------------------------------------------------------
static int checkCorDestino(Stack *origem, Stack *dest, int qtd, const char *f) {
    int testaDest = (strchr(f, 'C') || strchr(f, 'D'));
    if (testaDest && isEmpty(dest)) return 0;

    if (testaDest) {
        int cBase = corNaipe(origem->cartas[origem->topo - qtd + 1].naipe);
        int cDest = corNaipe(dest->cartas[dest->topo].naipe);
        if (strchr(f, 'C') && cBase != cDest) return 0;
        if (strchr(f, 'D') && cBase == cDest) return 0;
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Função: validarFlagsCor
// Objetivo: Orquestra todas as validações de alternâncias ou semelhanças de cor.
// -----------------------------------------------------------------------------
static int validarFlagsCor(Stack *origem, Stack *dest, int qtd, const char *f) {
    if (!checkCorBloco(origem, qtd, f)) return 0;
    if (!checkCorDestino(origem, dest, qtd, f)) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: validarRegras
// Objetivo: O "Chefe dos Seguranças". Passa a jogada por todos os blocos.
// -----------------------------------------------------------------------------
static int validarRegras(Stack *origem, Stack *dest, int qtd, const char *flags) {
    if (strchr(flags, '*') != NULL) return 1;

    if (!validarFlagsEspeciais(origem, dest, qtd, flags)) return 0;
    if (!validarFlagsValor(origem, dest, qtd, flags)) return 0;
    if (!validarFlagsNaipe(origem, dest, qtd, flags)) return 0;
    if (!validarFlagsCor(origem, dest, qtd, flags)) return 0;

    return 1;
}

// -----------------------------------------------------------------------------
// Função: verificarUmaVitoria
// Objetivo: Lê uma regra WIN e confirma se as pilhas desse tipo a cumprem.
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
// Objetivo: Procura um destino legal para uma carta e "empacota" os dados.
// -----------------------------------------------------------------------------
static int buscarDestinoAuto(EstadoJogo *estado, const Paciencia *p, int r, int o) {
    Stack *origem = &estado->pilhas[o];

    for (int d = 0; d < estado->nPilhas; d++) {
        if (o != d && strcmp(p->inits[d].tipo, p->autos[r].destino) == 0) {
            Stack *dest = &estado->pilhas[d];
            for (int qtd = size(origem); qtd >= 1; qtd--) {
                if (validarRegras(origem, dest, qtd, p->autos[r].flags)) {
                    return (o * 1000000) + (d * 1000) + qtd;
                }
            }
        }
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Função: tentarAutoUmaRegra
// Objetivo: Procura pilhas no tabuleiro que possam iniciar um movimento AUTO.
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
// Objetivo: Transfere as cartas mantendo a ordem original.
// -----------------------------------------------------------------------------
static void moverCartasFisicamente(Stack *origem, Stack *dest, int qtd) {
    Stack temp;
    initStack(&temp);
    for (int i = 0; i < qtd; i++) meter(&temp, tirar(origem));
    for (int i = 0; i < qtd; i++) meter(dest, tirar(&temp));
}

// -----------------------------------------------------------------------------
// Função: checkLimitesMover
// Objetivo: Isola a verificação de segurança de memória para o pmccabe.
// -----------------------------------------------------------------------------
static int checkLimitesMover(EstadoJogo *estado, int idxOrigem, int idxDestino, int qtd) {
    if (idxOrigem < 0 || idxOrigem >= estado->nPilhas) return 0;
    if (idxDestino < 0 || idxDestino >= estado->nPilhas) return 0;
    if (size(&estado->pilhas[idxOrigem]) < qtd || qtd <= 0) return 0;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: validarEMover
// Objetivo: Chamada principal quando o jogador tenta mover uma carta no terminal.
// -----------------------------------------------------------------------------
int validarEMover(EstadoJogo *estado, const Paciencia *p, int idxOrigem, int idxDestino, int qtd) {
    if (!checkLimitesMover(estado, idxOrigem, idxDestino, qtd)) return 0;

    Stack *origem = &estado->pilhas[idxOrigem];
    Stack *dest = &estado->pilhas[idxDestino];

    for (int i = 0; i < p->nMovs; i++) {
        if (strcmp(p->inits[idxOrigem].tipo, p->movs[i].origem) == 0 &&
            strcmp(p->inits[idxDestino].tipo, p->movs[i].destino) == 0) {

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
// Objetivo: Dá uma volta ao tabuleiro para executar a 1ª jogada automática válida.
// -----------------------------------------------------------------------------
static int executarCicloAuto(EstadoJogo *estado, const Paciencia *p) {
    for (int i = 0; i < p->nAutos; i++) {
        int acao = tentarAutoUmaRegra(estado, p, i);
        if (acao != -1) {
            int qtd = acao % 1000;
            int d = (acao / 1000) % 1000;
            int o = acao / 1000000;

            moverCartasFisicamente(&estado->pilhas[o], &estado->pilhas[d], qtd);
            return 1;
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Função: processarMovimentosAutomaticos
// Objetivo: Fica num ciclo de reações em cadeia até não haver mais autos possíveis.
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
// Objetivo: É chamada a cada turno para validar se as regras WIN foram cumpridas.
// -----------------------------------------------------------------------------
int verificarVitoria(const EstadoJogo *estado, const Paciencia *p) {
    if (p->nWins == 0) return 0;

    for (int i = 0; i < p->nWins; i++) {
        if (!verificarUmaVitoria(estado, p, i)) return 0;
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Função: avaliarQuantidadesDica
// Objetivo: Testa as quantidades possíveis a mover para um destino válido.
// -----------------------------------------------------------------------------
static int avaliarQuantidadesDica(Stack *orig, Stack *dst, const char *flags, int max_qtd, int d, int *dest, int *q) {
    for (int qtd = max_qtd; qtd >= 1; qtd--) {
        if (validarRegras(orig, dst, qtd, flags)) {
            *dest = d;
            *q = qtd;
            return 1;
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Função: testarDestinoValido
// Objetivo: Isola a verificação do destino sem usar 'continue'.
// -----------------------------------------------------------------------------
static int testarDestinoValido(const EstadoJogo *est, const Paciencia *p, int r, int o, Stack *orig, int d, int *dest, int *q) {
    // Condição positiva em vez de usar 'continue'
    if (o != d && strcmp(p->inits[d].tipo, p->movs[r].destino) == 0) {
        Stack *dst = (Stack *)&est->pilhas[d];
        int max_qtd = 1;

        if (strcmp(p->inits[o].tipo, "STOCK") != 0) {
            max_qtd = size(orig);
        }

        return avaliarQuantidadesDica(orig, dst, p->movs[r].flags, max_qtd, d, dest, q);
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Função: testarRegraDica
// Objetivo: Testa destinos válidos usando validarRegras SEM alterar o jogo.
// -----------------------------------------------------------------------------
static int testarRegraDica(const EstadoJogo *est, const Paciencia *p, int r, int o, int *dest, int *q) {
    Stack *orig = (Stack *)&est->pilhas[o];

    if (isEmpty(orig) || strcmp(p->inits[o].tipo, p->movs[r].origem) != 0) {
        return 0;
    }

    for (int d = 0; d < est->nPilhas; d++) {
        if (testarDestinoValido(est, p, r, o, orig, d, dest, q)) {
            return 1; // Encontrou dica válida
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Função: procurarNoTabuleiro
// Objetivo: Varre todas as pilhas do tabuleiro (ignorando o STOCK) à procura
//           da primeira jogada legal possível. Dá prioridade a movimentos
//           nas pilhas principais.
// -----------------------------------------------------------------------------
static int procurarNoTabuleiro(const EstadoJogo *est, const Paciencia *p, int *orig, int *dest, int *qtd) {
    for (int r = 0; r < p->nMovs; r++) {
        for (int o = 0; o < est->nPilhas; o++) {
            if (strcmp(p->inits[o].tipo, "STOCK") != 0) {
                if (testarRegraDica(est, p, r, o, dest, qtd)) {
                    *orig = o;
                    return 1;
                }
            }
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Função: procurarNoStock
// Objetivo: Procura uma jogada válida partindo exclusivamente da pilha de
//           STOCK. Serve como último recurso caso o tabuleiro principal
//           esteja bloqueado.
// -----------------------------------------------------------------------------
static int procurarNoStock(const EstadoJogo *est, const Paciencia *p, int *orig, int *dest, int *qtd) {
    for (int r = 0; r < p->nMovs; r++) {
        for (int o = 0; o < est->nPilhas; o++) {
            if (strcmp(p->inits[o].tipo, "STOCK") == 0) {
                if (testarRegraDica(est, p, r, o, dest, qtd)) {
                    *orig = o;
                    return 1;
                }
            }
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Função: procurarDica
// Objetivo: Orquestra a procura de uma jogada legal, testando primeiro o
//           tabuleiro (Fase 1) e, se não encontrar nada, recorrendo ao
//           STOCK (Fase 2).
// -----------------------------------------------------------------------------
int procurarDica(const EstadoJogo *estado, const Paciencia *p, int *o, int *d, int *qtd) {
    if (procurarNoTabuleiro(estado, p, o, d, qtd)) return 1;
    if (procurarNoStock(estado, p, o, d, qtd)) return 1;
    return 0;
}
