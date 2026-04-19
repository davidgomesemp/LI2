#include <CUnit/Basic.h>
#include <string.h>

#include "ssimon.h"
#include "stack.h"
#include "card.h"

// -----------------------------
// TESTES
// -----------------------------

void test_indicesValidos_ok() {
    SimonState estado;
    for (int i = 0; i < 10; i++) initStack(&estado.colunas[i]);

    meter(&estado.colunas[0], getCard(0));

    CU_ASSERT_EQUAL(indicesValidos(&estado, 0, 1, 1), 1);
}

void test_indicesValidos_erro() {
    SimonState estado;
    for (int i = 0; i < 10; i++) initStack(&estado.colunas[i]);

    CU_ASSERT_EQUAL(indicesValidos(&estado, 0, 0, 1), 0);
}

// -----------------------------

void test_blocoValido_ok() {
    Carta bloco[3];

    bloco[0] = getCard(0); // A♠
    bloco[1] = getCard(1); // 2♠
    bloco[2] = getCard(2); // 3♠

    CU_ASSERT_EQUAL(blocoValido(bloco, 3), 1);
}

void test_blocoValido_erro() {
    Carta bloco[2];

    bloco[0] = getCard(0);
    bloco[1] = getCard(10); // quebra sequência

    CU_ASSERT_EQUAL(blocoValido(bloco, 2), 0);
}

// -----------------------------

void test_validaDestino_vazio() {
    Stack s;
    initStack(&s);

    Carta c = getCard(5);

    CU_ASSERT_EQUAL(validaDestino(&s, c), 1);
}

void test_validaDestino_ok() {
    Stack s;
    initStack(&s);

    meter(&s, getCard(5)); // topo = 6
    Carta base = getCard(4); // 5

    CU_ASSERT_EQUAL(validaDestino(&s, base), 1);
}

void test_validaDestino_erro() {
    Stack s;
    initStack(&s);

    meter(&s, getCard(5));
    Carta base = getCard(2);

    CU_ASSERT_EQUAL(validaDestino(&s, base), 0);
}

// -----------------------------

void test_podeMover_ok() {
    SimonState estado;
    for (int i = 0; i < 10; i++) initStack(&estado.colunas[i]);

    meter(&estado.colunas[0], getCard(4)); // 5
    meter(&estado.colunas[1], getCard(5)); // 6

    CU_ASSERT_EQUAL(podeMover(&estado, 0, 1, 1), 1);
}

void test_podeMover_erro() {
    SimonState estado;
    for (int i = 0; i < 10; i++) initStack(&estado.colunas[i]);

    meter(&estado.colunas[0], getCard(2));
    meter(&estado.colunas[1], getCard(5));

    CU_ASSERT_EQUAL(podeMover(&estado, 0, 1, 1), 0);
}

// -----------------------------

void test_moverCartas() {
    SimonState estado;
    for (int i = 0; i < 10; i++) initStack(&estado.colunas[i]);

    meter(&estado.colunas[0], getCard(4));

    moverCartas(&estado, 0, 1, 1);

    CU_ASSERT_EQUAL(size(&estado.colunas[0]), 0);
    CU_ASSERT_EQUAL(size(&estado.colunas[1]), 1);
}

// -----------------------------

void test_verificaVitoria() {
    SimonState estado;

    estado.fundacoes = 4;
    CU_ASSERT_EQUAL(verificaVitoriaSimon(&estado), 1);

    estado.fundacoes = 2;
    CU_ASSERT_EQUAL(verificaVitoriaSimon(&estado), 0);
}

// -----------------------------
// ORGANIZAÇÃO DOS TESTES
// -----------------------------

void adicionarTestes(CU_pSuite suite) {
    CU_add_test(suite, "indices validos ok", test_indicesValidos_ok);
    CU_add_test(suite, "indices validos erro", test_indicesValidos_erro);

    CU_add_test(suite, "bloco valido ok", test_blocoValido_ok);
    CU_add_test(suite, "bloco valido erro", test_blocoValido_erro);

    CU_add_test(suite, "valida destino vazio", test_validaDestino_vazio);
    CU_add_test(suite, "valida destino ok", test_validaDestino_ok);
    CU_add_test(suite, "valida destino erro", test_validaDestino_erro);

    CU_add_test(suite, "pode mover ok", test_podeMover_ok);
    CU_add_test(suite, "pode mover erro", test_podeMover_erro);

    CU_add_test(suite, "mover cartas", test_moverCartas);
    CU_add_test(suite, "verifica vitoria", test_verificaVitoria);
}

void correrTestes(void) {
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
}

// -----------------------------
// MAIN (AGORA OK < 15 statements)
// -----------------------------

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Testes_Simple_Simon", NULL, NULL);

    adicionarTestes(suite);
    correrTestes();

    CU_cleanup_registry();
    return 0;
}
