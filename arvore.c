#include "./arvore.h"
#include <stdlib.h>
#include <stdio.h>

struct arv {
    unsigned char letra;
    int freq;
    Arv *esq;
    Arv *dir;
};

Arv *arv_criavazia(void) {
    return NULL;
}

Arv *arv_cria(unsigned char l, int frequencia, Arv *e, Arv *d) {
    Arv *a = (Arv *)malloc(sizeof(Arv));
    a->dir = d;
    a->esq = e;
    a->letra = l;
    a->freq = frequencia;
    return a;
}

unsigned char arv_retornaLetra(Arv *a) {
    return a->letra;
}

int arv_retornaFreq(Arv *a) {
    return a->freq;
}

Arv *arv_libera(Arv *a) {
    if (!arv_vazia(a)) {
        arv_libera(a->esq);
        arv_libera(a->dir);
        free(a);
    }
    return NULL;
}

int arv_vazia(Arv *a) {
    return (a == NULL);
}

int arv_pertence(Arv *a, unsigned char l) {
    if (arv_vazia(a)) {
        return 0;
    } else {
        return (a->letra == l || arv_pertence(a->esq, l) || arv_pertence(a->dir, l));
    }
}

void arv_exibe(Arv *a) {
    printf("<");
    if (!arv_vazia(a)) {
        printf("%c:%d", a->letra, a->freq);
        arv_exibe(a->esq);
        arv_exibe(a->dir);
    }
    printf(">");
}

int arv_folhas(Arv *a) {
    if (!arv_vazia(a)) {
        int soma = arv_folhas(a->esq) + arv_folhas(a->dir);
        if (arv_vazia(a->dir) && arv_vazia(a->esq)) {
            return (soma + 1);
        }
        return soma;
    } else {
        return 0;
    }
}

int arv_altura(Arv *a) {
    if (arv_vazia(a)) {
        return 0;
    } else {
        int x = arv_altura(a->esq);
        int y = arv_altura(a->dir);
        return (x > y) ? (x + 1) : (y + 1);
    }
}

int arv_ehFolha(Arv *a) {
    return (a->dir == NULL && a->esq == NULL);
}

Arv *arv_retornaNoDireita(Arv *a) {
    return a->dir;
}

Arv *arv_retornaNoEsquerdo(Arv *a) {
    return a->esq;
}
