#include "jogo.h"
#include "card.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// -----------------------------------------------------------------------------
// Função: distribuirCartasPilha
// Objetivo: Retira um número específico de cartas do topo do baralho e
//           coloca-as numa pilha do tabuleiro durante o início do jogo.
// -----------------------------------------------------------------------------
static void distribuirCartasPilha(Stack *pilha, int *deck, int *idxBaralho, int qtd) {
    for (int i = 0; i < qtd; i++) {
        Carta c = getCard(deck[*idxBaralho]);
        meter(pilha, c);
        (*idxBaralho)++;
    }
}

// -----------------------------------------------------------------------------
// Função: inicializarEstadoJogo
// Objetivo: Prepara um novo jogo. Cria o baralho com 52 cartas, baralha-o, e
//           distribui as cartas pelas pilhas consoante as regras do ficheiro DSL.
// -----------------------------------------------------------------------------
void inicializarEstadoJogo(EstadoJogo *e, const Paciencia *p) {
    e->nPilhas = p->nInits;
    int deck[52];

    for (int i = 0; i < 52; i++) deck[i] = i;
    shuffleDeck(deck, 52);

    int idxBaralho = 0;
    for (int i = 0; i < e->nPilhas; i++) {
        initStack(&e->pilhas[i]);
        int qtd = p->inits[i].ncartas;
        distribuirCartasPilha(&e->pilhas[i], deck, &idxBaralho, qtd);
    }
}

// -----------------------------------------------------------------------------
// Função: initHistorico
// Objetivo: Coloca o contador de jogadas guardadas a zero.
// -----------------------------------------------------------------------------
void initHistorico(Historico *h) {
    h->total = 0;
}

// -----------------------------------------------------------------------------
// Função: guardarEstado
// Objetivo: Tira uma "fotografia" do tabuleiro atual e guarda no histórico
//           para permitir o Undo (voltar atrás). Respeita o limite máximo de jogadas.
// -----------------------------------------------------------------------------
void guardarEstado(Historico *h, const EstadoJogo *e) {
    if (h->total < MAX_HISTORICO) {
        h->estados[h->total] = *e;
        h->total++;
    }
}

// -----------------------------------------------------------------------------
// Função: desfazerJogada
// Objetivo: Recupera a última "fotografia" guardada do tabuleiro, substituindo
//           o estado atual. Diminui o contador do histórico.
// -----------------------------------------------------------------------------
int desfazerJogada(Historico *h, EstadoJogo *e) {
    if (h->total > 0) {
        h->total--;
        *e = h->estados[h->total];
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Função: cartaParaString
// Objetivo: Converte uma estrutura Carta numa string compacta (ex: "K♥")
//           para que possa ser escrita num ficheiro de texto durante o Save.
// -----------------------------------------------------------------------------
static void cartaParaString(Carta c, char *str) {
    const char* vals[] = {"A","2","3","4","5","6","7","8","9","T","J","Q","K"};
    sprintf(str, "%s%s", vals[c.value], c.naipe);
}

// -----------------------------------------------------------------------------
// Função: charParaValor
// Objetivo: Traduz o primeiro caráter de uma string de carta para o seu valor
//           matemático interno (0 a 12).
// -----------------------------------------------------------------------------
static int charParaValor(const char *str) {
    if (str[0] == '1' && str[1] == '0') return 9;
    char v = toupper(str[0]);
    const char *figuras = "A23456789TJQK";
    char *ptr = strchr(figuras, v);
    if (ptr != NULL) {
        return (int)(ptr - figuras);
    }
    return v - '1'; // Prevenção de falhas
}

// -----------------------------------------------------------------------------
// Função: extrairNaipeLetra
// Objetivo: Lida apenas com os caracteres alfabéticos.
// -----------------------------------------------------------------------------
static const char* extrairNaipeLetra(const char *str) {
    if (strpbrk(str, "sSeE")) return "♠";
    if (strpbrk(str, "hH")) return "♥";
    if (strpbrk(str, "dDoO")) return "♦";
    return "♣";
}

// -----------------------------------------------------------------------------
// Função: extrairNaipe
// Objetivo: Descobre o naipe lendo a string.
// -----------------------------------------------------------------------------
static const char* extrairNaipe(const char *str) {
    if (strstr(str, "♠")) return "♠";
    if (strstr(str, "♥")) return "♥";
    if (strstr(str, "♦")) return "♦";
    if (strstr(str, "♣")) return "♣";
    return extrairNaipeLetra(str);
}

// -----------------------------------------------------------------------------
// Função: stringParaCarta
// Objetivo: Processo inverso da gravação. Transforma uma palavra do ficheiro
//           de texto (ex: "10H") numa estrutura Carta real do jogo.
// -----------------------------------------------------------------------------
static void stringParaCarta(const char *str, Carta *c) {
    const char* vals[] = {"A","2","3","4","5","6","7","8","9","T","J","Q","K"};
    c->value = charParaValor(str);
    c->naipe = (char *)extrairNaipe(str);
    c->prnt = malloc(16 * sizeof(char));
    if (c->prnt) {
        sprintf(c->prnt, "%s%s", vals[c->value], c->naipe);
    }
}

// -----------------------------------------------------------------------------
// Função: obterNomeBase
// Objetivo: Limpa o caminho completo de um ficheiro (ex: "paciencias/golf.txt")
//           e devolve apenas o nome do ficheiro ("golf.txt") para o cabeçalho.
// -----------------------------------------------------------------------------
static const char* obterNomeBase(const char* caminho) {
    const char* barra = strrchr(caminho, '/');
    return barra ? barra + 1 : caminho;
}

// -----------------------------------------------------------------------------
// Função: salvarPilha
// Objetivo: Varre uma pilha e escreve todas as suas cartas numa linha de texto.
// -----------------------------------------------------------------------------
static void salvarPilha(FILE *f, const Stack *pilha) {
    for (int i = 0; i < size((Stack *)pilha); i++) {
        char s[16];
        cartaParaString(pilha->cartas[i], s);
        fprintf(f, "%s%s", s, (i == size((Stack *)pilha) - 1) ? "" : " ");
    }
}

// -----------------------------------------------------------------------------
// Função: salvarJogo
// Objetivo: Orquestra o processo de criação de um ficheiro '.save'. Escreve o
//           nome do jogo e despeja o conteúdo das pilhas linha a linha.
// -----------------------------------------------------------------------------
int salvarJogo(const EstadoJogo *e, const char *caminhoPac, const char *ficheiro) {
    FILE *f = fopen(ficheiro, "w");
    if (!f) return 0;

    fprintf(f, "%s\n", obterNomeBase(caminhoPac));

    for(int i = 0; i < e->nPilhas; i++) {
        if (!isEmpty((Stack*)&e->pilhas[i])) {
            salvarPilha(f, &e->pilhas[i]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return 1;
}

// -----------------------------------------------------------------------------
// Função: processarLinhaCartas
// Objetivo: Divide uma linha inteira do ficheiro de texto em palavras separadas
//           por espaços, criando cartas e empurrando-as para a respetiva pilha.
// -----------------------------------------------------------------------------
static void processarLinhaCartas(Stack *pilha, char *linha) {
    if (strlen(linha) == 0) return;
    char *token = strtok(linha, " ");
    while (token) {
        Carta c;
        stringParaCarta(token, &c);
        meter(pilha, c);
        token = strtok(NULL, " ");
    }
}

// -----------------------------------------------------------------------------
// Função: lerCabecalhoSave
// Objetivo: Extrai a primeira linha do ficheiro .save para saber qual foi o
//           jogo original que gerou aquela gravação.
// -----------------------------------------------------------------------------
static int lerCabecalhoSave(FILE *f, char *nomePacLido) {
    char linha[512];
    if (!fgets(linha, sizeof(linha), f)) return 0;
    linha[strcspn(linha, "\r\n")] = 0;
    strcpy(nomePacLido, linha);
    return 1;
}

// -----------------------------------------------------------------------------
// Função: carregarLinhaPilha
// Objetivo: Auxiliar para inicializar a pilha e processar as cartas da linha.
// -----------------------------------------------------------------------------
static void carregarLinhaPilha(EstadoJogo *e, int i, char *linha) {
    linha[strcspn(linha, "\r\n")] = 0;
    initStack(&e->pilhas[i]);
    processarLinhaCartas(&e->pilhas[i], linha);
}

// -----------------------------------------------------------------------------
// Função: carregarJogo
// Objetivo: Orquestra o processo de 'Load'. Abre o ficheiro, lê o cabeçalho e
//           reconstrói as pilhas linha a linha injetando-as no estado do jogo.
// -----------------------------------------------------------------------------
int carregarJogo(EstadoJogo *e, char *nomePacLido, const char *ficheiro) {
    FILE *f = fopen(ficheiro, "r");
    int i = 0;
    char linha[512];
    if (!f) return 0;
    if (!lerCabecalhoSave(f, nomePacLido)) {
        fclose(f);
        return 0;
    }
    while (fgets(linha, sizeof(linha), f)) {
        carregarLinhaPilha(e, i, linha);
        i++;
    }
    e->nPilhas = i;
    fclose(f);
    return 1;
}
