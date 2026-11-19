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
MatrizEsparsa *criar(int linhas, int colunas);


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
int posicao_valida(MatrizEsparsa *m, int linha, int coluna);

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
No *buscarNo(MatrizEsparsa *m, int linha, int coluna);

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
int buscar(MatrizEsparsa *m, int linha, int coluna);

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
int inserir(MatrizEsparsa *m, int linha, int coluna, int valor);

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
void imprimirRecorte(MatrizEsparsa *m, int linhaInicio, int linhaFim, int colunaInicio, int colunaFim);

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
void imprimirOcupacao(MatrizEsparsa *m);

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
void remover(MatrizEsparsa *m, int linha, int coluna);

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
void consultar(MatrizEsparsa *m, int linha, int coluna);

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
void imprimir_estrutura(MatrizEsparsa *m);

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
void imprimir_matriz(MatrizEsparsa *m);

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
void destruir(MatrizEsparsa *m);

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
void imprimirRelatorioMemoria(MatrizEsparsa *m);
#endif