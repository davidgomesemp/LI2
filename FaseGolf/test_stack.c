#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "stack.h"
#include "card.h"

/* Stack recém-inicializada está vazia */
void test_initStack_vazia(void) {
    Stack s;
    initStack(&s);
    CU_ASSERT_TRUE(isEmpty(&s));
}

/* isEmpty: falso depois de meter uma carta */
void test_isEmpty_apos_meter(void) {
    Stack s;
    initStack(&s);
    meter(&s, getCard(0));
    CU_ASSERT_FALSE(isEmpty(&s));
}

/* size: conta corretamente */
void test_size(void) {
    Stack s;
    initStack(&s);
    CU_ASSERT_EQUAL(size(&s), 0);

    meter(&s, getCard(0));
    CU_ASSERT_EQUAL(size(&s), 1);

    meter(&s, getCard(1));
    CU_ASSERT_EQUAL(size(&s), 2);
}

/* meter/tirar: comportamento LIFO */
void test_meter_tirar_lifo(void) {
    Stack s;
    initStack(&s);
    Carta c1 = getCard(3);
    Carta c2 = getCard(7);

    meter(&s, c1);
    meter(&s, c2);

    Carta t = tirar(&s);
    CU_ASSERT_EQUAL(t.value, c2.value);
    CU_ASSERT_STRING_EQUAL(t.naipe, c2.naipe);

    t = tirar(&s);
    CU_ASSERT_EQUAL(t.value, c1.value);
}

/* tirar: stack fica menor */
void test_tirar_diminui_size(void) {
    Stack s;
    initStack(&s);
    meter(&s, getCard(0));
    meter(&s, getCard(1));
    tirar(&s);
    CU_ASSERT_EQUAL(size(&s), 1);
}

/* verTopo: devolve o topo sem remover */
void test_verTopo(void) {
    Stack s;
    initStack(&s);
    meter(&s, getCard(4));
    meter(&s, getCard(9));

    Carta topo = verTopo(&s);
    CU_ASSERT_EQUAL(topo.value, getCard(9).value);
    CU_ASSERT_EQUAL(size(&s), 2);
}

/* Regista todos os testes relacionados com a estrutura de dados Stack. */
void adicionarTestesStack(CU_pSuite suite) {
    CU_add_test(suite, "initStack vazia",       test_initStack_vazia);
    CU_add_test(suite, "isEmpty apos meter",    test_isEmpty_apos_meter);
    CU_add_test(suite, "size",                  test_size);
    CU_add_test(suite, "meter tirar LIFO",      test_meter_tirar_lifo);
    CU_add_test(suite, "tirar diminui size",    test_tirar_diminui_size);
    CU_add_test(suite, "verTopo",               test_verTopo);
}

/* Ponto de entrada para os testes da Stack e retorna o número de falhas.
*/
int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) 
        return CU_get_error();
    CU_pSuite suite = CU_add_suite("stack", NULL, NULL);
    if (!suite) { 
        CU_cleanup_registry(); 
        return CU_get_error(); 
    }
    adicionarTestesStack(suite);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int falhas = (int)CU_get_number_of_failures();
    CU_cleanup_registry();
    return falhas;
}
