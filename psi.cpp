#include <iostream>
#include "pesquisaSequencial.h"
#include <chrono>
using namespace std;
using namespace chrono;

int main(int argc, char *argv[]) {

    auto tempoInicio = high_resolution_clock::now();
    FILE *arq;
    TipoRegistro itensAux[REGISTROSPAGINA], registroBuscado;  
    int menorChaveDaPagina[QTDPAGINAS], cont = 0;
    
    // cout << argc;

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
    
    // fflush(stdout);
    cout << "Forneça a chave para pesquisa: "; 
    cin >> registroBuscado.chave;

    if( pesquisa(menorChaveDaPagina, pos, &registroBuscado, arq, &cont) )
        cout << "Registro localizado: " << registroBuscado.chave << endl;
    else
        cout << "Registro não localizado" << endl;
    
    fclose(arq);
    auto tempoFim = high_resolution_clock::now();

    auto tempoExecucao = duration_cast<milliseconds>(tempoFim - tempoInicio);
    cout << "Tempo de execucao: " << tempoExecucao.count() << "ms" << endl;
    cout << "Comparacoes realizadas: " << cont << endl;
    return 0;
}
