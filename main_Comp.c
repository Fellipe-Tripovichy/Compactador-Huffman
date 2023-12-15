#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "./lista.h"
#include "./tad_funcoes.h"


int main(int argc,char *argv[]){
    if(argv[1] == NULL){
        puts("\nArgumentos insuficientes: insira o caminho do arquivo que deseja compactar\n\n");
    }else{
        char nomeArqOrigem[50] = "";
        strcat(nomeArqOrigem,argv[1]);
        char *nomeArquivo = strtok(nomeArqOrigem, ".");
        strcat(nomeArquivo,".comp");

        func_criaArquivoCodificado(argv[1],nomeArquivo);
    }

    return 0;
}