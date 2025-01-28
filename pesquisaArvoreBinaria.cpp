#include "pesquisaArvoreBinaria.h"
using namespace chrono;

typedef struct {
    int chave;
    long int dado1;
    char dado2[5000];
} TipoRegistro;

typedef struct {
    TipoRegistro registro;
    long int filho_dir;
    long int filho_esq;
} TipoNo;

bool pesquisaArvoreBinaria(int chave, FILE *arqEntrada, int *nTransferencias_pes, int *cont_pes) {

    TipoNo *no = (TipoNo*)malloc(sizeof(TipoNo));

    (*nTransferencias_pes)++;
    while( fread(no, sizeof(TipoNo), 1, arqEntrada)  ) {
        (*cont_pes)++;
        if(chave > no->registro.chave)
            fseek(arqEntrada, no->filho_dir * sizeof(TipoNo), SEEK_SET);
        else {
            (*cont_pes)++;
            if(chave < no->registro.chave)
                fseek(arqEntrada, no->filho_esq * sizeof(TipoNo), SEEK_SET);
            else {
                free(no);
                return true;
            }
        }
        (*nTransferencias_pes)++;
    }

    free(no);
    return false;

}

int main(int argc, char* argv[]) {
    FILE* arqEntrada = fopen(argv[1], "rb");
    int c,  cont_pes = 0, nTransferencias_pes = 0;
    cout << "Digite a chave para busca: \n";
    cin >> c;

    auto tempoInicioPes = high_resolution_clock::now();

    
    if(pesquisaArvoreBinaria(c, arqEntrada, &nTransferencias_pes, &cont_pes)){ 
        cout << "Chave encontrada!";
    }
    else {
        cout << "Não foi possivel achar a chave.";
    }
    
    auto tempoFimPes = high_resolution_clock::now();
    
    auto tempoPesquisa = duration_cast<milliseconds>(tempoFimPes - tempoInicioPes);

    cout << "PESQUISA ----------------" << endl;
    cout << "Número de transferências: " << nTransferencias_pes << endl;
    cout << "Comparacoes realizadas: " << cont_pes << endl;
    cout << "Tempo de pesquisa: " << tempoPesquisa.count() << "ms" << endl;

    return 0;
    
}