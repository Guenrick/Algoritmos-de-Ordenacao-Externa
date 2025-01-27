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
            TipoRegistro re[M * 2];          // Registros armazenados
        } U1;
    } UU;
} TipoPagina;

// Função para inserir em uma página que não está cheia. Insere no lugar correto, e aponta tambem
void InsereNaPaginaInt(TipoApontador Ap, TipoChave Chave, TipoApontador ApDir) { //ApDir e o apontador do filho a direitra
    short NaoAchouPosicao; //se nao for recebe 0
    int k;
    k = Ap->UU.U0.ni;
    NaoAchouPosicao = (k > 0);

    while (NaoAchouPosicao)
    {   
        if (Chave >= Ap->UU.U0.ri[k - 1]) //Verifica se oq eu quero add é maior do q o maior atual
        {
            NaoAchouPosicao = false; 
            break;           
        }
        Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k - 1];
        Ap->UU.U0.pi[k + 1] = Ap->UU.U0.pi[k];
        k--; //Se esta aqui entao a chave nao é maior
        if (k < 1) NaoAchouPosicao = false;//entao, achou posicao
    }
    Ap->UU.U0.ri[k] = Chave; //k é a posicao correta
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
    cout << "Inseriu " << Reg.Chave << "\n";
    Ap->UU.U1.re[k] = Reg; //k é a posicao correta
    Ap->UU.U1.ne++;
}

/* Função auxiliar para inserção. Aqui ela basicamente percorre
a arvore inteira procurando um lugar para inserir o 
RegChave */
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno, TipoApontador ApPagAuxiliar) 
{
    
    long i = 1; long j;
    TipoApontador ApTemp;

    //Nessa parte ja estou na pagina externa da chave a ser adicionada 
    if (Ap == nullptr) //Verifica se a arvore esta vazia
    { 
        *Cresceu = true;
        (*RegRetorno) = Reg;
        (*ApRetorno) = nullptr;
        ApPagAuxiliar->Pt = Externa;
        return;
    }

    if (Ap->Pt == Interna) //verifica se a pagina eh interna
    { 

        int i = 1;

        while (i < Ap->UU.U0.ni && Reg.Chave > Ap->UU.U0.ri[i - 1]) i++;
        
        if (Reg.Chave == Ap->UU.U0.ri[i -1])//Se achar uma chave igual, chama recursivamente para o lado direito

        while (i < Ap->UU.U0.ni && Reg.Chave > Ap->UU.U0.ri[i - 1]) i++;
        
        if (Reg.Chave == Ap->UU.U0.ri[i-1])//Se achar uma chave igual, chama recursivamente para o lado direito

        {
            Ins(Reg, Ap->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno, ApPagAuxiliar);
        }

        if(Reg.Chave < Ap->UU.U0.ri[i - 1]) i--;
        // Chamada recursivamente para o lado que
        Ins(Reg, Ap->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno, ApPagAuxiliar);

        //Se chegou nessa parte, eh o momento da divisao.
        //Aqui o codigo na esta sendo desempilhado, e a pagina externa esta correta.
        //Funciona exatamente como a arvore B.

        if (!*Cresceu) 
        {
            //Esse cresceu determina quando o programa termina
            //Enquanto cresceu for verdadeiro, tem coisas a fazer.
            return;
        }

        if (Ap->UU.U0.ni < 2 * M) // verificar se a pagina esta completa
        {
            /* nao esta completa */
            InsereNaPaginaInt(Ap, RegRetorno->Chave, *ApRetorno);
            *Cresceu = false;
            return;
        } 

        //CHEGOU AQUI, COMEÇA DIVISAO POIS A PAGINA ESTA COMPLETA
        ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
        ApTemp->UU.U0.ni = 0;
        ApTemp->UU.U0.pi[0] = nullptr;
        

        if (i < M + 1) 
        {
            InsereNaPaginaInt(ApTemp, Ap->UU.U0.ri[2 * M - 1], Ap->UU.U0.pi[2 * M]);//pega o ultimo elemento e taca na temp
            Ap->UU.U0.ni--;
            InsereNaPaginaInt(Ap, RegRetorno->Chave, *ApRetorno);//Regretorno é um valor que tava mais baixo e ta subindo
        }
        else 
        {
            InsereNaPaginaInt(ApTemp, RegRetorno->Chave, *ApRetorno);
        }

        for(j = M + 2; j <= 2 * M; j++)
            InsereNaPaginaInt(ApTemp, Ap->UU.U0.ri[j-1], Ap->UU.U0.pi[j]);
                
        Ap->UU.U0.ni = M; 
        ApTemp->UU.U0.pi[0] = Ap->UU.U0.pi[M + 1]; 

        RegRetorno->Chave = Ap->UU.U0.ri[M]; 

        *RegRetorno = Ap->UU.U0.ri[M]; 

        *ApRetorno = ApTemp; //Elementos do meio

    }



    // se a pagina eh externa: 
        
    while (i < Ap->UU.U1.ne && Reg.Chave > Ap->UU.U1.re[i - 1].Chave) i++;
    
    if (i < (2 * M))//Insere na pagina se ela nao estiver cheia

    // se a pagina eh externa: 

    if (Ap->UU.U1.ne < 2 * M)//Insere na pagina se ela nao estiver cheia

    {
        cout << "Entrou no insere pag externa\n";
        InsereNaPaginaExt(Ap, Reg);  
        *Cresceu = false;
        return;
    } 

    //Aqui começa a parte da divisao.
    ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    ApTemp->UU.U1.ne = 0; 

    cout << "Iniciou processo de divisao pagina externa" << endl;
    
    


    while (i < Ap->UU.U1.ne && Reg.Chave > Ap->UU.U1.re[i - 1].Chave) i++;


     //Esse if verifica se vai ser inserir na pagina que ja existe ou vai para a nova pagina
     cout << i << endl;
    if (i < M + 1)
    {
    /* Esse if meio que já faz a comparação de maior e menor
    se baseando apenas no i (pelo que entendi)*/
        InsereNaPaginaExt(ApTemp, Ap->UU.U1.re[2 * M - 1]);//pega o ultimo elemento e taca na temp (pag nova)
        Ap->UU.U1.ne++;
        InsereNaPaginaExt(Ap, Reg); // libera espaco nas 2 linhas acima e depois adiciona o Reg necessario
    }
    else 
    {
        InsereNaPaginaExt(ApTemp, Reg); // Reg vem se for um dos 2 maiores
    }

    cout << "Move ultimo valor da pagina atual para nova pagina\n";
    for(j = M + 2; j <= 2 * M; j++)
        InsereNaPaginaExt(ApTemp, Ap->UU.U1.re[j-1]);

    InsereNaPaginaExt(ApTemp, Ap->UU.U1.re[M]); //Insere o valor do meio na pagina da direita
    Ap->UU.U1.ne = M; //Esse Ap->n aqui em cima recebe recebe o tamanho da metade, já que foi dividido no meio

    //Para subir, tenho que inserir numa pagina interna e apontar
    *RegRetorno = Ap->UU.U1.re[M];
    *ApRetorno = ApTemp; //Aqui pega o valor do meio que vai subir

    *Cresceu = true;
    ApPagAuxiliar->Pt = Interna;
   
}

// Função principal de inserção
void Insere(TipoRegistro Reg, TipoApontador* Ap, TipoApontador* ApPagAuxiliar) {

    short Cresceu;
    TipoRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;
    //o ins tem 3 retornos
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno, *ApPagAuxiliar);

    if ( Cresceu && (*ApPagAuxiliar)->Pt == Interna ) { //se cresceu eh true, chegou na raiz. (aqui cria nova pagina raiz)
        ApTemp = (TipoPagina*) malloc(sizeof(TipoPagina));
        ApTemp->UU.U0.ni = 1;
        ApTemp->UU.U0.ri[0] = RegRetorno.Chave;
        ApTemp->UU.U0.pi[1] = ApRetorno;
        ApTemp->UU.U0.pi[0] = *Ap;

        ApTemp->Pt = Interna;
        *Ap = ApTemp;
        cout << "Primeira pagina interna\n";
    }
    // No final aptemp é atualizado com o novo endereço da raiz que foi criado 
      
    if ( Cresceu && (*ApPagAuxiliar)->Pt == Externa ) { //(aqui inicia a primeira pagina folha)

        *Ap = ApTemp;
    }
    // No final aptemp é atualizado com o novo endereço da raiz que foi criado 

    if (Cresceu && (*ApPagAuxiliar)->Pt == Externa ) { //(aqui inicia a primeira pagina folha)

        ApTemp = (TipoPagina*) malloc(sizeof(TipoPagina));
        ApTemp->UU.U1.ne = 1;
        ApTemp->UU.U1.re[0] = RegRetorno;
        ApTemp->Pt = Externa;
        *Ap = ApTemp;

    }
}

// Função de pesquisa
bool Pesquisa(TipoRegistro *x, TipoApontador *Ap) 
{
    int i = 1;
    if ((*Ap)->Pt == Interna) { // Aqui pesquisa na pag interna

        cout << "Pesquisa entrou na pagina interna\n";
        while (i < (*Ap)->UU.U0.ni && x->Chave > (*Ap)->UU.U0.ri[i - 1]) i++; //pesquisa na pag
        if (x->Chave < (*Ap)->UU.U0.ri[i - 1]) //maior ou menor que o ultimo registro verificado
            Pesquisa(x, &(*Ap)->UU.U0.pi[i - 1]);

        i = 1;
        while (i < Pag->UU.U0.ni && x->Chave > Pag->UU.U0.ri[i - 1]) i++; //pesquisa na pag
        if (x->Chave < Pag->UU.U0.ri[i - 1]) //maior ou menor que o ultimo registro verificado
            Pesquisa(x, &Pag->UU.U0.pi[i - 1]);

        else
            Pesquisa(x, &(*Ap)->UU.U0.pi[i]); //se achar o valor, vai para o filho do lado direito
        return true;
        //faz esse processo até encontrar uma pagina externa e ir para o while de baixo.
    }

    // Pesquisa na pagina externa
    while (i < (*Ap)->UU.U1.ne && x->Chave > (*Ap)->UU.U1.re[i - 1].Chave) {
        i++; //quando chego nessa parte ja estou na pagina certa do item (se ele existir)
        cout << "Aqui imprime as chaves da pagina externa" << endl;

    }

    if (x->Chave == (*Ap)->UU.U1.re[i - 1].Chave)
    {
        cout << "Entrou aqui\n";
        *x = (*Ap)->UU.U1.re[i - 1]; //retorna o registro completo
        return true;
    }

    else{
        // printf("TipoRegistro nao esta na arvore\n");
        return false;
    
}

int main() {
    TipoApontador Arvore = nullptr;
    TipoApontador Auxiliar = (TipoApontador)malloc(sizeof(TipoPagina));

    TipoRegistro *Reg;
    Reg = (TipoRegistro*)malloc(sizeof(TipoRegistro));

    // Exemplo de inserções
    for (int i = 1; i <= 10; i++) {
        Reg->Chave = i;
        cout << "Adicionando registro: " << Reg->Chave << endl;
        Insere(*Reg, &Arvore, &Auxiliar);
    }
    
    cout << "Antes da pesquisa\n";
  
    Reg->Chave = 7;
    fflush(stdin);
    if (Pesquisa(Reg, &Arvore)) {
        cout << "Chave encontrada: " << Reg->Chave << "\n";
    } else {
        cout << "Chave não encontrada.\n";
    }

    cout << "Depois da pesquisa\n";
    free(Auxiliar);
    free(Reg);


    return 0;
}

    Reg.Chave = 100;
    Pesquisa(&Reg, Arvore);
}

