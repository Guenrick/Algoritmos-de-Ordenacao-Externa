#include <iostream>
#include <stdlib.h>

using namespace std;

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

bool adicionaNo(FILE *arq_saida, TipoNo *no, TipoNo *no_pai, bool insere, int *numero_de_nos);

int main(int argc, char *argv[]) {
    
    FILE *arq_entrada, *arq_saida;
    TipoRegistro *registro;
    TipoNo *no, *no_pai;
    int numero_de_nos = 0;
    bool arvore_esta_vazia = true;

    no = (TipoNo*) malloc(sizeof(TipoNo));
    no_pai = (TipoNo*) malloc(sizeof(TipoNo));
    registro = (TipoRegistro*) malloc(sizeof(TipoRegistro));

    if( (arq_entrada = fopen(argv[1], "rb")) == NULL ) {
        cout << "Erro para abrir o arquivo\n"; 
        return 1;
    }

    if( (arq_saida = fopen("registros_arvore_binaria.bin", "wb+")) == NULL ) {
        cout << "Erro na criacao do arquivo de saida.\n";
        return 1;
    }
    
    while( fread(registro, sizeof(TipoRegistro), 1, arq_entrada) ) {

        no->registro = *registro;
        no->filho_dir = -1;
        no->filho_esq = -1;

        adicionaNo(arq_saida, no, no_pai, arvore_esta_vazia, &numero_de_nos);

        fseek(arq_saida, 0, SEEK_SET); // retorna para o comeco do arq_saida para ler o no pai
        fread(no_pai, sizeof(TipoNo), 1, arq_saida);
        fseek(arq_saida, 0, SEEK_SET); // retorna para o inicio da arvore para iniciar processo recursivo
        
        arvore_esta_vazia = false;

    }

    fclose(arq_entrada);
    fclose(arq_saida);
    free(no);
    free(no_pai);
    free(registro);

    cout << "Numero de nos: "<< numero_de_nos << endl;

}

bool adicionaNo(FILE *arq_saida, TipoNo *no, TipoNo *no_pai, bool insere, int *numero_de_nos) {

    if(insere) { // arq_saida esta vazio ou alcancou registro com filho vago
         
        fwrite(no, sizeof(TipoNo), 1, arq_saida);
        (*numero_de_nos)++;
        return true;

    } else {
        
        if (no->registro.chave < no_pai->registro.chave) { // filho menor que pai
            
            if(no_pai->filho_esq == -1) {
                no_pai->filho_esq = *numero_de_nos;
                fwrite(no_pai, sizeof(TipoNo), 1, arq_saida);
                fseek(arq_saida, 0, SEEK_END);
                return adicionaNo(arq_saida, no, no_pai, true, numero_de_nos);
            } else {
                fseek(arq_saida, (no_pai->filho_esq * sizeof(TipoNo)), SEEK_SET);
                fread(no_pai, sizeof(TipoNo), 1, arq_saida); // reseta no_pai para o primeiro no da arvore
                fseek(arq_saida, - (1 * sizeof(TipoNo)), SEEK_CUR);
                return adicionaNo(arq_saida, no, no_pai, false, numero_de_nos);
            }

        } else if (no->registro.chave > no_pai->registro.chave) {
            
            if(no_pai->filho_dir == -1) {
                no_pai->filho_dir = *numero_de_nos;
                fwrite(no_pai, sizeof(TipoNo), 1, arq_saida);  
                fseek(arq_saida, 0, SEEK_END);
                return adicionaNo(arq_saida, no, no_pai, true, numero_de_nos);
            } else {
                fseek(arq_saida, (no_pai->filho_dir * sizeof(TipoNo)), SEEK_SET);
                fread(no_pai, sizeof(TipoNo), 1, arq_saida); // reseta no_pai para o primeiro no da arvore
                fseek(arq_saida, - (1 * sizeof(TipoNo)), SEEK_CUR);
                return adicionaNo(arq_saida, no, no_pai, false, numero_de_nos);
            }
        }      
    }
    return false;
}