#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "card.h"

/* getCard devolve o valor numérico correto */
void test_getCard_valor(void) {
    Carta c = getCard(0);
    CU_ASSERT_EQUAL(c.value, 0);

    Carta k = getCard(12);
    CU_ASSERT_EQUAL(k.value, 12);

    Carta as_copas = getCard(13);
    CU_ASSERT_EQUAL(as_copas.value, 0);
}

/* getCard devolve o naipe correto */
void test_getCard_naipe(void) {
    Carta espada = getCard(0);
    CU_ASSERT_STRING_EQUAL(espada.naipe, "♠");

    Carta copa = getCard(13);
    CU_ASSERT_STRING_EQUAL(copa.naipe, "♥");

    Carta ouro = getCard(26);
    CU_ASSERT_STRING_EQUAL(ouro.naipe, "♦");

    Carta pau = getCard(39);
    CU_ASSERT_STRING_EQUAL(pau.naipe, "♣");
}

/* getCard: campo prnt não é NULL */
void test_getCard_prnt_nao_nulo(void) {
    Carta c = getCard(5);
    CU_ASSERT_PTR_NOT_NULL(c.prnt);
}

/* getValue devolve o valor correto */
void test_getValue(void) {
    Carta c = getCard(7);
    CU_ASSERT_EQUAL(getValue(c), 7);
}

/* getSuite devolve o naipe correto */
void test_getSuite(void) {
    Carta c = getCard(26);
    CU_ASSERT_STRING_EQUAL(getSuite(c), "♦");
}

/* podeJogar: diferença de 1 → válido */
void test_podeJogar_valido(void) {
    Carta a = getCard(5);
    Carta b = getCard(6);
    CU_ASSERT_TRUE(podeJogar(a, b));

    Carta c = getCard(10);
    Carta d = getCard(9);
    CU_ASSERT_TRUE(podeJogar(c, d));
}

/* podeJogar: diferença != 1 → inválido */
void test_podeJogar_invalido(void) {
    Carta a = getCard(0);
    Carta b = getCard(2);
    CU_ASSERT_FALSE(podeJogar(a, b));

    Carta c = getCard(5);
    Carta d = getCard(5);
    CU_ASSERT_FALSE(podeJogar(c, d));
}

/* Valida se todas as 52 cartas estão presentes no array após o shuffle. */
void validarPresencaDeck(int deck[], int encontrado[]) {
    for (int i = 0; i < 52; i++) {
        CU_ASSERT(deck[i] >= 0 && deck[i] < 52);
        encontrado[deck[i]] = 1;
    }
}

/* Garante que nenhuma carta foi duplicada ou apagada durante a aleatorização. */
void test_shuffleDeck_contem_todos(void) {
    int deck[52], encontrado[52] = {0};
    for (int i = 0; i < 52; i++) deck[i] = i;
    shuffleDeck(deck, 52);
    validarPresencaDeck(deck, encontrado);
    for (int i = 0; i < 52; i++) {
        CU_ASSERT_EQUAL(encontrado[i], 1);
    }
}

/* Função para registar os testes relacionados com o card.c */
void adicionarTestesCard(CU_pSuite suite) {
    CU_add_test(suite, "getCard valor",            test_getCard_valor);
    CU_add_test(suite, "getCard naipe",            test_getCard_naipe);
    CU_add_test(suite, "getCard prnt nao nulo",    test_getCard_prnt_nao_nulo);
    CU_add_test(suite, "getValue",                 test_getValue);
    CU_add_test(suite, "getSuite",                 test_getSuite);
    CU_add_test(suite, "podeJogar valido",         test_podeJogar_valido);
    CU_add_test(suite, "podeJogar invalido",       test_podeJogar_invalido);
    CU_add_test(suite, "shuffleDeck contem todos", test_shuffleDeck_contem_todos);
}

/*
*Ponto de entrada para os testes de unidade do módulo Card.
*Retorna o número total de falhas encontradas (0 se todos os testes passarem).
*/
int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) return CU_get_error();
    CU_pSuite suite = CU_add_suite("card", NULL, NULL);
    if (!suite) { CU_cleanup_registry(); return CU_get_error(); }
    adicionarTestesCard(suite);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int falhas = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return falhas;
}
