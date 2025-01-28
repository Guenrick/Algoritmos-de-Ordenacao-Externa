#include <iostream>
#include "pesquisaSequencial.h"
#include <chrono>
using namespace std;
using namespace chrono;

auto duracao_pre = 0;
int nTransferencias_pre = 0;
int nTransferencias_pes = 0;

int main(int argc, char *argv[]) {

    FILE *arq;
    TipoRegistro itensAux[REGISTROSPAGINA], registroBuscado;  
    int menorChaveDaPagina[QTDPAGINAS], cont = 0, ordem;
    
    // cout << argc << endl;

    if( (arq = fopen(argv[1], "rb")) == NULL ) {
        cout << "Erro para abrir o arquivo\n"; 
        return 1;
    }
    
    // gerando a tabela de indices das paginas
    int pos = 0;
    while( fread(itensAux, sizeof(TipoRegistro), REGISTROSPAGINA, arq) ) {
        menorChaveDaPagina[pos++] = itensAux[0].chave;
        cont++;
    }
    nTransferencias_pre = cont;

    // fflush(stdout);
    cout << "Forneça a chave para pesquisa: "; 
    cin >> registroBuscado.chave;

    /* TEMPO DE PESQUISA */
    if( pesquisa(menorChaveDaPagina, pos, &registroBuscado, arq, &cont, &nTransferencias_pes))
        cout << "Registro localizado: " << registroBuscado.chave << endl;
    else
        cout << "Registro não localizado" << endl;
    
    fclose(arq);
    auto tempoFim = high_resolution_clock::now();

    
    /* Tempo de execução */
    cout << "PRÉ-PROCESSAMENTO -------" << endl;
    cout << "Número de transferências: " << nTransferencias_pre << endl;
    /*TEMPO DE PRE-PROCESSAMENTO*/
    cout << "\n";
    cout << "PESQUISA ----------------" << endl;
    cout << "Comparacoes realizadas: " << cont << endl;
    cout << "Número de transferências: " << nTransferencias_pes << endl;
    /*TEMPO DE PRE-PROCESSAMENTO*/
    
    return 0;
}
