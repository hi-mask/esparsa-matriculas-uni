#ifndef MATRIZESPARSA_H
#define MATRIZESPARSA_H

/* ============================================================
   ESTRUTURAS
   ============================================================ */

/* usada so no relatorio de memoria (versao em triplas) */
typedef struct {
    int linha;
    int coluna;
    int valor;
} Tripla;

/* no da lista encadeada, mantida ordenada por (linha, coluna) */
typedef struct No {
    int linha;
    int coluna;
    int valor;
    struct No *prox;    /* o ponteiro extra: liga ao proximo no */
} No;

/* a matriz esparsa: a lista mais as dimensoes e a contagem de nao-nulos */
typedef struct {
    int linhas;         /* dimensoes da matriz */
    int colunas;
    int total;          /* quantos nao nulos existem */
    No *inicio;         /* primeiro no da lista (NULL = lista vazia) */
} MatrizEsparsa;


/* ============================================================
   FUNCOES DA MATRIZ
   ============================================================ */

/* Cria uma matriz esparsa vazia (nenhum no na lista ainda) */
MatrizEsparsa *criar(int linhas, int colunas);

/* Confere se (linha, coluna) cabe na matriz */
int posicao_valida(MatrizEsparsa *m, int linha, int coluna);

/* Procura (linha, coluna). Devolve o PONTEIRO para o no, ou NULL se nao existe.
   Custo: O(k), percorre a lista ate achar. */
No *buscar(MatrizEsparsa *m, int linha, int coluna);

/* Inclui um elemento nao nulo. Se a posicao ja existe, so atualiza. */
void incluir(MatrizEsparsa *m, int linha, int coluna, int valor);

/* Remove o elemento de (linha, coluna), se existir. */
void remover(MatrizEsparsa *m, int linha, int coluna);

/* Mostra o valor de (linha, coluna). O que nao esta guardado vale zero. */
void consultar(MatrizEsparsa *m, int linha, int coluna);

/* Mostra como os nos estao ligados, com ENDERECOS.
   Repare: cada no tem um endereco proprio e o prox guarda o endereco do
   seguinte. Nada obriga os nos a ficarem lado a lado na memoria. */
void imprimir_estrutura(MatrizEsparsa *m);
/* Mostra a matriz inteira, com os zeros. So para matrizes pequenas! */
void imprimir_matriz(MatrizEsparsa *m);

/* Carrega a matriz de exemplo dos slides: (0,2,5) (1,3,8) (3,1,3) */
void carregar_exemplo(MatrizEsparsa *m);

/* Libera toda a memoria: um free por no, depois a struct */
void destruir(MatrizEsparsa *m);

#endif