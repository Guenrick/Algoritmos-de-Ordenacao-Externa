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
            TipoRegistro ri[M * 2];             // Chaves
            TipoApontador pi[M * 2 + 1];     // Ponteiros para filhos
        } U0;
        struct { // Estrutura para páginas externas
            int ne;                        // Número de registros
            TipoRegistro re[M * 4];          // Registros armazenados
        } U1;
    } UU;
} TipoPagina;

// Função para inserir em uma página que não está cheia. Insere no lugar correto, e aponta tambem
void InsereNaPaginaInt(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir) { //ApDir e o apontador do filho a direitra
    short NaoAchouPosicao; //se nao for recebe 0
    int k;
    k = Ap->UU.U0.ni;
    NaoAchouPosicao = (k > 0);

    while (NaoAchouPosicao)
    {   
        if (Reg.Chave >= Ap->UU.U0.ri[k - 1].Chave) //Verifica se oq eu quero add é maior do q o maior atual
        {
            NaoAchouPosicao = false; 
            break;           
        }
        Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k - 1];
        Ap->UU.U0.pi[k + 1] = Ap->UU.U0.pi[k];
        k--; //Se esta aqui entao a chave nao é maior
        if (k < 1) NaoAchouPosicao = false;//entao, achou posicao
    }
    Ap->UU.U0.ri[k].Chave = Reg.Chave; //k é a posicao correta
    Ap->UU.U0.pi[k + 1] = ApDir;// algumas vezes ApDir vai ser nulo
    Ap->UU.U0.ni++;
}


void InsereNaPaginaExt(TipoApontador Ap, TipoRegistro Reg) { //ApDir e o apontador do filho a direitra
    short NaoAchouPosicao; //se nao for recebe 0
    int k;
    k = Ap->UU.U1.ne;
    NaoAchouPosicao = (k > 0);

    while (NaoAchouPosicao)
    {   
        if (Reg.Chave >= Ap->UU.U1.re[k-1].Chave) //Verifica se oq eu quero add é maior do q o maior atual
        {
            NaoAchouPosicao = false; 
            break;           
        }
        Ap->UU.U1.re[k] = Ap->UU.U1.re[k - 1];
        k--; //Se esta aqui entao a chave nao é maior
        if (k < 1) NaoAchouPosicao = false;//entao, achou posicao
    }
    Ap->UU.U1.re[k] = Reg; //k é a posicao correta
    Ap->UU.U1.ne++;
}

/* Função auxiliar para inserção. Aqui ela basicamente percorre
a arvore inteira procurando um lugar para inserir o 
RegChave */
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno) 
{
    
    long i = 1; long j;
    TipoApontador ApTemp;

    if (Ap->Pt == Interna) //verifica se a pagina eh interna
    { 
        while (i < Ap->UU.U0.ni && Reg.Chave > Ap->UU.U0.ri[i - 1].Chave) i++;
        
        if (Reg.Chave == Ap->UU.U0.ri[i-1].Chave)//Se achar uma chave igual, chama recursivamente para o lado direito
        {
            Ins(Reg, Ap->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno);
        }

        if(Reg.Chave < Ap->UU.U0.ri[i - 1].Chave) i--;
        // Chamada recursivamente para o lado que
        Ins(Reg, Ap->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno);

        //Se chegou nessa parte, eh o momento da divisao.
        //Aqui o codigo na esta sendo desempilhado, e a pagina externa esta correta.
        //Funciona exatamente como a arvore B.

            if (!*Cresceu) 
        {
            //Esse cresceu determina quando o programa termina
            //Enquanto cresceu for verdadeiro, tem coisas a fazer.
            return;
        }

        if (Ap->UU.U0.ni < 2 * M) //verifica se a pagina ta completa
        //se sim, tem que fazer a divisão
        {
            InsereNaPaginaInt(Ap, *RegRetorno, *ApRetorno);
            *Cresceu = false;
            return;
        } 
        //CHEGOU AQUI, COMEÇA DIVISAO
        // Overflow (pagina completa, tem que dividir)
        //Aqui está criando uma nova pagina
        ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
        ApTemp->UU.U1.ne = 0;
        
        
        //Esse if verifica se vai ser inserir na pagina que ja existe
        //ou vai para a nova pagina
        if (i < M + 1) //i é o valor aonde eu tenho que inserir(lembrando
        //que o valor que quero inserir é o reg retorno/reg) ou seja, o i se trata da posicao do reg
        {
        /* Esse if meio que já faz a comparação de maior e menor
        se baseando apenas no i (pelo que entendi)*/
            InsereNaPaginaInt(ApTemp, Ap->UU.U0.ri[2 * M - 1], Ap->UU.U0.pi[2 * M]);//pega o ultimo elemento e taca na temp
            Ap->UU.U0.ni;
            InsereNaPaginaInt(Ap, *RegRetorno, *ApRetorno);//Regretorno é um valor que tava mais baixo e ta subindo
        }
        else 
        {
            InsereNaPaginaInt(ApTemp, *RegRetorno, *ApRetorno);
        }

        /*esse for faz com que um elemento dos maiores seja levado para a proxima pagina */
        /*é obrigatorio ser dos maiores, pois os menores continuam na mesma*/
        for(j = M + 2; j <= 2 * M; j++)
            InsereNaPaginaInt(ApTemp, Ap->UU.U0.ri[j-1], Ap->UU.U0.pi[j]);
                    /*apontador aptemp-> recebendo o mais a direita(pode ser null)*/
        Ap->UU.U0.ni = M; ApTemp->UU.U0.pi[0] = Ap->UU.U0.pi[M + 1]; /*ApTemp se trata da nova pagina. Ap é a anterior que fica com tamain M*/
        //Esse Ap->n aqui em cima recebe recebe o tamanho da metade, já que foi dividido no meio
        *RegRetorno = Ap->UU.U0.ri[M]; *ApRetorno = ApTemp; //Elementos do meio
    }


    //Nessa parte ja estou na pagina externa da chave a ser adicionada 
     if (Ap == nullptr) //Verifica se a arvore esta vazia
    { 
        *Cresceu = true;
        (*RegRetorno) = Reg;
        (*ApRetorno) = nullptr;
        return;
    }

    if (Ap->UU.U1.ne < 2 * M)//Insere na pagina se ela nao estiver cheia
    {
        InsereNaPaginaExt(Ap, *RegRetorno);  
        *Cresceu = false;
        return;
    } 

    //Aqui começa a parte da divisao.
    ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    ApTemp->UU.U1.ne = 0; 

     //Esse if verifica se vai ser inserir na pagina que ja existe ou vai para a nova pagina
    if (i < M + 1)
    {
    /* Esse if meio que já faz a comparação de maior e menor
    se baseando apenas no i (pelo que entendi)*/
        InsereNaPaginaExt(ApTemp, Ap->UU.U1.re[2 * M - 1]);//pega o ultimo elemento e taca na temp (pag nova)
        Ap->UU.U1.ne++;
        InsereNaPaginaExt(Ap, *RegRetorno); // libera espaco nas 2 linhas acima e depois adiciona o Reg necessario
    }
    else 
    {
        InsereNaPaginaExt(ApTemp, *RegRetorno); // Reg vem se for um dos 2 maiores
    }


    for(j = M + 2; j <= 2 * M; j++)
        InsereNaPaginaExt(ApTemp, Ap->UU.U1.re[j-1]);

    InsereNaPaginaExt(ApTemp, Ap->UU.U1.re[M]); //Insere o valor do meio na pagina da direita
    Ap->UU.U1.ne = M - 1; //Esse Ap->n aqui em cima recebe recebe o tamanho da metade, já que foi dividido no meio

    //Para subir, tenho que inserir numa pagina interna e apontar
    InsereNaPaginaInt();
    *RegRetorno = Ap->UU.U1.re[M]; *ApRetorno = ApTemp; //Aqui pega o valor do meio que vai subir

}

// Função principal de inserção
void Insere(TipoRegistro Reg, TipoApontador* Ap) {
    short Cresceu;
    TipoRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;
    //o ins tem 3 retornos
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
    if (Cresceu) { //se cresceu eh true, chegou na raiz. (aqui cria nova pagina raiz)
        ApTemp = (TipoPagina*) malloc(sizeof(TipoPagina));
        ApTemp->UU.U0.ni = 1;
        ApTemp->UU.U0.ri[0] = RegRetorno;
        ApTemp->UU.U0.pi[1] = ApRetorno;
        ApTemp->UU.U0.pi[0] = *Ap; *Ap = ApTemp;
    }
    // No final aptemp é atualizado com o novo endereço da raiz que foi criado 

    if (Cresceu) { //(aqui inicia a primeira pagina folha)
        ApTemp = (TipoPagina*) malloc(sizeof(TipoPagina));
        ApTemp->UU.U1.ne = 1;
        ApTemp->UU.U1.re[0] = RegRetorno;
        *Ap = ApTemp;
    }
}

// Função de pesquisa
bool Pesquisa(TipoRegistro *x, TipoApontador *Ap) 
{
    int i;
    TipoApontador Pag; //recebendo endereço da raiz
    Pag = *Ap;

    if ((*Ap)->Pt == Interna) { // Aqui pesquisa na pag interna
        i = 1;
        while (i < Pag->UU.U0.ni && x->Chave > Pag->UU.U0.ri[i - 1].Chave) i++; //pesquisa na pag
        if (x->Chave < Pag->UU.U0.ri[i - 1].Chave) //maior ou menor que o ultimo registro verificado
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
            cout << Ap->UU.U0.ri[i].Chave << " ";
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

int main() {
    TipoApontador Arvore;


    TipoRegistro Reg;

    // Inserindo alguns registros
    Reg.Chave = 10;
    Insere(Reg, &Arvore);

    Reg.Chave = 20;
    Insere(Reg, &Arvore);

    Reg.Chave = 15;
    Insere(Reg, &Arvore);

    Reg.Chave = 5;
    Insere(Reg, &Arvore);

    // Imprimindo a árvore
    cout << "Árvore B*:" << endl;
    Imprime(Arvore, 0);

    // Pesquisando na árvore
    Reg.Chave = 15;
    Pesquisa(&Reg, Arvore);

    Reg.Chave = 100;
    Pesquisa(&Reg, Arvore);
}
