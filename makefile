PROJ_NAME_COMP = compactador
PROJ_NAME_DESCOMP = decompactador
CC_FLAGS=-c -w -Wall

$(PROJ_NAME_COMP): main_Comp.o  main_Descomp.o lista.o arvore.o tad_funcoes.o bitmap.o
	gcc -o $(PROJ_NAME_DESCOMP) main_Descomp.o lista.o arvore.o tad_funcoes.o bitmap.o
	gcc -o $(PROJ_NAME_COMP) main_Comp.o lista.o arvore.o tad_funcoes.o bitmap.o

main_Descomp.o: main_Descomp.c
	gcc -o main_Descomp.o main_Descomp.c $(CC_FLAGS)

main_Comp.o: main_Comp.c
	gcc -o main_Comp.o main_Comp.c $(CC_FLAGS)

lista.o: lista.c
	gcc -o lista.o lista.c $(CC_FLAGS)

arvore.o: arvore.c
	gcc -o arvore.o arvore.c $(CC_FLAGS)

tad_funcoes.o: tad_funcoes.c
	gcc -o tad_funcoes.o tad_funcoes.c $(CC_FLAGS)

bitmap.o: bitmap.c
	gcc -o bitmap.o bitmap.c $(CC_FLAGS)




clean:
	rm -rf *.o
	rm -rf $(PROJ_NAME_COMP)
	rm -rf $(PROJ_NAME_DESCOMP)

rmpromper: clean
	rm -rf $(PROJ_NAME)

run:
	valgrind ./$(PROJ_NAME)