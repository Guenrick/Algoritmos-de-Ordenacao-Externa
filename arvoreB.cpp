#include <iostream>
using namespace std;
#define M 2

typedef int TipoChave;

typedef struct {
    TipoChave chave;
    /* demais componentes do registro */
} TipoRegistro;

typedef struct TipoPagina* pPagina;

typedef struct TipoPagina {
    short n;
    TipoRegistro r[M];
    pPagina p[M+1];
} TipoPagina;

void arvoreInicia(pPagina pNav) {
    pNav = NULL;
}

bool pesquisa(TipoRegistro *registro, pPagina pNav) {
    int i = 1; /* variavel auxiliar para acessar os vetores de registros e ponteiros */

    if (pNav == NULL) /* condicional para caso a arvore esteja vazia OU o registro buscado nao existe na arvore */
        return false;
    

    while( i < pNav->n && registro->chave > pNav->r[i-1].chave) /* realiza a pesquisa sequencial no vetor registros na pagina */
        i++;

    if (registro->chave == pNav->r[i-1].chave) { /* chave encontrada */
        *registro = pNav->r[i-1];
        return true;
    }
    
    if (registro->chave < pNav->r[i-1].chave) /* chave nao encontrada, segue filho a esquerda, pois a chave eh menor */
        return pesquisa(registro, pNav->p[i-1]);
    else    /* segue filho a direita */
        return pesquisa(registro, pNav->p[i]);
}

void navegaArvoreB(pPagina pNav) {
    int i = 0;

    if (pNav == NULL) return;

    while(i <= pNav->n) {
        navegaArvoreB(pNav->p[i]);
        if(i != pNav->n)
            std::cout << pNav->r[i].chave << " ";
        i++;
    }
}

void InsereNaPagina(pPagina Ap, TipoRegistro reg, pPagina ApDir) {
    int k = Ap->n;
    while(k > 0) {

        if(reg.chave >= Ap->r[k-1].chave) {
            break;
        }

        Ap->r[k] = Ap->r[k-1];
        Ap->p[k+1] = Ap->p[k];

        k--;
    }

    Ap->r[k] = reg;
    Ap->p[k+1] = ApDir;
    Ap->n++;
}

void ins(TipoRegistro reg, pPagina pNav, bool *cresceu, TipoRegistro *regRetorno, pPagina *ApRetorno) {

    long i = 1, j;
    pPagina ApTemp;

    if (pNav == NULL) { /* arvore esta vazia: deve criar pagina raiz, portanto cresceu eh true. Alem disso, tambem indica quando uma pagina folha foi alcancada */
        *cresceu = true;
        *regRetorno = reg;
        *ApRetorno = NULL;

        return;
    }

    while (i < pNav->n && reg.chave > pNav->r[i-1].chave) 
        i++;

    if(reg.chave == pNav->r[i-1].chave) {
        cout << "Registro ja esta presente!\n";
        *cresceu = false;
        return;
    }

    if(reg.chave < pNav->r[i-1].chave) i--; /* calcula para qual dos filhos devemos prosseguir na arvore: esquerda ou direita */
    ins(reg, pNav->p[i], cresceu, regRetorno, ApRetorno); 

    /* uma vez que uma chamada recursiva eh finalizada por meio do return, a chamada anterior segue daqui: */
    if(*cresceu == false) return; // se cresceu for falso, nenhuma nova manipulacao precisa ser feita na pagina

    if(pNav->n < M) { /* pagina possui espaco para novos registros */
        InsereNaPagina(pNav, *regRetorno, *ApRetorno);
        *cresceu = false;
        return;
    }
    /* caso haja overflow, ou seja, a pagina ja esteja cheia, uma nova pagina eh criada para iniciar o processo de divisao dos registros */
    ApTemp = (pPagina) malloc(sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;

    if (i < (M/2)+1) {
        InsereNaPagina(ApTemp, pNav->r[M-1], pNav->p[M]);
        pNav->n--;
        InsereNaPagina(pNav, *regRetorno, *ApRetorno);
    } else
        InsereNaPagina(ApTemp, *regRetorno, *ApRetorno);
    
    for(j = (M/2)+2; j <= M; j++)
        InsereNaPagina(ApTemp, pNav->r[j-1], pNav->p[j]);
    
    pNav->n = M; 
    ApTemp->p[0] = pNav->p[(M/2)+1];
    *regRetorno = pNav->r[M/2];
    *ApRetorno = ApTemp;

}

bool insere(TipoRegistro registro, pPagina *pNav) {
    bool cresceu = false;
    TipoRegistro registroRetorno;
    pPagina ApRetorno, ApTemp;

    ins(registro, *pNav, &cresceu, &registroRetorno, &ApRetorno);

    if (cresceu) { /* arvore cresce na altura pela raiz */
        ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
        ApTemp->n = 1;
        ApTemp->r[0] = registroRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *pNav;
        *pNav = ApTemp;
    }
}