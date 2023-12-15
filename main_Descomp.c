#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "./lista.h"
#include "./tad_funcoes.h"

int main(int argc,char *argv[]){
    if(argv[1] == NULL){
        puts("\nArgumentos insuficientes: insira o caminho do arquivo que deseja descompactar\n\n");
    }else{
        char nomeArquivo[50] = "";
        for(int i=0; i<strlen(argv[1])-5;i++){
            nomeArquivo[i] = argv[1][i];
        }

        func_criaArquivoDecodificado(argv[1],nomeArquivo);
    }

    return 0;
}
