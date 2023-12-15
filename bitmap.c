#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./bitmap.h"

struct map {
    unsigned int max_size;    ///< Tamanho máximo em bits
    unsigned int length;       ///< Tamanho atual em bits
    unsigned char *contents;   ///< Conteúdo do mapa de bits
};

/**
 * Função auxiliar que imprime uma mensagem de falha e aborta o programa caso testresult seja falso.
 * @param testresult Valor booleano representando o resultado do teste que deveria ser verdadeiro.
 * @param message A mensagem para ser impressa se resultado do teste for falso.
 */
void assert(int testresult, char *message) {
    if (!testresult) {
        printf("%s\n", message);
        exit(EXIT_FAILURE);
    }
}

/**
 * Retorna o conteúdo do mapa de bits.
 * @param bm O mapa de bits.
 */
unsigned char *bitmapGetContents(bitmap *bm) {
    return bm->contents;
}

/**
 * Retorna o tamanho máximo do mapa de bits.
 * @param bm O mapa de bits.
 * @return O tamanho máximo do mapa de bits.
 */
unsigned int bitmapGetMaxSize(bitmap *bm) {
    return bm->max_size;
}

/**
 * Retorna o tamanho atual do mapa de bits.
 * @param bm O mapa de bits.
 * @return O tamanho atual do mapa de bits.
 */
unsigned int bitmapGetLength(bitmap *bm) {
    return bm->length;
}

/**
 * Constrói um novo mapa de bits, definindo um tamanho máximo.
 * @param max_size O tamanho máximo para o mapa de bits.
 * @return O mapa de bits inicializado.
 */
bitmap *bitmapInit(unsigned int max_size) {
    bitmap *bm;
    bm = (bitmap *)malloc(sizeof(bitmap));
    // Definir tamanho máximo em bytes, com arredondamento para cima
    unsigned int max_sizeInBytes = (max_size + 7) / 8;
    // Alocar espaço de memória para o tamanho máximo em bytes
    bm->contents = calloc(max_sizeInBytes, sizeof(char));
    // Verificar alocação de memória
    assert(bm->contents != NULL, "Erro de alocação de memória.");
    // Definir valores iniciais para tamanho máximo e tamanho atual
    bm->max_size = max_size;
    bm->length = 0;
    return bm;
}

/**
 * Retorna o valor do bit na posição index.
 * @param bm O mapa de bits.
 * @param index A posição do bit.
 * @pre index < bitmapGetLength(bm)
 * @return O valor do bit.
 */
unsigned char bitmapGetBit(bitmap *bm, unsigned int index) // Index in bits
{
    // Verificar se index < bm.length, pois caso contrário, index é inválido
    // assert(index < bm->length, "Acesso a posição inexistente no mapa de bits.");

    // index/8 é o índice do byte que contém o bit em questão
    // 7-(index%8) é o deslocamento do bit em questão no byte
    return (bm->contents[index / 8] >> (7 - (index % 8))) & 0x01;
}

/**
 * Modifica o valor do bit na posição index.
 * @param bm O mapa de bits.
 * @param index A posição do bit.
 * @param bit O novo valor do bit.
 * @post bitmapGetBit(bm,index)==bit
 */
static void bitmapSetBit(bitmap *bm, unsigned int index, unsigned char bit) {
    // Verificar se index < bm->length, pois caso contrário, index é inválido
    assert(index < bm->length, "Acesso a posição inexistente no mapa de bits.");
    // index/8 é o índice do byte que contém o bit em questão
    // 7-(index%8) é o deslocamento do bit em questão no byte
    bit = bit & 0x01;
    bit = bit << (7 - (index % 8));
    bm->contents[index / 8] = bm->contents[index / 8] | bit;
}

/**
 * Adiciona um bit no final do mapa de bits.
 * @param bm O mapa de bits.
 * @param bit O novo valor do bit.
 * @pre bitmapGetLength(bm) < bitmapGetMaxSize(bm)
 * @post (bitmapGetBit(bm,bitmapGetLength(bm) @ pre)==bit)
 * and (bitmapGetLength(bm) == bitmapGetLength(bm) @ pre+1)
 */
void bitmapAppendLeastSignificantBit(bitmap *bm, unsigned char bit) {
    // Verificar se bm->length < bm->max_size, caso contrário, o bitmap está cheio
    assert(bm->length < bm->max_size, "Tamanho máximo excedido no mapa de bits.");
    // Como um bit será adicionado, devemos incrementar o tamanho do mapa de bits
    bm->length++;
    bitmapSetBit(bm, bm->length - 1, bit);
}

/**
 * Libera a memória dinâmica alocada para o mapa de bits.
 * @param bm O mapa de bits.
 */
void bitmapLibera(bitmap *bm) {
    free(bm->contents);
    free(bm);
}
