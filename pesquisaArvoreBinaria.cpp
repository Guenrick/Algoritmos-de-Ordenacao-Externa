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
    (*nTransferencias_pes)--;

    free(no);
    return false;

}

int main(int argc, char* argv[]) {
    FILE* arqEntrada = fopen(argv[1], "rb");
    cout << "Digite a chave para busca: \n";
    int c,  cont_pes = 0, nTransferencias_pes = 0;
    cin >> c;

    auto tempoInicio = high_resolution_clock::now();

    if(pesquisaArvoreBinaria(c, arqEntrada, &nTransferencias_pes, &cont_pes)) 
        cout << "Chave encontrada!";
    else 
        cout << "Não foi possivel achar a chave.";
    
    auto tempoFim = high_resolution_clock::now();
    auto tempoExecucao = duration_cast<milliseconds>(tempoFim - tempoInicio);
    cout << "PESQUISA ----------------" << endl;
    cout << "Número de transferências: " << nTransferencias_pes << endl;
    cout << "Comparacoes realizadas: " << cont_pes << endl;
    /*TEMPO DE PRE-PROCESSAMENTO*/
    cout << "\nTempo de pesquisa: " << tempoExecucao.count() << "ms\n";
    return 0;
    
}