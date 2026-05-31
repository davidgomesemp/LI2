#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "parser.h"
#include "card.h"
#include "jogo.h"
#include "engine.h"
#include "interface.h"
#include "menu.h"

// -----------------------------------------------------------------------------
// Função: tratarMover
// Objetivo: Tenta executar um movimento. Se válido, guarda estado e processa autos.
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
// -----------------------------------------------------------------------------
static void tratarUndo(Historico *h, EstadoJogo *e) {
    if (!desfazerJogada(h, e)) {
        printf("\n>>> ERRO: Nada para desfazer! <<<\n");
    }
}
 
// -----------------------------------------------------------------------------
// Função: tratarSaveLoad
// Objetivo: Trata de guardar ou carregar o estado do jogo em ficheiro.
// -----------------------------------------------------------------------------
static void tratarSaveLoad(Comando cmd, EstadoJogo *e, Historico *h, const char *caminhoPac) {
    char nomePacLido[MAX_CAMINHO];
    if (cmd.ficheiro[0] == '\0') {
        printf("\n>>> ERRO: Nome de ficheiro em falta! <<<\n");
        return;
    }
    if (cmd.tipo == 's' && salvarJogo(e, caminhoPac, cmd.ficheiro)) {
        printf("\n>>> Jogo guardado! <<<\n");
    } else if (cmd.tipo == 'l' && carregarJogo(e, nomePacLido, cmd.ficheiro)) {
        initHistorico(h);
    }
}

// -----------------------------------------------------------------------------
// Função: tratarDica
// Objetivo: Procura uma jogada válida no tabuleiro e exibe-a como dica ao
//           jogador. Caso não existam movimentos, exibe uma mensagem de aviso.
// -----------------------------------------------------------------------------
static void tratarDica(const EstadoJogo *e, const Paciencia *p) {
    int orig, dest, qtd;
    if (procurarDica(e, p, &orig, &dest, &qtd)) {
        printf("\n>>> DICA: Tenta mover %d carta(s) da pilha %d para a pilha %d! <<<\n", qtd, orig, dest);
    } else {
        printf("\n>>> DICA: Nao encontrei nenhuma jogada possivel. <<<\n");
    }
}
 
// -----------------------------------------------------------------------------
// Função: executarComando
// Objetivo: Encaminha o comando lido para a função de tratamento adequada.
// -----------------------------------------------------------------------------
static int executarComando(Comando cmd, EstadoJogo *e, Paciencia *p, Historico *h, const char *caminhoPac) {
    if (cmd.tipo == 'q') return 0;
    if (cmd.tipo == 'u') tratarUndo(h, e);
    else if (cmd.tipo == 's' || cmd.tipo == 'l') tratarSaveLoad(cmd, e, h, caminhoPac);
    else if (cmd.tipo == 'm') tratarMover(cmd, e, p, h);
    else if (cmd.tipo == 'd') tratarDica(e, p);
    else printf("\n>>> ERRO: Comando desconhecido! <<<\n");
    return 1;
}
 
// -----------------------------------------------------------------------------
// Função: loopJogo
// Objetivo: Mantém a interface ativa, mostra tabuleiro, verifica vitória e pede comandos.
// -----------------------------------------------------------------------------
static void loopJogo(EstadoJogo *e, Paciencia *p, Historico *h, const char *caminhoPac) {
    int aJogar = 1;
    while (aJogar) {
        mostrarTabuleiro(e, p);
        if (verificarVitoria(e, p)) {
            printf("\n*** PARABENS! GANHASTE O JOGO! ***\n\n");
            aJogar = 0; 
        } else {
            Comando cmd = lerComando();
            aJogar = executarComando(cmd, e, p, h, caminhoPac);
        }
    }
}
 
// -----------------------------------------------------------------------------
// Função: carregarConfiguracao
// Objetivo: Trata do menu inicial e carrega as regras da paciência escolhida.
// -----------------------------------------------------------------------------
static int carregarConfiguracao(Paciencia *p, char caminho[MAX_CAMINHO]) {
    if (escolherPaciencia("paciencias", caminho) <= 0) return 0;
    if (!lerPaciencia(caminho, p)) return -1;
    return 1;
}
 
// -----------------------------------------------------------------------------
// Função: jogar
// Objetivo: Inicializa a mesa com cartas baralhadas e arranca o ciclo principal.
// -----------------------------------------------------------------------------
static void jogar(Paciencia *p, Historico *h, const char *caminhoPac) {
    EstadoJogo e;
    inicializarEstadoJogo(&e, p);
    initHistorico(h);
    processarMovimentosAutomaticos(&e, p);
    loopJogo(&e, p, h, caminhoPac);
}

 
// -----------------------------------------------------------------------------
// Função: main
// Objetivo: Ponto de entrada. Aloca memória, arranca o jogo e liberta recursos no fim.
// -----------------------------------------------------------------------------

int main() {
    srand (time(NULL));
    Paciencia p;
    char caminhoPac[MAX_CAMINHO];
    Historico *h = malloc(sizeof(Historico));
    
    if (h == NULL) {
        printf("Erro fatal: Falta de memoria.\n");
        return 1;
    }
    
    int status = carregarConfiguracao(&p, caminhoPac);
    
    if (status > 0) {
        jogar(&p, h, caminhoPac);
    }
    
    free(h);
    
    return (status < 0) ? 1 : 0;
}
 

