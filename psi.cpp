#include <iostream>
#include "pesquisaSequencial.h"
#include <chrono>
using namespace std;
using namespace chrono;

auto duracao_pre = 0;
int n_buscas_pre = 0;
int nTransferencias_pre = 0;
int nTransferencias_pes = 0;

int main(int argc, char *argv[]) {

    //auto tempoInicio = high_resolution_clock::now();
    FILE *arq;
    TipoRegistro itensAux[REGISTROSPAGINA], registroBuscado;  
    int menorChaveDaPagina[QTDPAGINAS], cont = 0;
    
    // cout << argc;

    if( (arq = fopen(argv[1], "rb")) == NULL ) {
        cout << "Erro para abrir o arquivo\n"; 
        return 1;
    }
    
    // gerando a tabela de indices das paginas
    auto inicio_pre = std::chrono::high_resolution_clock::now();
    int pos = 0;
    while( fread(itensAux, sizeof(TipoRegistro), REGISTROSPAGINA, arq) ) {
        menorChaveDaPagina[pos++] = itensAux[0].chave;
        cont++;
    }
    auto fim_pre = std::chrono::high_resolution_clock::now();
    // calculando o tempo de execucao da geracao da tabela de indices (pre-processamento)
    auto duracao_pre = std::chrono::duration_cast<std::chrono::microseconds>(fim_pre - inicio_pre);
    nTransferencias_pre = cont;

    // fflush(stdout);
    cout << "Forneça a chave para pesquisa: "; 
    cin >> registroBuscado.chave;

    auto inicio_pes = std::chrono::high_resolution_clock::now();
    if( pesquisa(menorChaveDaPagina, pos, &registroBuscado, arq, &cont, &n_buscas_pre, &nTransferencias_pes) ){
        cout << "Registro localizado: " << registroBuscado.chave << endl;
        auto fim_pes = std::chrono::high_resolution_clock::now();
    }
    else
        cout << "Registro não localizado" << endl;
        auto fim_pes = std::chrono::high_resolution_clock::now();

    // calculando o tempo de execucao da pesquisa
    auto duracao_pes = std::chrono::duration_cast<std::chrono::microseconds>(fim_pes - inicio_pes);

    fclose(arq);
    //auto tempoFim = high_resolution_clock::now();

    //auto tempoExecucao = duration_cast<milliseconds>(tempoFim - tempoInicio);
    cout << "PRÉ-PROCESSAMENTO -------" << endl;
    cout << "Número de transferências: " << nTransferencias_pre << endl;
    cout << "Número de buscas: " << n_buscas_pre << endl;
    cout << "Tempo de execucao: " << duracao_pre.count() << "ms" << endl;

    cout << "PESQUISA ----------------" << endl;
    cout << "Número de transferências: " << nTransferencias_pes << endl;
    cout << "Comparacoes realizadas: " << cont << endl;
    cout << "Tempo de execucao: " << duracao_pes.count() << "ms" << endl;
    return 0;
}
