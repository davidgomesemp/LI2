#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "golf.h"
#include "stack.h"
#include "card.h"

/* inicializarJogo: 7 pilhas com 5 cartas cada */
void test_inicializarJogo_pilhas(void) {
    GolfState estado;
    srand(42);
    inicializarJogo(&estado);

    for (int i = 0; i < 7; i++) {
        CU_ASSERT_EQUAL(size(&estado.pilha[i]), 5);
    }
}

/* inicializarJogo: discard tem 1 carta, deck tem 16 */
void test_inicializarJogo_deck_discard(void) {
    GolfState estado;
    srand(42);
    inicializarJogo(&estado);

    CU_ASSERT_FALSE(isEmpty(&estado.discard));
    CU_ASSERT_EQUAL(size(&estado.deck), 16);
}

/* verificaVitoria: 0 quando há cartas nas pilhas */
void test_verificaVitoria_falso(void) {
    GolfState estado;
    srand(42);
    inicializarJogo(&estado);
    CU_ASSERT_EQUAL(verificaVitoria(&estado), 0);
}

/* verificaVitoria: 1 quando todas as pilhas estão vazias */
void test_verificaVitoria_verdadeiro(void) {
    GolfState estado;
    inicializarJogo(&estado);

    for (int i = 0; i < 7; i++) {
        while (!isEmpty(&estado.pilha[i])) {
            tirar(&estado.pilha[i]);
        }
    }
    CU_ASSERT_EQUAL(verificaVitoria(&estado), 1);
}

/* realizarJogada: opção inválida devolve 0 */
void test_realizarJogada_opcao_invalida(void) {
    GolfState estado;
    srand(42);
    inicializarJogo(&estado);
    CU_ASSERT_EQUAL(realizarJogada(&estado, 'Z'), 0);
}

/* realizarJogada: coluna vazia devolve 0 */
void test_realizarJogada_coluna_vazia(void) {
    GolfState estado;
    inicializarJogo(&estado);

    while (!isEmpty(&estado.pilha[0])) {
        tirar(&estado.pilha[0]);
    }

    CU_ASSERT_EQUAL(realizarJogada(&estado, 'A'), 0);
}

/* Regista todos os testes de integração do módulo Golf. */
void adicionarTestesGolf(CU_pSuite suite) {
    CU_add_test(suite, "inicializarJogo pilhas",         test_inicializarJogo_pilhas);
    CU_add_test(suite, "inicializarJogo deck e discard", test_inicializarJogo_deck_discard);
    CU_add_test(suite, "verificaVitoria falso",          test_verificaVitoria_falso);
    CU_add_test(suite, "verificaVitoria verdadeiro",     test_verificaVitoria_verdadeiro);
    CU_add_test(suite, "realizarJogada opcao invalida",  test_realizarJogada_opcao_invalida);
    CU_add_test(suite, "realizarJogada coluna vazia",    test_realizarJogada_coluna_vazia);
}

/* Ponto de entrada para os testes globais do jogo Golf. Retorna o número total de falhas. */
int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();
    CU_pSuite suite = CU_add_suite("golf", NULL, NULL);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    adicionarTestesGolf(suite);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int falhas = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return falhas;
}
