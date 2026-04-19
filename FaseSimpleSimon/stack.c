#include "stack.h"
#include "card.h"
#include <stdlib.h>
#include <stdio.h>

// -----------------------------------------------------------------------------
// Função: initStack
// Objetivo: Inicializa a pilha definindo o topo como -1 (indicando que esta vazia).
// Parâmetros: s - Apontador para a pilha a ser inicializada.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void initStack (Stack *s)
{
    // Os arrays começam no índice 0. Se o topo fosse 0, significava que já lá estava 1 carta!
    // Nota: (*s).topo é a mesma coisa que s->topo. São duas formas de escrever o mesmo.
    s->topo = -1;
}

// -----------------------------------------------------------------------------
// Função: isEmpty
// Objetivo: Verifica se a pilha esta vazia.
// Parâmetros: s - Apontador para a pilha.
// Retorno: 1 (verdadeiro) se a pilha estiver vazia, ou 0 (falso) caso contrario.
// -----------------------------------------------------------------------------
int isEmpty(Stack *s)
{
    // Isto é uma verificação direta. Se o topo for -1, devolve 1 (True). Senão, devolve 0 (False).
    return s->topo == -1;
}

// -----------------------------------------------------------------------------
// Função: meter
// Objetivo: Adiciona uma carta ao topo da pilha, se esta ainda nao estiver cheia.
// Parâmetros: s - Apontador para a pilha; c - Carta a ser adicionada.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void meter (Stack *s, Carta c)
{
    // Verifica se a pilha já atingiu o limite máximo do baralho (índice 51 = 52 cartas).
    if (s->topo != 51)
    {
        s->topo++; // Primeiro subimos o "andar" do topo (ex: passa de -1 para 0).
        s->cartas[s->topo] = c; // Depois guardamos a carta nesse novo "andar".
    }
    else
    {
        printf ("Erro: Stack cheia\n");
        exit(1); // Aborta o programa imediatamente para evitar rebentar com a memória.
    }
}

// -----------------------------------------------------------------------------
// Função: tirar
// Objetivo: Remove e devolve a carta que esta atualmente no topo da pilha.
// Parâmetros: s - Apontador para a pilha de onde a carta sera removida.
// Retorno: A Carta que foi removida do topo.
// -----------------------------------------------------------------------------
Carta tirar (Stack *s)
{
    // Guardamos o índice do andar atual antes de o apagarmos.
    int i = s->topo;

    if (isEmpty(s))
    {
        printf("Erro: stack vazia\n");
        exit(1);
    }
    else
    {
        s->topo--; // Descemos o andar do topo (a carta "desaparece" da pilha virtualmente).
        return (s->cartas[i]); // Devolvemos a carta que estava no andar antigo.
    }
}

// -----------------------------------------------------------------------------
// Função: size
// Objetivo: Calcula o numero total de cartas atualmente na pilha.
// Parâmetros: s - Apontador para a pilha.
// Retorno: Inteiro correspondente ao numero de cartas na pilha.
// -----------------------------------------------------------------------------
int size (Stack *s)
{
    // Como os arrays começam em 0, se o topo estiver na posição 4, temos na verdade 5 cartas.
    return (s->topo + 1);
}

// -----------------------------------------------------------------------------
// Função: verTopo
// Objetivo: Retorna a carta no topo da pilha sem a remover, para podermos consultar o seu valor.
// Parâmetros: s - Apontador para a pilha.
// Retorno: A Carta que se encontra no topo da pilha.
// -----------------------------------------------------------------------------
Carta verTopo (Stack *s)
{
    if (isEmpty(s))
    {
        printf ("Erro: Stack vazia\n");
        exit(1);
    }
    // Apenas "espreita" a posição do array, não faz s->topo-- como na função tirar().
    return (s->cartas[s->topo]);
}
