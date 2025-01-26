#include <stdio.h>
#ifndef PESQUISA_SEQUENCIAL
#define PESQUISA_SEQUENCIAL

#define QTDPAGINAS 5 // qtd de registros / 20
#define REGISTROSPAGINA 20

typedef struct TipoRegistro {
    int chave;
    long int dado1;
    char dado2[5000];
} TipoRegistro;

bool pesquisa(int indexPagina[], int tam, TipoRegistro *registroBuscado, FILE *arq, int *cont, int *n_buscas_pre, int *nTransferencias_pes);

#endif
