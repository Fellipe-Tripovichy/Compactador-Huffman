#include "./tad_funcoes.h"
#include "./arvore.h"
#include "./bitmap.h"
#include <string.h>
#include <inttypes.h>

#define TAM 128
#define TAM_2 256
#define UM_MEGA 1048576

//************************************************
// Função que verifica se o ponteiro para arquivo é diferente de NULL
static void verificaArq(char *nome, FILE *arq) {
    if (!arq) {
        printf("\nErro 001: Ao Abrir Arquivo %s\n", nome);
        exit(1);
    }
}

// **************LISTA DE FREQUENCIA**************
void func_montarListaDeFreq(int vet[], char *nomearq) {
    memset(vet, 0, sizeof(int) * TAM_2);

    FILE *arq = fopen(nomearq, "r");
    verificaArq(nomearq, arq);

    int l;
    while ((l = fgetc(arq)) != EOF) {
        if (l < TAM_2) {
            vet[l]++;
        }
    }
    fclose(arq);
}

void func_exibeListaDeFreq(int vet[]) {
    printf("\n\nLista de frequencia\n");

    for (int i = 0; i < TAM_2; i++) {
        if (vet[i] != 0)
            printf("%d: %d\n", i, vet[i]);
    }

    printf("\n");
}

// **************MONTANDO ARVORE**************
void func_montaListaDeArv(Lista *l, int vet[]) {
    for (int i = 0; i < TAM_2; i++) {
        if (vet[i] != 0) {
            Arv *a1 = arv_cria(i, vet[i], NULL, NULL);
            lista_insereNoFinal(a1, l);
        }
    }
}

Arv *func_montaArvore(Lista *l) {
    while (!lista_temSoUmaCel(l)) {
        lista_ordena(l);
        Arv *a1 = lista_retiraPrimeiro(l);
        Arv *a2 = lista_retiraPrimeiro(l);
        Arv *raiz = arv_cria('+', (arv_retornaFreq(a1) + arv_retornaFreq(a2)), a1, a2);
        lista_insereNoFinal(raiz, l);
    }
    Arv *retorno = lista_retornaPrimArv(l);
    lista_libera(l);
    return retorno;
}

// **************CRIANDO DICIONARIO**************
char **func_alocaDicionario(int coluna) {
    char **c = malloc(sizeof(char *) * TAM_2);
    for (int i = 0; i < TAM_2; i++) {
        c[i] = calloc(coluna, sizeof(char));
    }
    return c;
}

void func_montaDicionario(char **dic, Arv *a, char *caminho, int coluna) {
    if (arv_ehFolha(a)) {
        strcpy(dic[arv_retornaLetra(a)], caminho);
    } else {
        char camEsq[coluna];
        char camDir[coluna];
        strcpy(camEsq, caminho);
        strcpy(camDir, caminho);
        strcat(camDir, "1");
        strcat(camEsq, "0");
        func_montaDicionario(dic, arv_retornaNoDireita(a), camDir, coluna);
        func_montaDicionario(dic, arv_retornaNoEsquerdo(a), camEsq, coluna);
    }
}

void func_ExibeDicionario(char **d) {
    printf("\n\nDicionario:\n");
    for (int i = 0; i < TAM_2; i++) {
        if (strlen(d[i]) > 0)
            printf("letra:%c -> caminho:%s\n", i, d[i]);
    }
    printf("\n");
}

void func_liberaDicionario(char **d) {
    for (int i = 0; i < TAM_2; i++) {
        free(d[i]);
    }
    free(d);
}

// **************CRIANDO CODIFICADOR**************
void func_escreveCodificado(char **dic, char *arqOrig, FILE *arqDest, int tamanho, long int numBit) {
    FILE *arq = fopen(arqOrig, "r");
    verificaArq(arqOrig, arq);
    rewind(arq);

    bitmap *bm = bitmapInit(numBit);

    int l;
    while ((l = fgetc(arq)) != EOF) {
        if (l < TAM_2) {
            int cont = strlen(dic[l]);
            for (int i = 0; i < cont; i++) {
                bitmapAppendLeastSignificantBit(bm, dic[l][i]);
            }
        }
    }

    fwrite(bitmapGetContents(bm), sizeof(unsigned char), ((numBit + 7) / 8), arqDest);
    bitmapLibera(bm);
    fclose(arq);
    return;
}

void func_escreveDecodificado(char *arqDest, FILE *arqOrig, Arv *a, long int numBit) {
    FILE *arq = fopen(arqDest, "w");
    verificaArq(arqDest, arq);

    Arv *aux = a;
    bitmap *bm2 = bitmapInit(numBit + 1);

    fread(bitmapGetContents(bm2), sizeof(unsigned char), (numBit + 7) / 8, arqOrig);

    for (int i = 0; i < numBit; i++) {
        uint32_t teste;
        teste = bitmapGetBit(bm2, i);

        if ((0x01 & teste) != 0) {
            aux = arv_retornaNoDireita(aux);
        } else if ((0x01 & teste) == 0) {
            aux = arv_retornaNoEsquerdo(aux);
        }

        if (arv_ehFolha(aux)) {
            char letra = arv_retornaLetra(aux);
            fprintf(arq, "%c", letra);
            aux = a;
        }
    }

    bitmapLibera(bm2);
    fclose(arq);
    return;
}

// **************MANIPULANDO ARQUIVO**************
void func_criaArquivoCodificado(char *arqOrig, char *arqDest) {
    int vet[TAM_2];
    func_montarListaDeFreq(vet, arqOrig);

    Lista *l = lista_cria();
    func_montaListaDeArv(l, vet);

    Arv *a = func_montaArvore(l);

    char **dicionario = func_alocaDicionario(arv_altura(a) + 1);
    func_montaDicionario(dicionario, a, "", arv_altura(a) + 1);

    long int numBit = 0;
    for (int i = 0; i < TAM_2; i++) {
        numBit = numBit + (strlen(dicionario[i]) * vet[i]);
    }

    FILE *arqEsc = fopen(arqDest, "wb");
    verificaArq(arqDest, arqEsc);

    fwrite(vet, sizeof(int), TAM_2, arqEsc);

    func_escreveCodificado(dicionario, arqOrig, arqEsc, arv_altura(a), numBit);

    func_liberaDicionario(dicionario);
    arv_libera(a);
    fclose(arqEsc);
}

void func_criaArquivoDecodificado(char *arqOrig, char *arqDest) {
    FILE *arqLeitura = fopen(arqOrig, "rb");
    verificaArq(arqOrig, arqLeitura);

    int vet[TAM_2];
    fread(vet, sizeof(int), TAM_2, arqLeitura);

    Lista *l = lista_cria();
    func_montaListaDeArv(l, vet);

    Arv *a = func_montaArvore(l);

    char **dicionario = func_alocaDicionario(arv_altura(a) + 1);
    func_montaDicionario(dicionario, a, "", arv_altura(a) + 1);

    long int numBit = 0;
    for (int i = 0; i < TAM_2; i++) {
        numBit = numBit + (strlen(dicionario[i]) * vet[i]);
    }

    func_escreveDecodificado(arqDest, arqLeitura, a, numBit);

    func_liberaDicionario(dicionario);
    arv_libera(a);
    fclose(arqLeitura);
    return;
}
