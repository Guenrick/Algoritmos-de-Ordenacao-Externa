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
    int cont = 0;
    
    // cout << argc;

    if( (arq = fopen(argv[1], "rb")) == NULL ) {
        cout << "Erro para abrir o arquivo\n"; 
        return 1;
    }
    
    // gerando a tabela de indices das paginas
    int pos = 0;

    auto tempoInicioProc = high_resolution_clock::now();

    int menorChaveDaPagina[QTDPAGINAS];

    fseek(arq, 0, SEEK_END); // Vai para o final do arquivo
    long tamanhoArquivo = ftell(arq);
    long tamanhoRegistro = sizeof(TipoRegistro) * REGISTROSPAGINA;
    long numPaginas = tamanhoArquivo / tamanhoRegistro;
    
    if (string(argv[1]) == "registrosDecrescente.bin") {
        for (long i = numPaginas - 1; i >= 0; i--) {
            fseek(arq, i * tamanhoRegistro, SEEK_SET);
            fread(itensAux, sizeof(TipoRegistro), REGISTROSPAGINA, arq);
            menorChaveDaPagina[pos++] = itensAux[REGISTROSPAGINA - 1].chave;
            cont++;
        }
    } else {
        fseek(arq, 0, SEEK_SET); // Vai para o início do arquivo
        while (fread(itensAux, sizeof(TipoRegistro), REGISTROSPAGINA, arq)) {
            menorChaveDaPagina[pos++] = itensAux[0].chave;
            cont++;
        }
    }

    auto tempoFimProc = high_resolution_clock::now();

    nTransferencias_pre = cont;    
    // fflush(stdout);
    cout << "Forneça a chave para pesquisa: "; 
    cin >> registroBuscado.chave;

    auto tempoInicioPes = high_resolution_clock::now();

    if( pesquisa(menorChaveDaPagina, pos, &registroBuscado, arq, &cont, &nTransferencias_pes) )
        cout << "Registro localizado: " << registroBuscado.chave << endl;
    else
        cout << "Registro não localizado" << endl;

    auto tempoFimPes = high_resolution_clock::now();


       
    fclose(arq);

    auto tempoProcessamento = duration_cast<milliseconds>(tempoFimProc - tempoInicioProc);
    auto tempoPesquisa = duration_cast<milliseconds>(tempoFimPes - tempoInicioPes);
    
     /* Tempo de execução */
    cout << "\nPRÉ-PROCESSAMENTO -------" << endl;
    cout << "Número de transferências: " << nTransferencias_pre << endl;
    cout << "Comparacoes realizadas: " << cont << endl;
    cout << "Tempo de pre-processamento: " << tempoProcessamento.count() << "ms" << endl;
    /*TEMPO DE PRE-PROCESSAMENTO*/
    cout << "\n";
    cout << "PESQUISA ----------------" << endl;
    cout << "Número de transferências: " << nTransferencias_pes << endl;
    cout << "Comparacoes realizadas: " << cont << endl;
    cout << "Tempo de pesquisa: " << tempoPesquisa.count() << "ms" << endl;
    /*TEMPO DE PRE-PROCESSAMENTO*/

    return 0;
}
