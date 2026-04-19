#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "ssimon.h"
#include "stack.h"
#include "card.h"

// -----------------------------------------------------------------------------
// Função: prepararEstado
// Objetivo: Inicializa um estado vazio para testes.
// -----------------------------------------------------------------------------
void prepararEstado(SimonState *estado)
{
    estado->fundacoes = 0;

    for (int i = 0; i < 10; i++)
        initStack(&(estado->colunas[i]));
}

// -----------------------------------------------------------------------------
// Teste: indicesValidos com dados corretos.
// -----------------------------------------------------------------------------
void teste_indices_validos_ok()
{
    SimonState estado;
    prepararEstado(&estado);

    meter(&(estado.colunas[0]), getCard(0));
    meter(&(estado.colunas[0]), getCard(1));

    CU_ASSERT_EQUAL(indicesValidos(&estado, 0, 1, 1), 1);
    CU_ASSERT_EQUAL(indicesValidos(&estado, 0, 1, 2), 1);
}

// -----------------------------------------------------------------------------
// Teste: indicesValidos com dados errados.
// -----------------------------------------------------------------------------
void teste_indices_validos_erro()
{
    SimonState estado;
    prepararEstado(&estado);

    meter(&(estado.colunas[0]), getCard(0));

    CU_ASSERT_EQUAL(indicesValidos(&estado, -1, 1, 1), 0);
    CU_ASSERT_EQUAL(indicesValidos(&estado, 0, 10, 1), 0);
    CU_ASSERT_EQUAL(indicesValidos(&estado, 0, 0, 1), 0);
    CU_ASSERT_EQUAL(indicesValidos(&estado, 0, 1, 2), 0);
    CU_ASSERT_EQUAL(indicesValidos(&estado, 0, 1, 0), 0);
}

// -----------------------------------------------------------------------------
// Teste: blocoValido com sequência válida.
// -----------------------------------------------------------------------------
void teste_bloco_valido_ok()
{
    Carta bloco[3];

    bloco[0] = getCard(2);
    bloco[1] = getCard(1);
    bloco[2] = getCard(0);

    CU_ASSERT_EQUAL(blocoValido(bloco, 3), 1);
}

// -----------------------------------------------------------------------------
// Teste: blocoValido com sequência inválida.
// -----------------------------------------------------------------------------
void teste_bloco_valido_erro()
{
    Carta bloco[3];

    bloco[0] = getCard(2);
    bloco[1] = getCard(14);
    bloco[2] = getCard(0);

    CU_ASSERT_EQUAL(blocoValido(bloco, 3), 0);
}

// -----------------------------------------------------------------------------
// Teste: validaDestino numa coluna vazia.
// -----------------------------------------------------------------------------
void teste_valida_destino_vazio()
{
    Stack s;
    initStack(&s);

    CU_ASSERT_EQUAL(validaDestino(&s, getCard(0)), 1);
}

// -----------------------------------------------------------------------------
// Teste: validaDestino com movimento válido.
// -----------------------------------------------------------------------------
void teste_valida_destino_ok()
{
    Stack s;
    initStack(&s);

    meter(&s, getCard(3));
    CU_ASSERT_EQUAL(validaDestino(&s, getCard(2)), 1);
}

// -----------------------------------------------------------------------------
// Teste: validaDestino com movimento inválido.
// -----------------------------------------------------------------------------
void teste_valida_destino_erro()
{
    Stack s;
    initStack(&s);

    meter(&s, getCard(5));
    CU_ASSERT_EQUAL(validaDestino(&s, getCard(2)), 0);
}

// -----------------------------------------------------------------------------
// Teste: podeMover com jogada válida.
// -----------------------------------------------------------------------------
void teste_pode_mover_ok()
{
    SimonState estado;
    prepararEstado(&estado);

    meter(&(estado.colunas[0]), getCard(1));
    meter(&(estado.colunas[0]), getCard(0));
    meter(&(estado.colunas[1]), getCard(2));

    CU_ASSERT_EQUAL(podeMover(&estado, 0, 1, 2), 1);
}

// -----------------------------------------------------------------------------
// Teste: podeMover com jogada inválida.
// -----------------------------------------------------------------------------
void teste_pode_mover_erro()
{
    SimonState estado;
    prepararEstado(&estado);

    meter(&(estado.colunas[0]), getCard(1));
    meter(&(estado.colunas[0]), getCard(0));
    meter(&(estado.colunas[1]), getCard(5));

    CU_ASSERT_EQUAL(podeMover(&estado, 0, 1, 2), 0);
}

// -----------------------------------------------------------------------------
// Teste: moverCartas altera corretamente as colunas.
// -----------------------------------------------------------------------------
void teste_mover_cartas()
{
    SimonState estado;
    prepararEstado(&estado);

    meter(&(estado.colunas[0]), getCard(1));
    meter(&(estado.colunas[0]), getCard(0));
    meter(&(estado.colunas[1]), getCard(2));

    moverCartas(&estado, 0, 1, 2);

    CU_ASSERT_EQUAL(size(&(estado.colunas[0])), 0);
    CU_ASSERT_EQUAL(size(&(estado.colunas[1])), 3);
}

// -----------------------------------------------------------------------------
// Teste: verificaVitoriaSimon.
// -----------------------------------------------------------------------------
void teste_verifica_vitoria()
{
    SimonState estado;
    prepararEstado(&estado);

    estado.fundacoes = 4;
    CU_ASSERT_EQUAL(verificaVitoriaSimon(&estado), 1);

    estado.fundacoes = 3;
    CU_ASSERT_EQUAL(verificaVitoriaSimon(&estado), 0);
}

// -----------------------------------------------------------------------------
// Função principal dos testes.
// -----------------------------------------------------------------------------
int main()
{
    CU_pSuite suite = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    suite = CU_add_suite("Testes_Simple_Simon", NULL, NULL);

    if (suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "indices validos ok", teste_indices_validos_ok);
    CU_add_test(suite, "indices validos erro", teste_indices_validos_erro);
    CU_add_test(suite, "bloco valido ok", teste_bloco_valido_ok);
    CU_add_test(suite, "bloco valido erro", teste_bloco_valido_erro);
    CU_add_test(suite, "valida destino vazio", teste_valida_destino_vazio);
    CU_add_test(suite, "valida destino ok", teste_valida_destino_ok);
    CU_add_test(suite, "valida destino erro", teste_valida_destino_erro);
    CU_add_test(suite, "pode mover ok", teste_pode_mover_ok);
    CU_add_test(suite, "pode mover erro", teste_pode_mover_erro);
    CU_add_test(suite, "mover cartas", teste_mover_cartas);
    CU_add_test(suite, "verifica vitoria", teste_verifica_vitoria);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
