#ifndef ENGINE_H
#define ENGINE_H

#include "jogo.h"
#include "parser.h"

// -----------------------------------------------------------------------------
// Função: validarEMover
// Objetivo: Chamada principal quando o jogador tenta mover uma carta. Verifica
//           a validade dos índices, testa as regras do jogo e executa a
//           movimentação física caso seja legal.
// Retorno: 1 se o movimento foi executado com sucesso, 0 se foi inválido.
// -----------------------------------------------------------------------------
int validarEMover(EstadoJogo *estado, const Paciencia *p, int idxOrigem, int idxDestino, int qtd);

// -----------------------------------------------------------------------------
// Função: processarMovimentosAutomaticos
// Objetivo: Fica num ciclo de reações em cadeia, verificando e executando
//           movimentos automáticos até não haver mais nenhum possível.
// Retorno: 1 se pelo menos uma carta foi movida, 0 se nada aconteceu.
// -----------------------------------------------------------------------------
int processarMovimentosAutomaticos(EstadoJogo *estado, const Paciencia *p);

// -----------------------------------------------------------------------------
// Função: verificarVitoria
// Objetivo: Deve ser chamada a cada turno para validar se todas as regras
//           de vitória (WIN) definidas no ficheiro de configuração foram cumpridas.
// Retorno: 1 se o jogador ganhou o jogo, 0 caso contrário.
// -----------------------------------------------------------------------------
int verificarVitoria(const EstadoJogo *estado, const Paciencia *p);

// -----------------------------------------------------------------------------
// Função: procurarDica
// Objetivo: Varre o tabuleiro à procura da primeira jogada legal possível.
//           Se encontrar, guarda as coordenadas (origem, destino e quantidade)
//           nos apontadores fornecidos para que a interface possa sugerir o
//           movimento ao jogador.
// Retorno: 1 se encontrou uma jogada válida, 0 se não há movimentos possíveis.
// -----------------------------------------------------------------------------
int procurarDica(const EstadoJogo *estado, const Paciencia *p, int *o, int *d, int *qtd);

#endif // ENGINE_H
