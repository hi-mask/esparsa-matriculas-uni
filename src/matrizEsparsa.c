#include "../libraries/matrizEsparsa.h"
#include <stdio.h>
#include <stdlib.h>

/* ============================================================
   FUNCOES DA MATRIZ
   ============================================================ */

/* Cria uma matriz esparsa vazia (nenhum no na lista ainda) */
MatrizEsparsa *criar(int linhas, int colunas) {
    MatrizEsparsa *m = malloc(sizeof(MatrizEsparsa));

    if (m == NULL) {
        fprintf(stderr, "Erro: sem memoria.\n");
        exit(1);
    }

    m->linhas  = linhas;
    m->colunas = colunas;
    m->total   = 0;
    m->inicio  = NULL;  /* lista comeca vazia */
    return m;
}

/* Confere se (linha, coluna) cabe na matriz */
int posicao_valida(MatrizEsparsa *m, int linha, int coluna) {
    if (linha < 0 || linha >= m->linhas ||
        coluna < 0 || coluna >= m->colunas) {
        printf("  [ERRO] Posicao (%d,%d) fora dos limites da matriz %dx%d.\n",
               linha, coluna, m->linhas, m->colunas);
        return 0;
    }
    return 1;
}

/* Procura (linha, coluna). Devolve o PONTEIRO para o no, ou NULL se nao existe.
   Custo: O(k), percorre a lista ate achar. */
No *buscar(MatrizEsparsa *m, int linha, int coluna) {
    No *atual = m->inicio;

    while (atual != NULL) {
        if (atual->linha == linha && atual->coluna == coluna) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

/* Inclui um elemento nao nulo. Se a posicao ja existe, so atualiza. */
void incluir(MatrizEsparsa *m, int linha, int coluna, int valor) {
    No *achado = NULL;
    No *novo = NULL;

    if (!posicao_valida(m, linha, coluna)) {
        return;
    }

    /* Zero nao e guardado: e justamente o que a matriz esparsa economiza */
    if (valor == 0) {
        printf("  [AVISO] Valor 0 ignorado: zero nao ocupa espaco na estrutura.\n");
        return;
    }

    /* Posicao repetida? Entao so atualiza o valor */
    achado = buscar(m, linha, coluna);
    if (achado != NULL) {
        printf("  [INFO] Posicao (%d,%d) ja existia com valor %d: atualizada para %d.\n",
               linha, coluna, achado->valor, valor);
        achado->valor = valor;
        return;
    }

    /* Cada no e um malloc INDEPENDENTE: nao existe vetor para encher,
       nem realloc para copiar tudo. */
    novo = malloc(sizeof(No));
    if (novo == NULL) {
        printf("  [ERRO] Sem memoria para criar o no.\n");
        return;
    }

    novo->linha  = linha;
    novo->coluna = coluna;
    novo->valor  = valor;

    /* Liga o novo no NA FRENTE da lista: O(1), so mexe em dois ponteiros */
    novo->prox = m->inicio;
    m->inicio  = novo;
    m->total++;

    printf("  [OK] (%d,%d,%d) incluido no inicio da lista. Total: %d no(s).\n",
           linha, coluna, valor, m->total);
}

/* Remove o elemento de (linha, coluna), se existir. */
void remover(MatrizEsparsa *m, int linha, int coluna) {
    No *anterior = NULL;
    No *atual = m->inicio;

    if (!posicao_valida(m, linha, coluna)) {
        return;
    }

    /* Anda pela lista guardando tambem o no ANTERIOR: sem ele nao
       da para "pular" o no removido. */
    while (atual != NULL &&
           !(atual->linha == linha && atual->coluna == coluna)) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("  [AVISO] Posicao (%d,%d) nao tem elemento guardado.\n", linha, coluna);
        return;
    }

    /* Refaz a ligacao: o anterior passa a apontar para o proximo.
       Nenhum outro elemento sai do lugar. */
    if (anterior == NULL) {
        m->inicio = atual->prox;       /* removendo o primeiro no */
    } else {
        anterior->prox = atual->prox;  /* removendo do meio ou do fim */
    }
    free(atual);
    m->total--;

    printf("  [OK] (%d,%d) removido; so o ponteiro prox foi refeito. Total: %d.\n",
           linha, coluna, m->total);
}

/* Mostra o valor de (linha, coluna). O que nao esta guardado vale zero. */
void consultar(MatrizEsparsa *m, int linha, int coluna) {
    No *achado = NULL;

    if (!posicao_valida(m, linha, coluna)) {
        return;
    }

    achado = buscar(m, linha, coluna);
    if (achado != NULL) {
        printf("  Consulta (%d,%d) = %d\n", linha, coluna, achado->valor);
    } else {
        printf("  Consulta (%d,%d) = 0 (nao esta guardado: zero implicito)\n",
               linha, coluna);
    }
}

/* Mostra como os nos estao ligados, com ENDERECOS.
   Repare: cada no tem um endereco proprio e o prox guarda o endereco do
   seguinte. Nada obriga os nos a ficarem lado a lado na memoria. */
void imprimir_estrutura(MatrizEsparsa *m) {
    No *atual = m->inicio;
    int pos = 0;

    printf("\n  Lista encadeada: %d no(s)\n", m->total);
    printf("  sizeof(No) = %lu bytes\n\n", (unsigned long)sizeof(No));
    printf("  pos  linha  coluna  valor  endereco        prox\n");
    printf("  ---  -----  ------  -----  --------------  --------------\n");

    while (atual != NULL) {
        printf("  %3d  %5d  %6d  %5d  %p  ", pos,
               atual->linha, atual->coluna, atual->valor, (void *)atual);
        if (atual->prox != NULL) {
            printf("%p\n", (void *)atual->prox);
        } else {
            printf("NULL\n");
        }
        atual = atual->prox;
        pos++;
    }

    /* Desenho da cadeia, do inicio ate NULL */
    printf("\n  inicio");
    atual = m->inicio;
    while (atual != NULL) {
        printf(" -> (%d,%d,%d)", atual->linha, atual->coluna, atual->valor);
        atual = atual->prox;
    }
    printf(" -> NULL\n\n");
}

/* Mostra a matriz inteira, com os zeros. So para matrizes pequenas! */
void imprimir_matriz(MatrizEsparsa *m) {
    int i = 0;
    int j = 0;
    No *achado = NULL;

    if (m->linhas > 10 || m->colunas > 10) {
        printf("  [AVISO] Matriz grande demais para imprimir inteira (maximo 10x10).\n");
        printf("          E esse o ponto da estrutura esparsa: nao precisamos dos zeros.\n");
        return;
    }

    printf("\n  Matriz %dx%d:\n\n", m->linhas, m->colunas);
    for (i = 0; i < m->linhas; i++) {
        printf("  ");
        for (j = 0; j < m->colunas; j++) {
            achado = buscar(m, i, j);
            if (achado != NULL) {
                printf("%5d", achado->valor);
            } else {
                printf("%5d", 0);
            }
        }
        printf("\n");
    }
    printf("\n");
}

/* Carrega a matriz de exemplo dos slides: (0,2,5) (1,3,8) (3,1,3) */
void carregar_exemplo(MatrizEsparsa *m) {
    if (m->linhas < 4 || m->colunas < 4) {
        printf("  [AVISO] O exemplo precisa de uma matriz de pelo menos 4x4.\n");
        return;
    }
    incluir(m, 0, 2, 5);
    incluir(m, 1, 3, 8);
    incluir(m, 3, 1, 3);
}

/* Libera toda a memoria: um free por no, depois a struct */
void destruir(MatrizEsparsa *m) {
    No *atual = m->inicio;
    No *proximo = NULL;

    while (atual != NULL) {
        proximo = atual->prox;  /* guarda o proximo ANTES de liberar */
        free(atual);
        atual = proximo;
    }
    free(m);
}

