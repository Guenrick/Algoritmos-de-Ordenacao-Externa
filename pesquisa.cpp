#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {

    /* selecao do metodo de pesquisa */
    switch (stoi(argv[1])) {
    case 1:
        /* executa ACESSO SEQUENCIAL INDEXADO */
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

    int qntd_registros = stoi(argv[2]);

    /* selecao da ordenacao do arquivo */
    switch (stoi(argv[3])) {
    case 1:
        /* arquivo ascendente */
        break;
    case 2:
        /* arquivo descendente */
        break;
    case 3:
        /* arquivo desodernado aleatoriamente */
        break;  
    default:
        break;
    }

    int chave = stoi(argv[4]);
    
    if(argc > 5 && string(argv[5]) == "-P")
        /* chaves de pesquisa dos registros do arquivo considerado sao apresentadas na tela */

    return 0;

}