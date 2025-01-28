#include <iostream>
using namespace std;
#define M 2// Ordem mínima (t)

int nTransferencias_pre = 0, nComparacoes_pre = 0; //temp
int nTransferencias_pes = 0, nComparacoes_pes = 0; //t

// Definição das estruturas
typedef long TipoChave;

typedef struct TipoRegistro {
    TipoChave Chave;
} TipoRegistro;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
    short n; // Número de chaves no nó
    TipoRegistro r[2 * M ]; // Array de registros (chaves)
    TipoApontador p[2 * M + 1]; // Array de ponteiros para filhos
} TipoPagina;


// Função para inserir em uma página que não está cheia. Insere no lugar correto, e aponta tambem
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir) {
    nTransferencias_pre++;
    short NaoAchouPosicao; //se nao for recebe 0
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);

    while (NaoAchouPosicao)
    {   
        if (Reg.Chave >= Ap->r[k-1].Chave) //Verifica se oq eu quero add é maior do q o maior atual
        {
            nComparacoes_pre++;
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

    // Verifica se a arvore esta vazia.
    if (Ap == nullptr) 
    { // Se sim, cria a pagina raiz. Alem disso, quando der nulo  
     //quer dizer que chegou numa pagina folha.
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

    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

    if (Cresceu) {
        ApTemp = (TipoPagina*) malloc(sizeof(TipoPagina));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap; *Ap = ApTemp;
    }
    //No final aptemp é atualizado com o novo endereço da raiz que foi criado
}

bool Pesquisa(TipoRegistro* x, TipoApontador Ap) {
    if (Ap == nullptr) {
        return false; // Chave não encontrada
    }

    long i = 1; // Inicialização de i

    // Pesquisa sequencial para encontrar o intervalo desejado
    while (i < Ap->n && x->Chave > Ap->r[i - 1].Chave) {
        nComparacoes_pes++;
        i++;
    }

    if (x->Chave == Ap->r[i - 1].Chave) {
        nComparacoes_pes++;
        *x = Ap->r[i - 1]; // Registro encontrado
        return true;
    }

    if (x->Chave < Ap->r[i - 1].Chave) {
        nComparacoes_pes++;
        return Pesquisa(x, Ap->p[i - 1]); // Pesquisa no filho à esquerda
    } 
    else
    {
        return Pesquisa(x, Ap->p[i]); // Pesquisa no filho à direita
    }
}

void imprimirArvore(TipoApontador Ap, int nivel = 0) {
    if (Ap == nullptr) {
        return;
    }

    // Imprime as chaves do nó atual
    cout << "Nivel " << nivel << " (n = " << Ap->n << "): ";
    for (int i = 0; i < Ap->n; i++) {
        cout << Ap->r[i].Chave << " ";
    }
    cout << endl;

    // Imprime os filhos recursivamente
    for (int i = 0; i <= Ap->n; i++) {
        imprimirArvore(Ap->p[i], nivel + 1);
    }
}

void freeArvore(TipoApontador Ap) {
    if (Ap == nullptr) {
        return; // Se o nó for nulo, não há nada para liberar
    }

    // Libera os filhos recursivamente
    for (int i = 0; i <= Ap->n; i++) {
        freeArvore(Ap->p[i]);
    }

    // Libera o nó atual
    delete Ap;
}
int main() {
    TipoApontador raiz = nullptr;
    TipoRegistro reg;

    // Inserindo alguns registros na árvore
    reg.Chave = 10;
    Insere(reg, &raiz);

    reg.Chave = 20;
    Insere(reg, &raiz);

    reg.Chave = 5;
    Insere(reg, &raiz);

    reg.Chave = 15;
    Insere(reg, &raiz);

    reg.Chave = 25;
    Insere(reg, &raiz);

    // Inserindo mais chaves para testar a divisão de nós
    reg.Chave = 30;
    Insere(reg, &raiz);

    reg.Chave = 35;
    Insere(reg, &raiz);

    reg.Chave = 40;
    Insere(reg, &raiz);

    reg.Chave = 45;
    Insere(reg, &raiz);

    reg.Chave = 50;
    Insere(reg, &raiz);

    
    reg.Chave = 36;
    if(Pesquisa(&reg, raiz))
    
    // Imprimindo a árvore
    cout << "Estrutura da arvore B após inserções adicionais:" << endl;
    imprimirArvore(raiz);

     /* Tempo de execução */
    cout << "\nPRÉ-PROCESSAMENTO -------" << endl;
    cout << "Número de transferências: " << nTransferencias_pre << endl;
    cout << "Comparacoes realizadas: " << nComparacoes_pre << endl;
    //cout << "Tempo de pre-processamento: " << tempoProcessamento.count() << "ms" << endl;
    /*TEMPO DE PRE-PROCESSAMENTO*/
    cout << "\n";
    cout << "PESQUISA ----------------" << endl;
    cout << "Número de transferências: " << nTransferencias_pes << endl;
    cout << "Comparacoes realizadas: " << nComparacoes_pes << endl;
    //cout << "Tempo de pesquisa: " << tempoPesquisa.count() << "ms" << endl;
    /*TEMPO DE PRE-PROCESSAMENTO*/
    
    // Liberando a memória da árvore
    freeArvore(raiz);
    return 0;
}   
