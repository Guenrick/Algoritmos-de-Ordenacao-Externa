#include <stdio.h>
#include <iostream>
#include <string>
#include "psi.h"

using namespace std;

// Argumentos: <metodo> <qntd_registros> <ordenacao_arquivo> <chave> [-P]

// <metodo>:    
// 1 - Pesquisa Sequencial Indexada
// 2 - Arvore Binaria de Pesquisa em Memoria Externa
// 3 - Arvore B
// 4 - Arvore B*

// <qntd_registros>: quantidade de registros a serem considerados

// <ordenacao_arquivo>:
// 1 - Registros em ordem crescente
// 2 - Registros em ordem decrescente
// 3 - Registros desordenados

// <chave>: chave a ser pesquisada

// [-P]: apresenta as chaves de pesquisa dos registros do arquivo considerado na tela

int main(int argc, char *argv[]) {


    int qntd_registros = stoi(argv[2]);

    // De 0 a 100
    int chaves_alestorias[20] = {0, 5, 10, 15, 20, 25, 30, 35, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};

    // Transformar de acordo com o numero de registros que o usuario deseja
    for (int i = 0; i < 20; i++) {
        chaves_alestorias[i] = chaves_alestorias[i] * (qntd_registros / 100);
    }

    int chave_desejada = stoi(argv[4]);

    string arq_path;

    /* selecao da ordenacao do arquivo */
    switch (stoi(argv[3])) {
    case 1: //registrosAscendente.bin
        arq_path = "registrosAscendente.bin";
        break;
    case 2: //registrosDecrescente.bin
        arq_path = "registrosDecrescente.bin";
        break;
    case 3: //registrosDesordenados.bin
        arq_path = "registrosDesordenados.bin";
        break;  
    default:
        break;
    }


    /* selecao do metodo de pesquisa */
    switch (stoi(argv[1])) {
    case 1:
        psi(arq_path.c_str(), chave_desejada);
        break;
    case 2:
        /* executa ARVORE BINÁRIA DE PESQUISA EM MEMORIA EXTERNA */
        break;
    case 3:
        /* executa ARVORE B */
        break;
    case 4:
        /* executa ARVORE B* */
        break;    
    default:
        break;
    }

    if(argc > 5 && string(argv[5]) == "-P")
        /* chaves de pesquisa dos registros do arquivo considerado sao apresentadas na tela */
    return 0;

}