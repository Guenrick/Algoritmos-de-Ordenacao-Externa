#include <stdio.h>
#ifndef PESQUISA_SEQUENCIAL
#define PESQUISA_SEQUENCIAL

#define QTDPAGINAS 5 // qtd de registro / nº de paginas
#define REGISTROSPAGINA 20

typedef struct TipoRegistro {
    int chave;
    long int dado1;
    char dado2[5000];
} TipoRegistro;

bool pesquisa(int indexPagina[], int tam, TipoRegistro *registroBuscado, FILE *arq, int *cont, int *nTransferencias_pes);

#endif
