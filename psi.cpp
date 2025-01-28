#include "psi.h"

using namespace std;
using namespace chrono;

// Função de busca binária
bool buscaBinaria(TipoRegistro pagina[], int qtdItens, int chave, int pEsq, int pDir, int pMeio, TipoRegistro *registro, int *cont) {
    (*cont)++;
    if (pEsq > pDir)
        return false;

    (*cont)++;
    if (chave > pagina[pMeio].chave) {
        pEsq = pMeio + 1;
        (*cont)++;
    } else if (chave < pagina[pMeio].chave)
        pDir = pMeio - 1;
    else if (chave == pagina[pMeio].chave) {
        *registro = pagina[pMeio];
        return true;
    }

    pMeio = (pDir + pEsq) / 2;

    return buscaBinaria(pagina, qtdItens, chave, pEsq, pDir, pMeio, registro, cont);
}

// Função de pesquisa sequencial
bool pesquisa(int menorChaveDaPagina[], int tam, TipoRegistro *registroBuscado, FILE *arq, int *cont, int *nTransferencias_pes) {
    int qtdItens, desloc, numeroDaPagina = 0;
    TipoRegistro pagina[REGISTROSPAGINA];

    while (numeroDaPagina < tam && menorChaveDaPagina[numeroDaPagina] <= registroBuscado->chave)
        numeroDaPagina++; // busca pela pagina onde o item pode estar

    if (numeroDaPagina == 0)
        return false; // item nao existe no arquivo
    else {
        qtdItens = REGISTROSPAGINA;
        if (numeroDaPagina == tam) {
            fseek(arq, 0, SEEK_END);
            qtdItens = (ftell(arq) / sizeof(TipoRegistro)) % REGISTROSPAGINA;

            if (qtdItens == 0)
                qtdItens = REGISTROSPAGINA;
        }
    }

    desloc = (numeroDaPagina - 1) * REGISTROSPAGINA * sizeof(TipoRegistro);
    fseek(arq, desloc, SEEK_SET);
    (*nTransferencias_pes)++;
    fread(&pagina, sizeof(TipoRegistro), qtdItens, arq);

    int pEsq = 0, pDir = qtdItens - 1, pMeio = pDir / 2;
    return buscaBinaria(pagina, qtdItens, registroBuscado->chave, pEsq, pDir, pMeio, registroBuscado, cont);
}

int psi(string arq_path, int chave) {
    FILE *arq;
    TipoRegistro itensAux[REGISTROSPAGINA], registroBuscado;
    int cont = 0;
    auto duracao_pre = 0;
    int nTransferencias_pre = 0, nTransferencias_pes = 0;



    if ((arq = fopen(arq_path.c_str(), "rb")) == NULL) {
        cout << "Erro ao abrir o arquivo" << endl;
        return 1;
    }

    int pos = 0;
    auto tempoInicioProc = high_resolution_clock::now();
    int menorChaveDaPagina[QTDPAGINAS];

    fseek(arq, 0, SEEK_END);
    long tamanhoArquivo = ftell(arq);
    long tamanhoRegistro = sizeof(TipoRegistro) * REGISTROSPAGINA;
    long numPaginas = tamanhoArquivo / tamanhoRegistro;

    if (string(arq_path) == "registrosDecrescente.bin") {
        for (long i = numPaginas - 1; i >= 0; i--) {
            fseek(arq, i * tamanhoRegistro, SEEK_SET);
            fread(itensAux, sizeof(TipoRegistro), REGISTROSPAGINA, arq);
            menorChaveDaPagina[pos++] = itensAux[REGISTROSPAGINA - 1].chave;
            cont++;
        }
    } else {
        fseek(arq, 0, SEEK_SET);
        while (fread(itensAux, sizeof(TipoRegistro), REGISTROSPAGINA, arq)) {
            menorChaveDaPagina[pos++] = itensAux[0].chave;
            cont++;
        }
    }

    auto tempoFimProc = high_resolution_clock::now();
    nTransferencias_pre = cont;

    registroBuscado.chave = chave;

    auto tempoInicioPes = high_resolution_clock::now();

    if (pesquisa(menorChaveDaPagina, pos, &registroBuscado, arq, &cont, &nTransferencias_pes))
        cout << "Registro localizado: " << registroBuscado.chave << endl;
    else
        cout << "Registro não localizado" << endl;

    auto tempoFimPes = high_resolution_clock::now();
    fclose(arq);

    auto tempoProcessamento = duration_cast<milliseconds>(tempoFimProc - tempoInicioProc);
    auto tempoPesquisa = duration_cast<milliseconds>(tempoFimPes - tempoInicioPes);

    cout << "\nPRÉ-PROCESSAMENTO -------" << endl;
    cout << "Número de transferências: " << nTransferencias_pre << endl;
    cout << "Comparacoes realizadas: " << cont << endl;
    cout << "Tempo de pre-processamento: " << tempoProcessamento.count() << "ms" << endl;

    cout << "\nPESQUISA ----------------" << endl;
    cout << "Número de transferências: " << nTransferencias_pes << endl;
    cout << "Comparacoes realizadas: " << cont << endl;
    cout << "Tempo de pesquisa: " << tempoPesquisa.count() << "ms" << endl;

    return 0;
}

// int main () {
//     string arq_path;
//     cout << "Forneça o caminho do arquivo: ";
//     cin >> arq_path;

//     int chave;
//     cout << "Forneça a chave de busca: ";
//     cin >> chave;

//     psi(arq_path, chave);

//     return 0;
// }