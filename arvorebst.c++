#include <iostream>
using namespace std;

#define M 2  // Grau mínimo da árvore B*


// Definição das estruturas
typedef long TipoChave; 

typedef struct TipoRegistro {
    TipoChave Chave;
} TipoRegistro;

typedef enum { Interna, Externa } TipoIntExt;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
    TipoIntExt Pt; // Define se a página é interna ou externa
    union {
        struct { // Estrutura para páginas internas
            int ni;                        // Número de chaves
            TipoChave ri[M * 2];             // Chaves
            TipoApontador pi[M * 2 + 1];     // Ponteiros para filhos
        } U0;
        struct { // Estrutura para páginas externas
            int ne;                        // Número de registros
            TipoRegistro re[M * 4];          // Registros armazenados
        } U1;
    } UU;
} TipoPagina;

// // Prototipação de funções
// void Inicializa(TipoApontador* Arvore);
// void Insere(TipoRegistro Reg, TipoApontador* Ap);
// void Pesquisa(TipoRegistro* x, TipoApontador Ap);
// void Imprime(TipoApontador Ap, int Nivel);

// Função para inicializar a árvore
void Inicializa(TipoApontador* Arvore) {
    *Arvore = nullptr; 
}

// // Função para inserir uma chave em uma página interna
// void InsereNaPaginaInterna(TipoApontador Ap, TipoChave Reg, TipoApontador ApDir) {
//     int k = Ap->UU.U0.ni;
//     while (k > 0 && Reg < Ap->UU.U0.ri[k - 1]) {
//         Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k - 1];
//         Ap->UU.U0.pi[k + 1] = Ap->UU.U0.pi[k];
//         k--;
//     }
//     Ap->UU.U0.ri[k] = Reg;
//     Ap->UU.U0.pi[k + 1] = ApDir;
//     Ap->UU.U0.ni++;
// }

// // Função para inserir em uma página externa
// void InsereNaPaginaExterna(TipoApontador Ap, TipoRegistro Reg) {
//     int k = Ap->UU.U1.ne;
//     while (k > 0 && Reg.Chave < Ap->UU.U1.re[k - 1].Chave) {
//         Ap->UU.U1.re[k] = Ap->UU.U1.re[k - 1];
//         k--;
//     }
//     Ap->UU.U1.re[k] = Reg;
//     Ap->UU.U1.ne++;
// }

// // Função principal de inserção
// void Insere(TipoRegistro Reg, TipoApontador* Ap) {
//     // Se a árvore está vazia, cria a primeira página externa
//     if (*Ap == nullptr) {
//         *Ap = new TipoPagina();
//         (*Ap)->Pt = Externa;
//         (*Ap)->UU.U1.ne = 1;
//         (*Ap)->UU.U1.re[0] = Reg;
//         return;
//     }

//     // Implementação do restante do algoritmo de inserção aqui
//     // Incluindo lógica para redistribuição e divisão
//     InsereNaPaginaExterna(*Ap, Reg); // Simples para este exemplo
// }

// Função de pesquisa
bool Pesquisa(TipoRegistro *x, TipoApontador *Ap) 
{
    int i;
    TipoApontador Pag; //recebendo endereço da raiz
    Pag = *Ap;

    if ((*Ap)->Pt == Interna) { // Aqui pesquisa na pag interna
        i = 1;
        while (i < Pag->UU.U0.ni && x->Chave > Pag->UU.U0.ri[i - 1]) i++; //pesquisa na pag
        if (x->Chave < Pag->UU.U0.ri[i - 1]) //maior ou menor que o ultimo registro verificado
            Pesquisa(x, &Pag->UU.U0.pi[i - 1]);
        else
            Pesquisa(x, &Pag->UU.U0.pi[i]); //se achar o valor, vai para o filho do lado direito
        return;
        //faz esse processo até encontrar uma pagina externa e ir para o while de baixo.
    }

    i = 1; // Pesquisa na pagina externa
    while (i < Pag->UU.U1.ne && x->Chave > Pag->UU.U1.re[i - 1].Chave)
        i++; //quando chego nessa parte ja estou na pagina certa do item (se ele existir)
    if (x->Chave == Pag->UU.U1.re[i - 1].Chave)
    {
        *x = Pag->UU.U1.re[i - 1]; //retorna o registro completo
        return true;
    }

    else{
        printf("TipoRegistro nao esta na arvore\n");
        return false;
    }
}


// Função para imprimir a árvore
void Imprime(TipoApontador Ap, int Nivel) {
    if (Ap == nullptr) return;

    cout << "Nível " << Nivel << ": ";
    if (Ap->Pt == Interna) {
        for (int i = 0; i < Ap->UU.U0.ni; i++) {
            cout << Ap->UU.U0.ri[i] << " ";
        }
        cout << endl;
        for (int i = 0; i <= Ap->UU.U0.ni; i++) {
            Imprime(Ap->UU.U0.pi[i], Nivel + 1);
        }
    } else {
        for (int i = 0; i < Ap->UU.U1.ne; i++) {
            cout << Ap->UU.U1.re[i].Chave << " ";
        }
        cout << endl;
    }
}

// Função principal
int main() {
    // TipoApontador Arvore;
    // Inicializa(&Arvore);

    // TipoRegistro Reg;

    // // Inserindo alguns registros
    // Reg.Chave = 10;
    // Insere(Reg, &Arvore);

    // Reg.Chave = 20;
    // Insere(Reg, &Arvore);

    // Reg.Chave = 15;
    // Insere(Reg, &Arvore);

    // Reg.Chave = 5;
    // Insere(Reg, &Arvore);

    // // Imprimindo a árvore
    // cout << "Árvore B*:" << endl;
    // Imprime(Arvore, 0);

    // // Pesquisando na árvore
    // Reg.Chave = 15;
    // Pesquisa(&Reg, Arvore);

    // Reg.Chave = 100;
    // Pesquisa(&Reg, Arvore);

    // return 0;
}
