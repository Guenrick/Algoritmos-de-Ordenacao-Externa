#ifndef PSI_H
#define PSI_H

#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>

#define QTDPAGINAS 5
#define REGISTROSPAGINA 20

typedef struct TipoRegistro {
    int chave;
    long int dado1;
    char dado2[5000];
} TipoRegistro;

bool buscaBinaria(TipoRegistro pagina[], int qtdItens, int chave, int pEsq, int pDir, int pMeio, TipoRegistro *registro, int *cont);
bool pesquisa(int menorChaveDaPagina[], int tam, TipoRegistro *registroBuscado, FILE *arq, int *cont, int *nTransferencias_pes);
int psi(std::string arq_path, int chave);

#endif // PSI_H