#ifndef geradorDeArquivo_h
#define geradorDeArquivo_h
#include <iostream>

typedef struct TipoRegistro TipoRegistro;

bool geraArqCrescente(int, FILE*);
bool geraArqDecrescente(int, FILE*);
bool geraArqDesordenado(int, FILE*);

#endif