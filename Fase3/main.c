#include "interface.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Função: tratarMover
// Objetivo: Tenta executar um movimento. Se válido, guarda estado e processa autos.
// Parâmetros: cmd - Comando com origem/destino; e - Estado do jogo; p - Regras; h - Histórico.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void tratarMover(Comando cmd, EstadoJogo *e, Paciencia *p, Historico *h) {
    EstadoJogo copia = *e;
    if (validarEMover(e, p, cmd.orig, cmd.dest, cmd.qtd)) {
        guardarEstado(h, &copia);
        processarMovimentosAutomaticos(e, p);
    } else {
        printf("\n>>> ERRO: Movimento invalido! <<<\n");
    }
}

// -----------------------------------------------------------------------------
// Função: tratarUndo
// Objetivo: Executa o comando de recuar uma jogada.
// Parâmetros: h - Histórico com estados guardados; e - Estado atual a reescrever.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void tratarUndo(Historico *h, EstadoJogo *e) {
    if (!desfazerJogada(h, e)) {
        printf("\n>>> ERRO: Nada para desfazer! <<<\n");
    }
}

// -----------------------------------------------------------------------------
// Função: tratarSaveLoad
// Objetivo: Trata de guardar ou carregar o estado do jogo em ficheiro.
// Parâmetros: cmd - Comando lido; e - Estado do jogo; h - Histórico (para limpar no Load).
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void tratarSaveLoad(Comando cmd, EstadoJogo *e, Historico *h) {
    if (cmd.tipo == 's' && salvarJogo(e, cmd.ficheiro)) {
        printf("\n>>> Jogo guardado! <<<\n");
    } else if (cmd.tipo == 'l' && carregarJogo(e, cmd.ficheiro)) {
        initHistorico(h);
    }
}

// -----------------------------------------------------------------------------
// Função: executarComando
// Objetivo: Encaminha o comando lido para a função de tratamento adequada.
// Parâmetros: cmd - Comando; e - Estado do jogo; p - Regras da paciência; h - Histórico.
// Retorno: 0 se for para sair do jogo, 1 caso contrário.
// -----------------------------------------------------------------------------
static int executarComando(Comando cmd, EstadoJogo *e, Paciencia *p, Historico *h) {
    if (cmd.tipo == 'q') return 0;
    if (cmd.tipo == 'u') tratarUndo(h, e);
    else if (cmd.tipo == 's' || cmd.tipo == 'l') tratarSaveLoad(cmd, e, h);
    else if (cmd.tipo == 'm') tratarMover(cmd, e, p, h);
    else printf("\n>>> ERRO: Comando desconhecido! <<<\n");
    return 1;
}

// -----------------------------------------------------------------------------
// Função: loopJogo
// Objetivo: Mantém a interface ativa, mostra tabuleiro, verifica vitória e pede comandos.
// Parâmetros: e - Estado atual do jogo; p - Regras; h - Histórico.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void loopJogo(EstadoJogo *e, Paciencia *p, Historico *h) {
    int aJogar = 1;
    while (aJogar) {
        mostrarTabuleiro(e, p);
        if (verificarVitoria(e, p)) {
            printf("\n*** PARABENS! GANHASTE O JOGO! ***\n\n");
            break;
        }
        Comando cmd = lerComando();
        aJogar = executarComando(cmd, e, p, h);
    }
}

// -----------------------------------------------------------------------------
// Função: carregarConfiguracao
// Objetivo: Trata do menu inicial e carrega as regras da paciência escolhida.
// Parâmetros: p - Estrutura Paciencia a preencher.
// Retorno: 1 em sucesso, 0 se quiser sair, -1 em erro.
// -----------------------------------------------------------------------------
static int carregarConfiguracao(Paciencia *p) {
    char caminho[MAX_CAMINHO];
    if (escolherPaciencia("paciencias", caminho) <= 0) return 0;
    if (!lerPaciencia(caminho, p)) return -1;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: jogar
// Objetivo: Inicializa a mesa com cartas baralhadas e arranca o ciclo principal.
// Parâmetros: p - Regras da paciência; h - Histórico na memória.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void jogar(Paciencia *p, Historico *h) {
    EstadoJogo e;
    inicializarEstadoJogo(&e, p);
    initHistorico(h);
    processarMovimentosAutomaticos(&e, p);
    loopJogo(&e, p, h);
}

// -----------------------------------------------------------------------------
// Função: main
// Objetivo: Ponto de entrada. Aloca memória, arranca o jogo e liberta recursos no fim.
// Parâmetros: Nenhum.
// Retorno: 0 se terminar com sucesso, 1 em caso de erro.
// -----------------------------------------------------------------------------
int main() {
    Paciencia p;
    Historico *h = malloc(sizeof(Historico));
    
    if (h == NULL) {
        printf("Erro fatal: Falta de memoria.\n");
        return 1;
    }
    
    int status = carregarConfiguracao(&p);
    
    if (status > 0) {
        jogar(&p, h);
    }
    
    free(h);
    
    return (status < 0) ? 1 : 0; 
}
