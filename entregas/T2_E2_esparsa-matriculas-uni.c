#include <stdio.h>
#include <stdlib.h>

/* Estimativas da Entrega 1 (5.000 x 500, k = 25.000, 24 bytes por no) */
#define E1_DENSA   10000000UL
#define E1_TRIPLAS   300000UL
#define E1_LISTA     600000UL
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

/*=========================================================
    Função: comparar
    Autor: Marcos Paulo da Silva Oliveira

    Descrição:
        Compara duas posições na ordem da lista: primeiro por linha, depois por coluna.
        Devolve -1 se (l1,c1) vem antes de (l2,c2), 0 se são a mesma posição, 1 se vem depois.
    Parâmetros:
        1 - l1 : índice da linha do primeiro elemento
        2 - c1 : índice da coluna do primeiro elemento
        3 - l2 : índice da linha do segundo elemento
        4 - c2 : índice da coluna do segundo elemento
    Retorno:
        int : resultado da comparação.
  =========================================================*/
static int comparar(int l1, int c1, int l2, int c2) {
    if (l1 < l2) {
        return -1;
    }
    if (l1 > l2) {
        return 1;
    }
    if (c1 < c2) {
        return -1;
    }
    if (c1 > c2) {
        return 1;
    }
    return 0;
}

/*=========================================================
    Função: anexar
    Autor: Marcos Paulo da Silva Oliveira

    Descrição:
        Cria um novo nó e o anexa ao final da lista encadeada.
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
        2 - ultimo : ponteiro para o último nó da lista (NULL se lista vazia)
        3 - linha : índice da linha do novo elemento
        4 - coluna : índice da coluna do novo elemento
        5 - valor : valor a ser armazenado
    Retorno:
        No* : ponteiro para o novo nó criado.
  =========================================================*/
static No *anexar(MatrizEsparsa *m, No *ultimo, int linha, int coluna, int valor) {
    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        fprintf(stderr, "Erro: sem memoria.\n");
        exit(1);
    }

    novo->linha  = linha;
    novo->coluna = coluna;
    novo->valor  = valor;
    novo->prox   = NULL;

    if (ultimo == NULL) {
        m->inicio = novo;
    } else {
        ultimo->prox = novo;
    }
    m->total++;
    return novo;
}

/*=========================================================
    Função: criar
    Autor: Rovilson Mezencio

    Descrição:
        Cria uma matriz esparsa vazia (nenhum no na lista ainda).
    Parâmetros:
        1 - linhas : número de linhas da matriz
        2 - colunas : número de colunas da matriz
    Retorno:
        MatrizEsparsa* : ponteiro para a matriz criada ou NULL em caso de erro.
  =========================================================*/
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

/*=========================================================
    Função: posicao_valida
    Autor: Rovilson Mezencio

    Descrição:
        Verifica se determinada posição (linha, coluna)
        está dentro dos limites da matriz.
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
        2 - linha : índice da linha a ser verificada
        3 - coluna : índice da coluna a ser verificada
    Retorno:
        int : 1 se a posição for válida, 0 caso contrário.
  =========================================================*/
int posicao_valida(MatrizEsparsa *m, int linha, int coluna) {
    if (linha < 0 || linha >= m->linhas ||
        coluna < 0 || coluna >= m->colunas) {
        printf("  [ERRO] Posicao (%d,%d) fora dos limites da matriz %dx%d.\n",
               linha, coluna, m->linhas, m->colunas);
        return 0;
    }
    return 1;
}

/*=========================================================
    Função: buscarNo
    Autor: Marcos Paulo da Silva Oliveira

    Descrição:
        Procura um nó correspondente à posição informada.
        Como a lista é ordenada, interrompe a busca quando
        ultrapassa a posição desejada.
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
        2 - linha : índice da linha procurada
        3 - coluna : índice da coluna procurada
    Retorno:
        No* : ponteiro para o nó encontrado ou NULL caso
        não exista um elemento armazenado nessa posição.
  =========================================================*/
No *buscarNo(MatrizEsparsa *m, int linha, int coluna) {
    No *atual = m->inicio;
    int comparador = 0;
    while (atual != NULL) {
        comparador = comparar(atual->linha, atual->coluna, linha, coluna);
        if (comparador == 0) {
            return atual;
        }
        if (comparador > 0) {
            return NULL;  /* ja passou: se existisse, estaria antes */
        }
        atual = atual->prox;
    }
    return NULL;
}

/*=========================================================
    Função: buscar
    Autor: Marcos Paulo da Silva Oliveira

    Descrição:
        Consulta o valor armazenado em determinada posição
        da matriz. Caso não exista um nó nessa posição,
        considera que o valor é zero.
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
        2 - linha : índice da linha consultada
        3 - coluna : índice da coluna consultada
    Retorno:
        int : valor armazenado na posição ou zero caso
        não exista um elemento armazenado.
  =========================================================*/
int buscar(MatrizEsparsa *m, int linha, int coluna) {
    No *achado = buscarNo(m, linha, coluna);

    if (achado == NULL) {
        return 0;
    }
    return achado->valor;
}

/*=========================================================
    Função: inserir
    Autor: Marcos Paulo da Silva Oliveira

    Descrição:
        Insere um elemento na matriz esparsa, mantendo a
        lista encadeada ordenada por linha e coluna.
        Caso a posição já exista, atualiza seu valor.
        Valores iguais a zero não são armazenados.
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
        2 - linha : índice da linha de inserção
        3 - coluna : índice da coluna de inserção
        4 - valor : valor a ser armazenado
    Retorno:
        int : 0 se nada for feito, 1 se inserir um novo nó
        ou 2 se atualizar um elemento existente.
  =========================================================*/
int inserir(MatrizEsparsa *m, int linha, int coluna, int valor) {
    No *anterior = NULL;
    No *atual = m->inicio;
    No *novo = NULL;

    if (!posicao_valida(m, linha, coluna)) {
        return 0;
    }

    /* Zero nao e guardado: e justamente o que a matriz esparsa economiza */
    if (valor == 0) {
        return 0;
    }

    /* Anda ate o primeiro no que NAO vem antes de (linha, coluna),
       guardando o anterior para poder ligar o novo no no meio da lista */
    while (atual != NULL &&
           comparar(atual->linha, atual->coluna, linha, coluna) < 0) {
        anterior = atual;
        atual = atual->prox;
    }

    /* Posicao repetida? Entao so atualiza o valor */
    if (atual != NULL &&
        comparar(atual->linha, atual->coluna, linha, coluna) == 0) {
        atual->valor = valor;
        return 2;
    }

    novo = malloc(sizeof(No));
    if (novo == NULL) {
        printf("  [ERRO] Sem memoria para criar o no.\n");
        return 0;
    }

    novo->linha  = linha;
    novo->coluna = coluna;
    novo->valor  = valor;

    /* Liga o novo no entre o anterior e o atual */
    novo->prox = atual;
    if (anterior == NULL) {
        m->inicio = novo;       /* entra antes de todos */
    } else {
        anterior->prox = novo;  /* entra no meio ou no fim */
    }
    m->total++;
    return 1;
}

/*=========================================================
    Função: imprimirRecorte
    Autor: Marcos Paulo da Silva Oliveira

    Descrição:
        Exibe uma parte da matriz em formato de tabela,
        incluindo os valores zero das posições não armazenadas.
        Ajusta os limites do recorte para caber na matriz.
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
        2 - linhaInicio : primeira linha do recorte
        3 - linhaFim : última linha do recorte
        4 - colunaInicio : primeira coluna do recorte
        5 - colunaFim : última coluna do recorte
    Retorno:
        void.
  =========================================================*/
void imprimirRecorte(MatrizEsparsa *m, int linhaInicio, int linhaFim, int colunaInicio, int colunaFim) {
    int i = 0;
    int j = 0;

    /* Ajusta o recorte para caber na matriz */
    if (linhaInicio < 0) {
        linhaInicio = 0;
    }
    if (colunaInicio < 0) {
        colunaInicio = 0;
    }
    if (linhaFim >= m->linhas) {
        linhaFim = m->linhas - 1;
    }
    if (colunaFim >= m->colunas) {
        colunaFim = m->colunas - 1;
    }

    if (linhaInicio > linhaFim || colunaInicio > colunaFim) {
        printf("  [AVISO] Recorte vazio.\n");
        return;
    }
    if (colunaFim - colunaInicio + 1 > 15) {
        printf("  [AVISO] Recorte largo demais (maximo 15 colunas).\n");
        return;
    }

    printf("\n  Recorte: linhas %d a %d, colunas %d a %d\n\n",
           linhaInicio, linhaFim, colunaInicio, colunaFim);

    printf("         ");
    for (j = colunaInicio; j <= colunaFim; j++) {
        printf("%5d", j);
    }
    printf("\n");

    for (i = linhaInicio; i <= linhaFim; i++) {
        printf("  %5d |", i);
        for (j = colunaInicio; j <= colunaFim; j++) {
            printf("%5d", buscar(m, i, j));
        }
        printf("\n");
    }
    printf("\n");
}

/*=========================================================
    Função: imprimirOcupacao
    Autor: Marcos Paulo da Silva Oliveira

    Descrição:
        Calcula e exibe a taxa de ocupação da matriz esparsa,
        considerando a proporção entre elementos não nulos
        armazenados e o total de células da matriz.
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
    Retorno:
        void.
  =========================================================*/
void imprimirOcupacao(MatrizEsparsa *m) {
    long celulas = (long)m->linhas * m->colunas;
    double porcento = 0.0;

    if (celulas > 0) {
        porcento = 100.0 * m->total / celulas;
    }
    printf("Ocupacao: %d de %ld celulas = %.2f%%\n", m->total, celulas, porcento);
}

/*=========================================================
    Função: remover
    Autor: Rovilson Mezencio

    Descrição:
        Remove o elemento de (linha, coluna), se existir.
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
        2 - linha : índice da linha a ser consultada
        3 - coluna : índice da coluna a ser consultada
    Retorno:
        void.
  =========================================================*/
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

/*=========================================================
    Função: consultar
    Autor: Rovilson Mezencio

    Descrição:
        Mostra o valor de (linha, coluna). O que nao esta guardado vale zero.
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
        2 - linha : índice da linha a ser consultada
        3 - coluna : índice da coluna a ser consultada
    Retorno:
        void.
  =========================================================*/
void consultar(MatrizEsparsa *m, int linha, int coluna) {
    No *achado = NULL;

    if (!posicao_valida(m, linha, coluna)) {
        return;
    }

    achado = buscarNo(m, linha, coluna);
    if (achado != NULL) {
        printf("  Consulta (%d,%d) = %d\n", linha, coluna, achado->valor);
    } else {
        printf("  Consulta (%d,%d) = 0 (nao esta guardado: zero implicito)\n",
               linha, coluna);
    }
}

/*=========================================================
    Função: imprimir_estrutura
    Autor: Rovilson Mezencio

    Descrição:
        Imprime a estrutura da matriz esparsa, mostrando como os nós estão ligados.
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
    Retorno:
        void.
  =========================================================*/
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

/*=========================================================
    Função: imprimir_matriz
    Autor: Rovilson Mezencio

    Descrição:
        Imprime a matriz inteira, com os zeros. So para matrizes pequenas!
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
    Retorno:
        void.
  =========================================================*/
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
            achado = buscarNo(m, i, j);
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

/*=========================================================
    Função: destruir
    Autor: Rovilson Mezencio

    Descrição:
        Libera toda a memoria: um free por no, depois a struct
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
    Retorno:
        void.
  =========================================================*/
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

/*=========================================================
    Função: imprimirRelatorioMemoria
    Autor: Marcos Paulo da Silva Oliveira

    Descrição:
        Calcula e exibe um relatório comparativo de memória
        entre as representações de matriz densa, vetor de
        triplas e lista encadeada.
        Apresenta os tamanhos medidos utilizando sizeof e
        compara os resultados com estimativas anteriores.
    Parâmetros:
        1 - m : ponteiro para a matriz esparsa
    Retorno:
        void.
  =========================================================*/
void imprimirRelatorioMemoria(MatrizEsparsa *m) {
    unsigned long densa   = (unsigned long)m->linhas * m->colunas * sizeof(int);
    unsigned long triplas = (unsigned long)m->total * sizeof(Tripla);
    unsigned long lista   = (unsigned long)m->total * sizeof(No);

    printf("\n  RELATORIO DE MEMORIA\n");
    printf("  Matriz %d x %d, k = %d nao-nulos\n\n", m->linhas, m->colunas, m->total);
    printf("  sizeof(int)    = %lu bytes\n", (unsigned long)sizeof(int));
    printf("  sizeof(Tripla) = %lu bytes\n", (unsigned long)sizeof(Tripla));
    printf("  sizeof(No)     = %lu bytes\n\n", (unsigned long)sizeof(No));

    printf("  Representacao      Medido (bytes)   Estimado E1 (bytes)\n");
    printf("  -----------------  ---------------  -------------------\n");
    printf("  Matriz densa       %15lu  %19lu\n", densa, E1_DENSA);
    printf("  Vetor de triplas   %15lu  %19lu\n", triplas, E1_TRIPLAS);
    printf("  Lista encadeada    %15lu  %19lu\n\n", lista, E1_LISTA);

    if (lista > 0) {
        printf("  Economia (densa / lista): %.1f vezes\n\n", (double)densa / (double)lista);
    }
}

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