#include <iostream>
#include "pesquisaSequencial.h"

bool buscaBinaria(TipoRegistro pagina[], int qtdItens, int chave, int pEsq, int pDir, int pMeio, TipoRegistro *registro, int *cont) {

    if(pEsq > pDir)
        return false;

    if(chave > pagina[pMeio].chave)
        pEsq = pMeio + 1;
    else if (chave < pagina[pMeio].chave)
        pDir = pMeio - 1;
    else if (chave == pagina[pMeio].chave) {
        *registro = pagina[pMeio];
        return true;
    }

    pMeio = (pDir + pEsq) / 2;
    (*cont)++;

    return buscaBinaria(pagina, qtdItens, chave, pEsq, pDir, pMeio, registro, cont);

}

bool pesquisa(int menorChaveDaPagina[], int tam, TipoRegistro *registroBuscado, FILE *arq, int *cont) {

    int qtdItens, desloc, numeroDaPagina = 0;
    TipoRegistro pagina[REGISTROSPAGINA];

    while(numeroDaPagina < tam && menorChaveDaPagina[numeroDaPagina] <= registroBuscado->chave) 
        numeroDaPagina++; // busca pela pagina onde o item pode estar

    if (numeroDaPagina == 0) 
        return 0; // item nao existe no arquivo
    else {
        qtdItens = REGISTROSPAGINA;        
        if (numeroDaPagina == tam)  { /* caso estejamos na ultima pagina, existe a possibilidade de que ela nao esteja completamente preenchida. Assim, calculamos quantos itens existem nela */
            fseek(arq, 0, SEEK_END); 
            qtdItens = (ftell(arq)/sizeof(TipoRegistro)) % REGISTROSPAGINA; /* calculamos o resto da divisão da quantidade total de registros pela quantidade de itens por pagina */

            if (qtdItens == 0) /* se o resto for 0, isso significa que a ultima pagina tambem esta completamente preenchida */
                qtdItens = REGISTROSPAGINA; 
        }
    }

    // realiza a leitura da pagina
    desloc = (numeroDaPagina-1) * REGISTROSPAGINA * sizeof(TipoRegistro); /* calcula o tamanho do deslocamento a ser feito no arquivo */
    fseek(arq, desloc, SEEK_SET);
    fread(&pagina, sizeof(TipoRegistro), qtdItens, arq);

    int pEsq = 0, pDir = qtdItens - 1, pMeio = pDir / 2; 
    return buscaBinaria(pagina, qtdItens, registroBuscado->chave, pEsq, pDir, pMeio, registroBuscado, cont);

}

