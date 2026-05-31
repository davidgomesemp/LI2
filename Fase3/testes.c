#include "jogo.h"
#include "stack.h"
#include "card.h"
#include "parser.h"
#include "engine.h"
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testar_stack(void) {
    Stack s;
    initStack(&s);
    CU_ASSERT_TRUE(isEmpty(&s));
    CU_ASSERT_EQUAL(size(&s), 0);
    
    meter(&s, getCard(0)); 
    meter(&s, getCard(1)); 
    CU_ASSERT_FALSE(isEmpty(&s));
    CU_ASSERT_EQUAL(size(&s), 2);
    
    Carta retirada = tirar(&s);
    CU_ASSERT_EQUAL(retirada.value, 1); 
    CU_ASSERT_EQUAL(size(&s), 1);
}

void testar_undo(void) {
    Historico *h = malloc(sizeof(Historico));
    CU_ASSERT_PTR_NOT_NULL(h);
    
    EstadoJogo e_inicial, e_recuperado;
    initHistorico(h);
    e_inicial.nPilhas = 1;
    initStack(&e_inicial.pilhas[0]);
    meter(&e_inicial.pilhas[0], getCard(0));
    
    guardarEstado(h, &e_inicial);
    CU_ASSERT_EQUAL(h->total, 1);
    
    CU_ASSERT_TRUE(desfazerJogada(h, &e_recuperado));
    CU_ASSERT_EQUAL(h->total, 0);
    CU_ASSERT_EQUAL(size(&e_recuperado.pilhas[0]), 1);
    
    free(h);
}

void testar_save_load(void) {
    EstadoJogo original, carregado;
    const char *ficheiro = "save_teste.txt";
    char nomeLido[MAX_CAMINHO];
    
    original.nPilhas = 1;
    initStack(&original.pilhas[0]);
    meter(&original.pilhas[0], getCard(5)); 
    
    CU_ASSERT_TRUE(salvarJogo(&original, "teste.paciencias", ficheiro));
    CU_ASSERT_TRUE(carregarJogo(&carregado, nomeLido, ficheiro));
    
    CU_ASSERT_EQUAL(carregado.nPilhas, 1);
    CU_ASSERT_STRING_EQUAL(nomeLido, "teste.paciencias");
    
    Carta c = verTopo(&carregado.pilhas[0]);
    CU_ASSERT_EQUAL(c.value, 5);
}

void testar_parser(void) {
    Paciencia p;
    CU_ASSERT_FALSE(lerPaciencia("ficheiro_falso.paciencias", &p));

    int leu = lerPaciencia("paciencias/simplesimon.paciencias", &p);
    CU_ASSERT_TRUE(leu); 
    CU_ASSERT_STRING_EQUAL(p.nomeJogo, "SimpleSimon");
    CU_ASSERT_EQUAL(p.baralhos, 1);
}

void testar_motor_jogo(void) {
    Paciencia p;
    EstadoJogo e;
    
    strcpy(p.inits[0].tipo, "TAB");
    strcpy(p.inits[1].tipo, "TAB");
    p.nMovs = 1;
    strcpy(p.movs[0].origem, "TAB");
    strcpy(p.movs[0].destino, "TAB");
    strcpy(p.movs[0].flags, "<"); 

    e.nPilhas = 2;
    initStack(&e.pilhas[0]);
    initStack(&e.pilhas[1]);
    meter(&e.pilhas[0], getCard(3)); 
    meter(&e.pilhas[1], getCard(8)); 

    int mov_valido = validarEMover(&e, &p, 0, 1, 1);
    CU_ASSERT_FALSE(mov_valido);
}

static void setup_dica(Paciencia *p, EstadoJogo *e) {
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
}

void testar_dica(void) {
    Paciencia p;
    EstadoJogo e;
    int orig, dest, qtd;

    setup_dica(&p, &e);

    // --- CENÁRIO 1: Jogada Válida ---
    meter(&e.pilhas[0], getCard(3));
    meter(&e.pilhas[1], getCard(4));

    CU_ASSERT_TRUE(procurarDica(&e, &p, &orig, &dest, &qtd));
    CU_ASSERT_EQUAL(orig, 0);
    CU_ASSERT_EQUAL(dest, 1);
    CU_ASSERT_EQUAL(qtd, 1);

    // --- CENÁRIO 2: Jogada Inválida ---
    initStack(&e.pilhas[0]);
    initStack(&e.pilhas[1]);

    meter(&e.pilhas[0], getCard(8));
    meter(&e.pilhas[1], getCard(3));

    CU_ASSERT_FALSE(procurarDica(&e, &p, &orig, &dest, &qtd));
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Suite_Projeto", NULL, NULL);
    if (NULL == suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(suite, "Testar Stack", testar_stack)) ||
        (NULL == CU_add_test(suite, "Testar Undo", testar_undo)) ||
        (NULL == CU_add_test(suite, "Testar Save/Load", testar_save_load)) ||
        (NULL == CU_add_test(suite, "Testar Parser", testar_parser)) ||
        (NULL == CU_add_test(suite, "Testar Motor", testar_motor_jogo)) ||
        (NULL == CU_add_test(suite, "Testar Dica", testar_dica)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return CU_get_error();
}
