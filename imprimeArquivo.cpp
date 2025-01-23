#include <iostream>

typedef struct {
    int chave;
    long int dado1;
    char dado2[5000];
} TipoRegistro;

int main(int argc, char *argv[]) {

    if(argc == 2) {

        FILE *arqLeitura;
        arqLeitura = fopen(argv[1], "rb");

        TipoRegistro reg;
        while( fread(&reg, sizeof(TipoRegistro), 1, arqLeitura) ) {
            printf("%d\n", reg.chave); 
        }

    }
    
}