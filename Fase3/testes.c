#include "jogo.h"
#include "stack.h"
#include "card.h"
#include "parser.h"
#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// -----------------------------------------------------------------------------
// Função: testar_stack
// Objetivo: Valida a mecânica de LIFO, verificando push, pop e empty na Stack.
// Parâmetros: Nenhum.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void testar_stack() {
    Stack s;
    initStack(&s);
    assert(isEmpty(&s) == 1);
    assert(size(&s) == 0);
    
    meter(&s, getCard(0)); 
    meter(&s, getCard(1)); 
    assert(isEmpty(&s) == 0);
    assert(size(&s) == 2);
    
    Carta retirada = tirar(&s);
    assert(retirada.value == 1); 
    assert(size(&s) == 1);
    
    printf("[OK] Testes da Stack passaram!\n");
}

// -----------------------------------------------------------------------------
// Função: verificar_historico_undo
// Objetivo: Reduz código de asserts validando se os dados do Undo estão corretos.
// Parâmetros: h - Histórico utilizado para teste.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void verificar_historico_undo(Historico *h) {
    EstadoJogo e_recuperado;
    assert(desfazerJogada(h, &e_recuperado) == 1);
    assert(h->total == 0);
    assert(size(&e_recuperado.pilhas[0]) == 1);
    assert(desfazerJogada(h, &e_recuperado) == 0);
}

// -----------------------------------------------------------------------------
// Função: testar_undo
// Objetivo: Valida se é possível criar um snapshot e restaurá-lo perfeitamente.
// Parâmetros: Nenhum.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void testar_undo() {
    Historico *h = malloc(sizeof(Historico));
    if (h == NULL) exit(1);
    
    EstadoJogo e_inicial;
    initHistorico(h);
    e_inicial.nPilhas = 1;
    initStack(&e_inicial.pilhas[0]);
    meter(&e_inicial.pilhas[0], getCard(0));
    
    guardarEstado(h, &e_inicial);
    assert(h->total == 1);
    
    verificar_historico_undo(h);
    free(h);
    printf("[OK] Testes de Undo passaram!\n");
}

// -----------------------------------------------------------------------------
// Função: testar_save_load
// Objetivo: Grava um tabuleiro num ficheiro temporário e tenta reconstrui-lo.
// Parâmetros: Nenhum.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void testar_save_load() {
    EstadoJogo original, carregado;
    const char *ficheiro = "save_teste.txt";
    
    original.nPilhas = 1;
    initStack(&original.pilhas[0]);
    meter(&original.pilhas[0], getCard(5));  
    
    assert(salvarJogo(&original, ficheiro) == 1);
    assert(carregarJogo(&carregado, ficheiro) == 1);
    assert(carregado.nPilhas == 1);
    
    Carta c = verTopo(&carregado.pilhas[0]);
    assert(c.value == 5);
    assert(carregarJogo(&carregado, "ficheiro_fantasma.txt") == 0);
    
    printf("[OK] Testes de Save/Load passaram!\n");
}

// -----------------------------------------------------------------------------
// Função: testar_parser
// Objetivo: Valida tratamento de ficheiros faltosos e leitura correta de regras DSL.
// Parâmetros: Nenhum.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void testar_parser() {
    Paciencia p;
    assert(lerPaciencia("ficheiro_falso.paciencias", &p) == 0);

    int leu = lerPaciencia("paciencias/simplesimon.paciencias", &p);
    assert(leu == 1); 
    assert(strcmp(p.nomeJogo, "SimpleSimon") == 0);
    assert(p.baralhos == 1);
    assert(p.nInits == 14); 
    
    printf("[OK] Testes do Parser passaram!\n");
}

// -----------------------------------------------------------------------------
// Função: setup_ambiente_motor
// Objetivo: Cria regras falsas em memória para contornar leitura no teste do Motor.
// Parâmetros: p - Regras destino; e - Estado destino.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void setup_ambiente_motor(Paciencia *p, EstadoJogo *e) {
    inicializarPaciencia(p);
    strcpy(p->inits[0].tipo, "TAB");
    strcpy(p->inits[1].tipo, "TAB");
    p->nMovs = 1;
    strcpy(p->movs[0].origem, "TAB");
    strcpy(p->movs[0].destino, "TAB");
    strcpy(p->movs[0].flags, "<"); 

    e->nPilhas = 2;
    initStack(&e->pilhas[0]);
    initStack(&e->pilhas[1]);
    meter(&e->pilhas[0], getCard(3)); 
    meter(&e->pilhas[1], getCard(8)); 
}

// -----------------------------------------------------------------------------
// Função: testar_motor_jogo
// Objetivo: Força um movimento ilegal e verifica se a função principal o bloqueia.
// Parâmetros: Nenhum.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void testar_motor_jogo() {
    Paciencia p;
    EstadoJogo e;
    setup_ambiente_motor(&p, &e);

    int mov_valido = validarEMover(&e, &p, 0, 1, 1);
    assert(mov_valido == 0);

    printf("[OK] Testes da Engine passaram!\n");
}

// -----------------------------------------------------------------------------
// Função: main
// Objetivo: Dispara e orquestra todas as baterias de teste do testador.
// Parâmetros: Nenhum.
// Retorno: 0 em caso de sucesso absoluto.
// -----------------------------------------------------------------------------
int main() {
    printf("--- INICIANDO BATERIA DE TESTES (FASE 3) ---\n");
    testar_stack();
    testar_undo();
    testar_save_load();
    testar_parser();
    testar_motor_jogo();
    printf("--- TODOS OS 5 MODULOS DE TESTES PASSARAM COM SUCESSO! ---\n");
    return 0;
}
