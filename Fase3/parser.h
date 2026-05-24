#ifndef PARSER_H
#define PARSER_H

#define MAX_TIPOS 20
#define MAX_PILHAS 60
#define MAX_MOVS 100
#define MAX_NOME 50
#define MAX_FLAGS 20
#define MAX_CAMINHO 256

typedef struct {
    char nome[MAX_NOME];
    char flags[MAX_FLAGS];
} TipoPilha;

typedef struct {
    char tipo[MAX_NOME];
    int ncartas;
} InitPilha;

typedef struct {
    char origem[MAX_NOME];
    char destino[MAX_NOME];
    char flags[MAX_FLAGS];
} RegraMov;

typedef struct {
    char tipo[MAX_NOME];
    int ncartas;
} RegraWin;

typedef struct {
    char nomeJogo[MAX_NOME];
    int baralhos;

    TipoPilha tipos[MAX_TIPOS];
    int nTipos;

    InitPilha inits[MAX_PILHAS];
    int nInits;

    RegraMov movs[MAX_MOVS];
    int nMovs;

    RegraMov autos[MAX_MOVS];
    int nAutos;

    RegraWin wins[MAX_MOVS];
    int nWins;
} Paciencia;

void inicializarPaciencia(Paciencia *p);
int lerPaciencia(const char *caminho, Paciencia *p);
int listarPaciencias(const char *pasta, char ficheiros[][MAX_CAMINHO], int maxFicheiros);

#endif
