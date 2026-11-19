#include <stdio.h>
#include "../libraries/matrizEsparsa.h"
int main(void){

    MatrizEsparsa *m = NULL;
    int alunos[15]      = {31, 4, 47, 1, 12, 0, 23, 5, 38, 1, 17, 4, 42, 9, 5};
    int disciplinas[15] = {15, 2, 30, 7,  3, 3,  7, 5,  2, 0, 20, 4, 11, 8, 1};
    int i = 0;
    int resultado = 0;

    printf("==============================================================\n");
    printf("  MATRICULAS: matriz esparsa aluno x disciplina (Entrega 2)\n");
    printf("  linha = aluno | coluna = disciplina | valor 1 = matriculado\n");
    printf("==============================================================\n\n");

    m = criar(50, 50);

    /* 1 - insercao fora de ordem */
    printf("1) Matriculando 15 alunos FORA de ordem (aluno,disciplina):\n   ");
    for (i = 0; i < 15; i++) {
        inserir(m, alunos[i], disciplinas[i], 1);
        printf("(%d,%d) ", alunos[i], disciplinas[i]);
    }
    printf("\n\n");

    /* 2 - a lista sai ordenada */
    printf("2) A lista encadeada sai ORDENADA por (linha, coluna):\n");
    imprimir_estrutura(m);
    printf("\n");

    /* 3 - buscar */
    printf("3) Consultas com buscar(i, j):\n");
    printf("   buscar(1,7) = %d      buscar(2,2) = %d\n", buscar(m, 1, 7), buscar(m, 2, 2));
    printf("   (o aluno 1 esta matriculado na disciplina 7; o aluno 2 nao esta na 2)\n\n");

    /* 4 - casos de inserir() */
    printf("4) Casos especiais de inserir():\n");
    resultado = inserir(m, 1, 7, 2);
    if (resultado == 2) {
        printf("   inserir(1,7,2): posicao repetida, valor ATUALIZADO. buscar(1,7) = %d\n",
               buscar(m, 1, 7));
    }
    resultado = inserir(m, 20, 20, 0);
    if (resultado == 0) {
        printf("   inserir(20,20,0): valor zero NAO insere. buscar(20,20) = %d\n",
               buscar(m, 20, 20));
    }
    printf("   Total de nos continua em %d.\n\n", m->total);

    /* 5 - recorte da matriz */
    printf("5) Recorte da matriz, com os zeros:");
    imprimirRecorte(m, 0, 5, 0, 5);

    /* 6 - ocupacao */
    printf("6) ");
    imprimirOcupacao(m);

    destruir(m);
    return 0;

}