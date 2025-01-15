#include <iostream>
#include <string.h>
using namespace std;

#define N 100

typedef struct {
    int chave;
    long int dado1;
    char dado2[5000];
} TipoRegistro;

int main() {

    FILE *arqEscrita;
    if( (arqEscrita = fopen("registros.bin", "wb")) == NULL ) {
        cout << "Erro na criacao do arquivo.\n";
        return 1;
    }

    for(int i = 0; i < N; i++) {
        TipoRegistro reg;
        reg.chave = i+1;
        fwrite(&reg, sizeof(TipoRegistro), 1, arqEscrita);
    }

    fclose(arqEscrita);
    return 0;
}
