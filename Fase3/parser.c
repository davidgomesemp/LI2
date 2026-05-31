#include "parser.h"

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

typedef int (*ParserComando)(Paciencia *p, const char *linha);

typedef struct {
    const char *nome;
    ParserComando parser;
} ComandoDSL;

// -----------------------------------------------------------------------------
// Função: removerComentario
// Objetivo: Limpa o texto a partir do símbolo '#' numa linha lida.
// Parâmetros: linha - String contendo a instrução.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void removerComentario(char *linha) {
    char *comentario = strchr(linha, '#');

    if (comentario != NULL) {
        *comentario = '\0';
    }
}

// -----------------------------------------------------------------------------
// Função: saltarEspacos
// Objetivo: Ignora os espaços em branco iniciais de uma string.
// Parâmetros: linha - String original.
// Retorno: Apontador para o primeiro caractere útil.
// -----------------------------------------------------------------------------
static char *saltarEspacos(char *linha) {
    while (isspace((unsigned char)*linha)) {
        linha++;
    }

    return linha;
}

// -----------------------------------------------------------------------------
// Função: linhaVazia
// Objetivo: Verifica se a linha lida contém apenas quebras ou é vazia.
// Parâmetros: linha - String a avaliar.
// Retorno: 1 se for vazia, 0 caso contrário.
// -----------------------------------------------------------------------------
static int linhaVazia(const char *linha) {
    while (isspace((unsigned char)*linha)) {
        linha++;
    }

    return *linha == '\0';
}

// -----------------------------------------------------------------------------
// Função: terminaCom
// Objetivo: Confirma se uma linha inicia com um determinado comando.
// Parâmetros: linha - String lida; palavra - Prefixo esperado.
// Retorno: 1 se começar com a palavra, 0 caso contrário.
// -----------------------------------------------------------------------------
static int terminaCom(const char *texto, const char *sufixo) {
    size_t tamTexto = strlen(texto);
    size_t tamSufixo = strlen(sufixo);

    return tamTexto >= tamSufixo &&
           strcmp(texto + tamTexto - tamSufixo, sufixo) == 0;
}

// -----------------------------------------------------------------------------
// Função: parseJogo
// Objetivo: Interpreta o nome da paciência a partir da diretiva NAME.
// Parâmetros: p - Estrutura Paciencia; linha - Linha lida do ficheiro.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static int parseJogo(Paciencia *p, const char *linha) {
    return sscanf(linha, "%*s %49s", p->nomeJogo) == 1;
}

// -----------------------------------------------------------------------------
// Função: parseBaralhos
// Objetivo: Interpreta a quantidade de baralhos da diretiva DECKS.
// Parâmetros: p - Estrutura Paciencia; linha - Linha lida.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static int parseBaralhos(Paciencia *p, const char *linha) {
    return sscanf(linha, "%*s %d", &p->baralhos) == 1;
}

// -----------------------------------------------------------------------------
// Função: parseTipo
// Objetivo: Regista novos tipos de pilhas a partir da diretiva TIPO.
// Parâmetros: p - Estrutura Paciencia; linha - Linha lida.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static int parseTipo(Paciencia *p, const char *linha) {
    TipoPilha *tipo;

    if (p->nTipos >= MAX_TIPOS) return 0;
    tipo = &p->tipos[p->nTipos];

    if (sscanf(linha, "%*s %49s %19s", tipo->nome, tipo->flags) != 2) {
        return 0;
    }

    p->nTipos++;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: parseInit
// Objetivo: Define as pilhas do tabuleiro e sua quantidade de cartas inicial.
// Parâmetros: p - Estrutura Paciencia; linha - Linha lida.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static int parseInit(Paciencia *p, const char *linha) {
    InitPilha *init;

    if (p->nInits >= MAX_PILHAS) return 0;
    init = &p->inits[p->nInits];

    if (sscanf(linha, "%*s %49s %d", init->tipo, &init->ncartas) != 2) {
        return 0;
    }

    p->nInits++;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: parseRegra
// Objetivo: Extrai as flags (regras) de uma linha para aplicar aos movimentos.
// Parâmetros: flags - Destino da extração; linha - Linha contendo as flags.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static int parseRegra(RegraMov *regra, const char *linha) {
    return sscanf(linha, "%*s %49s %49s %19s",
                  regra->origem,
                  regra->destino,
                  regra->flags) == 3;
}

// -----------------------------------------------------------------------------
// Função: parseMov
// Objetivo: Processa e regista uma regra de movimento manual válida (MOV).
// Parâmetros: p - Estrutura Paciencia; linha - Linha lida.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static int parseMov(Paciencia *p, const char *linha) {
    if (p->nMovs >= MAX_MOVS) return 0;
    if (!parseRegra(&p->movs[p->nMovs], linha)) return 0;

    p->nMovs++;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: parseAuto
// Objetivo: Processa regras automáticas declaradas no ficheiro (AUTO).
// Parâmetros: p - Estrutura Paciencia; linha - Linha lida.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static int parseAuto(Paciencia *p, const char *linha) {
    if (p->nAutos >= MAX_MOVS) return 0;
    if (!parseRegra(&p->autos[p->nAutos], linha)) return 0;

    p->nAutos++;
    return 1;
}

// -----------------------------------------------------------------------------
// Função: parseWin
// Objetivo: Processa condições de vitória estipuladas no jogo (WIN).
// Parâmetros: p - Estrutura Paciencia; linha - Linha lida.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static int parseWin(Paciencia *p, const char *linha) {
    RegraWin *win;

    if (p->nWins >= MAX_MOVS) return 0;
    win = &p->wins[p->nWins];

    if (sscanf(linha, "%*s %49s %d", win->tipo, &win->ncartas) != 2) {
        return 0;
    }

    p->nWins++;
    return 1;
}

static const ComandoDSL comandos[] = {
    {"JOGO", parseJogo},
    {"BARALHOS", parseBaralhos},
    {"TIPO", parseTipo},
    {"INIT", parseInit},
    {"MOV", parseMov},
    {"AUTO", parseAuto},
    {"WIN", parseWin},
};

// -----------------------------------------------------------------------------
// Função: interpretarLinha
// Objetivo: Encaminha a linha limpa para o parser correspondente.
// Parâmetros: p - Estrutura Paciencia; linha - Texto limpo.
// Retorno: 1 se interpretada com sucesso, 0 se ignorada.
// -----------------------------------------------------------------------------
static int interpretarLinha(Paciencia *p, const char *linha) {
    char comando[MAX_NOME];
    int nComandos = (int)(sizeof(comandos) / sizeof(comandos[0]));

    if (sscanf(linha, "%49s", comando) != 1) return 1;

    for (int i = 0; i < nComandos; i++) {
        if (strcmp(comando, comandos[i].nome) == 0) {
            return comandos[i].parser(p, linha);
        }
    }

    return 0;
}

// -----------------------------------------------------------------------------
// Função: inicializarPaciencia
// Objetivo: Limpa a memória de uma estrutura pondo variáveis a zero.
// Parâmetros: p - Estrutura a inicializar.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
void inicializarPaciencia(Paciencia *p) {
    memset(p, 0, sizeof(*p));
}

// -----------------------------------------------------------------------------
// Função: lerLinhasFicheiro
// Objetivo: Lê um ficheiro de regras em ciclo, limpa e envia para interpretação.
// Parâmetros: f - Ficheiro de texto aberto; p - Estrutura a preencher.
// Retorno: Nenhum.
// -----------------------------------------------------------------------------
static void lerLinhasFicheiro(FILE *f, Paciencia *p) {
    char linha[512];
    while (fgets(linha, sizeof(linha), f)) {
        removerComentario(linha);
        char *limpa = saltarEspacos(linha);
        if (!linhaVazia(limpa)){ 
		interpretarLinha(p,limpa);
	}
    }
}

// -----------------------------------------------------------------------------
// Função: lerPaciencia
// Objetivo: Abre e orquestra a leitura completa do ficheiro DSL.
// Parâmetros: caminho - Ficheiro a abrir; p - Estrutura destino.
// Retorno: 1 sucesso, 0 se ficheiro não existir.
// -----------------------------------------------------------------------------
int lerPaciencia(const char *caminho, Paciencia *p) {
    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        printf("%s: No such file or directory\n", caminho);
        return 0;
    }
    inicializarPaciencia(p);
    lerLinhasFicheiro(f, p);
    fclose(f);
    return 1;
}

// -----------------------------------------------------------------------------
// Função: listarPaciencias
// Objetivo: Recolhe caminhos de todos os ficheiros .paciencias de uma diretoria.
// Parâmetros: pasta - Diretório; ficheiros - Matriz de destino; max_ficheiros - Limite.
// Retorno: O número total de ficheiros de jogos encontrados.
// -----------------------------------------------------------------------------
int listarPaciencias(const char *pasta, char ficheiros[][MAX_CAMINHO], int maxFicheiros) {
    DIR *dir = opendir(pasta);
    struct dirent *entrada;
    int total = 0;

    if (dir == NULL) {
        perror(pasta);
        return -1;
    }

    while ((entrada = readdir(dir)) != NULL && total < maxFicheiros) {
        if (terminaCom(entrada->d_name, ".paciencia") ||
            terminaCom(entrada->d_name, ".paciencias")) {
            snprintf(ficheiros[total], MAX_CAMINHO, "%s/%s", pasta, entrada->d_name);
            total++;
        }
    }

    closedir(dir);
    return total;
}
