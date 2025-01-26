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

// Função para inserir em uma página que não está cheia. Insere no lugar correto, e aponta tambem
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir) { //ApDir e o apontador do filho a direitra
    short NaoAchouPosicao; //se nao for recebe 0
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);

    while (NaoAchouPosicao)
    {   
        if (Reg.Chave >= Ap->r[k-1].Chave) //Verifica se oq eu quero add é maior do q o maior atual
        {
            NaoAchouPosicao = false; 
            break;           
        }
        Ap->r[k] = Ap->r[k-1];
        Ap->p[k + 1] = Ap->p[k];
        k--; //Se esta aqui entao a chave nao é maior
        if (k < 1) NaoAchouPosicao = false;//entao, achou posicao
    }
    Ap->r[k] = Reg; //k é a posicao correta
    Ap->p[k + 1] = ApDir;// algumas vezes ApDir vai ser nulo
    Ap->n++;
}

/* Função auxiliar para inserção. Aqui ela basicamente percorre
a arvore inteira procurando um lugar para inserir o 
RegChave */
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, 
         TipoRegistro *RegRetorno, TipoApontador *ApRetorno) 
{
    
    long i = 1; long j;
    TipoApontador ApTemp;

    if (Ap->Pt == Interna) //verifica se a pagina eh interna
    { 
        while (i < Ap->UU.U0.ni && Reg.Chave > Ap->UU.U0.ri[i - 1]) i++;
        
        if (Reg.Chave == Ap->UU.U0.ri[i-1])//Se achar uma chave igual, chama recursivamente para o lado direito
        {
            Ins(Reg, Ap->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno);
        }

        if(Reg.Chave < Ap->UU.U0.ri[i - 1]) i--;
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

        if (Ap->n < 2 * M) //verifica se a pagina ta completa
        //se sim, tem que fazer a divisão
        {
            InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
            *Cresceu = false;
            return;
        } 
        //CHEGOU AQUI, COMEÇA DIVISAO
        // Overflow (pagina completa, tem que dividir)
        //Aqui está criando uma nova pagina
        ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
        ApTemp->n = 0; 
        ApTemp->p[0] = nullptr;
        
        
        //Esse if verifica se vai ser inserir na pagina que ja existe
        //ou vai para a nova pagina
        if (i < M + 1) //i é o valor aonde eu tenho que inserir(lembrando
        //que o valor que quero inserir é o reg retorno/reg) ou seja, o i se trata da posicao do reg
        {
        /* Esse if meio que já faz a comparação de maior e menor
        se baseando apenas no i (pelo que entendi)*/
            InsereNaPagina(ApTemp, Ap->r[2 * M - 1], Ap->p[2 * M]);//pega o ultimo elemento e taca na temp
            Ap->n--;
            InsereNaPagina(Ap, *RegRetorno, *ApRetorno);//Regretorno é um valor que tava mais baixo e ta subindo
        }
        else 
        {
            InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
        }

        /*esse for faz com que um elemento dos maiores seja levado para a proxima pagina */
        /*é obrigatorio ser dos maiores, pois os menores continuam na mesma*/
        for(j = M + 2; j <= 2 * M; j++)
            InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j]);
                    /*apontador aptemp-> recebendo o mais a direita(pode ser null)*/
        Ap->n = M; ApTemp->p[0] = Ap->p[M + 1]; /*ApTemp se trata da nova pagina. Ap é a anterior que fica com tamain M*/
        //Esse Ap->n aqui em cima recebe recebe o tamanho da metade, já que foi dividido no meio
        *RegRetorno = Ap->r[M]; *ApRetorno = ApTemp; //Elementos do meio
    }

}

// Função principal de inserção
void Insere(TipoRegistro Reg, TipoApontador* Ap) {
    short Cresceu;
    TipoRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;

    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

    if (Cresceu) {
        ApTemp = (TipoPagina*) malloc(sizeof(TipoPagina));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap; *Ap = ApTemp;
    }
    //No final aptemp é atualizado com o novo endereço da raiz que foi criad


        
    }
    
    // Se nao for interna e estiver vazia, entao crio a pagina raiz.
    if (Ap == nullptr) 
    { // Se sim, cria a pagina raiz. 
        *Cresceu = true;
        (*RegRetorno) = Reg;
        (*ApRetorno) = nullptr;
        return;
    }

    // While como na pesquisa. Procura o item na pagina.
    while (i < Ap->n && Reg.Chave > Ap->r[i-1].Chave) i++; 
    if (Reg.Chave == Ap->r[i-1].Chave) 
    {
        //Esse if verifica se a chave já existe
        *Cresceu = false; // Chave já existe 
        return;
    }




    /* Esse if determina para qual lado dos ponteiros a
    chamada recursiva vai. Se o reg.chave for menor, 
    decrementa e vai para um ponteiro menor e vice-versa    */
    if(Reg.Chave < Ap->r[i - 1].Chave) i--;
    // Chamada recursiva
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);
    
    //Essa parte só é acessada quando começa a desempilhar.
    if (!*Cresceu) 
    {
        //Esse cresceu determina quando o programa termina
        //Enquanto cresceu for verdadeiro, tem coisas a fazer.
        return;
    }

    if (Ap->n < 2 * M) //verifica se a pagina ta completa
    //se sim, tem que fazer a divisão
    {
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = false;
        return;
    } 
    //CHEGOU AQUI, COMEÇA DIVISAO
    // Overflow (pagina completa, tem que dividir)
    //Aqui está criando uma nova pagina
    ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    ApTemp->n = 0; 
    ApTemp->p[0] = nullptr;
    
    
    //Esse if verifica se vai ser inserir na pagina que ja existe
    //ou vai para a nova pagina
    if (i < M + 1) //i é o valor aonde eu tenho que inserir(lembrando
    //que o valor que quero inserir é o reg retorno/reg) ou seja, o i se trata da posicao do reg
    {
    /* Esse if meio que já faz a comparação de maior e menor
    se baseando apenas no i (pelo que entendi)*/
        InsereNaPagina(ApTemp, Ap->r[2 * M - 1], Ap->p[2 * M]);//pega o ultimo elemento e taca na temp
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);//Regretorno é um valor que tava mais baixo e ta subindo
    }
    else 
    {
        InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    }

    /*esse for faz com que um elemento dos maiores seja levado para a proxima pagina */
   /*é obrigatorio ser dos maiores, pois os menores continuam na mesma*/
    for(j = M + 2; j <= 2 * M; j++)
        InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j]);
                /*apontador aptemp-> recebendo o mais a direita(pode ser null)*/
    Ap->n = M; ApTemp->p[0] = Ap->p[M + 1]; /*ApTemp se trata da nova pagina. Ap é a anterior que fica com tamain M*/
    //Esse Ap->n aqui em cima recebe recebe o tamanho da metade, já que foi dividido no meio
    *RegRetorno = Ap->r[M]; *ApRetorno = ApTemp; //Elementos do meio
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
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap; *Ap = ApTemp;
    }
    //No final aptemp é atualizado com o novo endereço da raiz que foi criad
}

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
