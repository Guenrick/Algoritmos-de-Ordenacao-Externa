#include <iostream>
#include <string.h>
#include <math.h>
using namespace std;
using namespace chrono;

typedef struct {
    int chave;
    long int dado1;
    char dado2[5000];
} TipoRegistro;

bool geraArqCrescente(int N, FILE *arqEscrita) {

    for(int i = 0; i < N; i++) {
        TipoRegistro reg;
        reg.chave = i+1;
        fwrite(&reg, sizeof(TipoRegistro), 1, arqEscrita);
    }

    fclose(arqEscrita);
    return true;

}

bool geraArqDecrescente(int N, FILE *arqEscrita) {

    for(int i = N; i > 0; i--) {
        TipoRegistro reg;
        reg.chave = i;
        fwrite(&reg, sizeof(TipoRegistro), 1, arqEscrita);
    }

    fclose(arqEscrita);
    return true;

}

bool geraArqDesordenado(int N, FILE *arqEscrita) {

    TipoRegistro *registros = (TipoRegistro*)malloc(N * sizeof(TipoRegistro));

    for(int i = 0; i < N; i++) {
        TipoRegistro reg;
        reg.chave = i+1;
        registros[i] = reg;
    }
    
    for(int i = N-1; i > 0; i--) { /* embaralha o array */
        int j = rand() % (i+1);
        TipoRegistro temp = registros[i];
        registros[i] = registros[j];
        registros[j] = temp;
    }

    for(int i = 0; i < N; i++) /* realiza a escrita no arquivo */
        fwrite(&registros[i], sizeof(TipoRegistro), 1, arqEscrita);
    
    free(registros);
    fclose(arqEscrita);
    return true;

}

int main (int argc, char *argv[]) {

    if(argc < 2)
        return 1;

    string arqTipo = string(argv[1]);
    int N;

    cout << "Digite o numero de registros: \n";
    cin >> N;

    FILE *arqEscrita;
    
    
    if(arqTipo == "-c") {

        if ( (arqEscrita = fopen("registrosAscendente.bin", "wb")) == NULL ) {
            cout << "Erro na criacao do arquivo.\n";
            return 1;
        }
        geraArqCrescente(N, arqEscrita);

    } else if(arqTipo == "-d") {

        if ( (arqEscrita = fopen("registrosDescrescente.bin", "wb")) == NULL ) {
            cout << "Erro na criacao do arquivo.\n";
            return 1;
        }
        geraArqDecrescente(N, arqEscrita);

    } else if(arqTipo == "-r") {

        if ( (arqEscrita = fopen("registrosDesordenados.bin", "wb")) == NULL ) {
            cout << "Erro na criacao do arquivo.\n";
            return 1;
        }
        geraArqDesordenado(N, arqEscrita);

    }

    return 0;
}

