# Sistema de Análise da Distribuição de Matrículas Universitárias

Matriz esparsa **aluno × disciplina** implementada em **C** com **lista encadeada ordenada** por `(linha, coluna)`. O sistema analisa a demanda de cada disciplina e a carga de disciplinas de cada aluno, e mostra, com dados do próprio sistema, quanta memória se economiza ao guardar só as matrículas que existem, em vez de uma tabela gigante cheia de zeros.

> Trabalho 2 da N2 de **Estrutura de Dados II** — PUC Goiás — Prof. Dr. Rovilson Mezencio.

---

## Sobre o projeto

Em uma universidade, cada aluno cursa poucas disciplinas de um catálogo enorme. Visto como tabela, o sistema é uma matriz em que a linha é o aluno, a coluna é a disciplina e a célula indica a matrícula. Quase tudo é zero (o aluno não está matriculado), e uma matriz comum `int m[L][C]` gastaria memória com milhões de zeros.

A matriz esparsa guarda só as células diferentes de zero, em uma lista encadeada mantida sempre ordenada por `(linha, coluna)`. A matriz comum acessa uma célula em O(1), mas a lista paga O(k) na busca, onde k é o número de não-nulos. O ganho está na memória, e é isso que o relatório final mede.

## O tema

| Item | Definição |
|---|---|
| **Linha** | Aluno, identificado pelo código numérico |
| **Coluna** | Disciplina, identificada pelo código numérico |
| **Valor** | `1` = o aluno está matriculado na disciplina no semestre. Na soma de dois semestres, `2` = cursou a disciplina nos dois semestres |
| **Célula vazia (zero)** | O aluno não está matriculado na disciplina (a célula não é armazenada na lista) |

Exemplos de células preenchidas:

| Célula | Significado |
|---|---|
| `(1042, 17, 1)` | O aluno 1042 está matriculado na disciplina 17 |
| `(1042, 35, 1)` | O aluno 1042 também está matriculado na disciplina 35 |
| `(2310, 17, 1)` | O aluno 2310 está matriculado na disciplina 17 |

## O nó da lista

```c
typedef struct No {
    int linha;            /* codigo do aluno */
    int coluna;           /* codigo da disciplina */
    int valor;            /* 1 = matriculado; 2 = cursou nos dois semestres; zero nunca e armazenado */
    struct No *prox;
} No;
```

Para o relatório de memória, a versão em triplas (vetor de registros) usa:

```c
typedef struct {
    int linha;
    int coluna;
    int valor;
} Tripla;
```

## Operações e seu significado no sistema

| Operação | O que responde no sistema |
|---|---|
| `inserir(i, j, v)` | Matricular o aluno `i` na disciplina `j`. Par repetido atualiza o valor; `v = 0` não insere |
| `buscar(i, j)` | O aluno `i` está matriculado na disciplina `j`? (`0` = não) |
| `remover(i, j)` | Cancelar a matrícula do aluno `i` na disciplina `j` |
| `somaLinha(i)` | Quantas disciplinas o aluno `i` está cursando? Na matriz dos dois semestres somados: quantas matrículas ele teve no total, contando repetições |
| `somaColuna(j)` | Quantos alunos estão matriculados na disciplina `j`, ou seja, qual a demanda dela? Na matriz somada: total de matrículas da disciplina nos dois semestres |
| `contarLinha(i)` | Quantas disciplinas **diferentes** o aluno `i` cursa? Em um semestre coincide com a soma da linha; na matriz somada, a disciplina cursada nos dois semestres conta uma só vez |
| `contarColuna(j)` | Quantos alunos **diferentes** estão matriculados na disciplina `j`? |
| **Transposta** | Passa de "aluno × disciplina" para "disciplina × aluno": cada linha passa a listar os alunos de uma disciplina, como uma lista de chamada |
| **Adição** | Matrículas do 1º semestre + matrículas do 2º semestre = total de matrículas no ano letivo (valor `2` = cursou a disciplina nos dois semestres). Se uma das matrizes for de ajustes (`+1` inclusão, `-1` trancamento), a matrícula trancada resulta em `0` e não é armazenada (cancelamento) |

Cada saída é impressa com a frase do tema, e não só o número:

```
O aluno 42 cursa 5 disciplinas
A disciplina 17 tem 120 alunos matriculados
O aluno 42 teve 7 matriculas no periodo (5 disciplinas diferentes)
```

## Funcionalidades

**Entrega 2 — Núcleo da matriz esparsa (em sala)**

- [ ] `struct No` com `linha`, `coluna`, `valor` e `prox`
- [ ] `inserir()` ordenado por (linha, coluna): par repetido atualiza o valor e valor zero não insere
- [ ] `buscar(i, j)`: devolve o valor da célula ou `0` se ela não existe
- [ ] Pelo menos 15 matrículas inseridas fora de ordem, com a lista impressa já ordenada
- [ ] `imprimirRecorte()`: mostra um pedaço da matriz (ex.: linhas 0 a 5, colunas 0 a 5) com os zeros, como tabela
- [ ] Ocupação em %: não-nulos / (linhas × colunas)

**Entrega Final — Sistema completo**

- [ ] `remover(i, j)` (cancelamento de matrícula)
- [ ] `somaLinha`, `somaColuna`, `contarLinha` e `contarColuna` com as frases do tema
- [ ] Carga dos arquivos `.txt` (`linha;coluna;valor`) dos dois semestres, com matriz de pelo menos 500 × 500 e mais de 1000 não-nulos
- [ ] Transposta ordenada (disciplina × aluno), funcionando como lista de chamada
- [ ] Adição dos dois semestres (merge da Aula 3), tratando o cancelamento (resultado zero não é armazenado)
- [ ] Menu interativo no console com todas as operações
- [ ] Relatório de memória: `sizeof(No)` medido e comparação densa × triplas × lista encadeada com `sizeof` no programa, confrontada com a estimativa da E1

**SURPREENDA — funcionalidade extra (1,0 ponto)**

- [ ] Ranking das 5 disciplinas mais procuradas e lista das disciplinas com baixa procura (possível turma vazia), para apoiar a decisão sobre a oferta de vagas *(ideia registrada na E1; pode mudar até a E2)*

## Estrutura do repositório

```
.
├── src/
│   ├── matrizEsparsa.c   # lista ordenada: inserir, buscar, remover, somas, adição, transposta
│   └── main.c            # menu e testes
├── libraries/
│   └── matrizEsparsa.h   # structs No e Tripla, protótipos
├── data/
│   ├── semestre1.txt     # matrículas do 1º semestre (500x500 ou mais, 1000+ não-nulos)
│   ├── semestre2.txt     # matrículas do 2º semestre, usada na adição
│   └── ajustes.txt       # ajustes do 1º semestre (+1 inclusão, -1 trancamento), usado para demonstrar o cancelamento
├── entregas/             # versões em arquivo único enviadas ao Teams
│   ├── T2_E2_esparsa-matriculas-uni.c
│   └── T2_EF_esparsa-matriculas-uni.c
├── docs/                 # E1 (.docx), roteiro do trabalho e relatório de memória
└── README.md
```

## Como compilar e executar

Requer um compilador C (o projeto é compatível com o **Dev-C++**).

**Com GCC:**

```bash
gcc -Ilibraries src/main.c src/matrizEsparsa.c -o esparsa
./esparsa          # no Windows: esparsa.exe
```

Execute o programa a partir da raiz do repositório, para que ele encontre os arquivos de `data/`.

**Com Dev-C++:** abra os arquivos de `src/` e `libraries/` em um projeto e compile (F11). Para as entregas, use o arquivo único em `entregas/`.

## Formato do arquivo de carga

Uma matrícula por linha, campos separados por `;`:

```
linha;coluna;valor
```

Exemplo de um semestre (aluno, disciplina, 1 = matriculado):

```
1042;17;1
1042;35;1
2310;17;1
```

Os registros podem vir fora de ordem: a lista se mantém ordenada a cada inserção.

Em uma matriz de ajustes, o trancamento entra como `-1` (ex.: `1042;35;-1`). Ao somar com a matriz do semestre, a célula vira zero e deixa de existir.

## Exemplo de saída esperada (Entrega 2)

Matriz de teste de 50 × 50:

```
Lista: (0,3,1) (1,0,1) (1,7,1) (4,2,1) ...
buscar(1,7) = 1   buscar(2,2) = 0
Ocupacao: 15 de 2500 celulas = 0.60%
```

## Cenário e estimativa de memória (E1)

Estimativa feita na Entrega 1, com 4 bytes por `int` e 24 bytes por nó da lista.

| Medida | Valor |
|---|---|
| Linhas (L) | 5.000 alunos |
| Colunas (C) | 500 disciplinas |
| Total de células (L × C) | 2.500.000 |
| Não-nulos estimados (k) | 25.000 (5.000 alunos × 5 disciplinas em média por semestre) |
| Ocupação (k / L × C) | 1% (abaixo do limite de 5%) |
| Memória densa (L × C × 4 bytes) | 10.000.000 bytes (≈ 9,5 MB) |
| Memória esparsa (k × 24 bytes) | 600.000 bytes (≈ 586 KB) |
| Economia | ≈ 16,7 vezes menor |

## Relatório de memória

> A preencher na Entrega Final, com a matriz carregada do arquivo e os tamanhos medidos com `sizeof` no próprio programa.

| Representação | Fórmula | Estimado (E1) | Medido (EF) |
|---|---|---|---|
| Matriz densa | linhas × colunas × `sizeof(int)` | 10.000.000 bytes | — |
| Vetor de triplas | k × `sizeof(Tripla)` | 300.000 bytes* | — |
| Lista encadeada | k × `sizeof(No)` | 600.000 bytes | — |

\* A E1 estimou só a densa e a lista; o valor das triplas é k × 12 bytes, com o mesmo k.

A E1 assumiu 24 bytes por nó, que é o valor de um compilador de 64 bits (3 `int` + padding + ponteiro de 8 bytes). Em compilador de 32 bits o `sizeof(No)` mede 16 bytes. Por isso o relatório final usa o valor medido.

## Entregas e pontuação

| Entrega | Onde | Pontos | Data (turma da manhã) |
|---|---|---|---|
| E1 — Proposta, EAP e cálculo de memória | Em sala | 0,5 | 19/11 |
| E2 — Núcleo da matriz esparsa | Em sala | 1,0 | 23/11 |
| EF — Sistema completo e defesa | Casa + defesa | 2,5 | 30/11 ou 03/12 |
| SURPREENDA — funcionalidade extra | Casa + defesa | 1,0 | 30/11 ou 03/12 |
| **Total** | | **5,0** | |

Datas sujeitas a ajuste pelo professor. N2 = Trabalho 1 + Trabalho 2 (5,0).

## Organização do trabalho (EAP)

| Pacote | O que o grupo vai fazer | Responsável |
|---|---|---|
| 1. Dados do tema | 1.1 Definir o cenário: 5.000 alunos, 500 disciplinas e cerca de 5 matrículas por aluno, com disciplinas mais e menos procuradas.<br>1.2 Gerar os arquivos `.txt` (`linha;coluna;valor`) de dois semestres, com pelo menos 500 × 500 e mais de 1.000 não-nulos. | Marcos Paulo da Silva Oliveira |
| 2. Núcleo da matriz esparsa | 2.1 Definir a `struct No` e o `inserir()` ordenado por (aluno, disciplina): par repetido atualiza o valor; valor zero não insere.<br>2.2 `buscar(i, j)`, `imprimirRecorte()` e impressão da ocupação (não-nulos / células, em %). | Marcos Paulo da Silva Oliveira |
| 3. Operações com significado | 3.1 `remover(i, j)`, `somaLinha`, `somaColuna`, `contarLinha` e `contarColuna`, com frases do tema.<br>3.2 Transposta ordenada (disciplina × aluno), funcionando como lista de chamada.<br>3.3 Adição dos dois semestres (merge), tratando o cancelamento. | Samuel de Godoy Larroque |
| 4. Métricas e relatório de memória | 4.1 Medir `sizeof(No)` e comparar, com `sizeof` no programa, memória densa × triplas × lista encadeada.<br>4.2 Comparar os números medidos com a estimativa da E1 e escrever o relatório de memória. | Todos |
| 5. Apresentação | 5.1 Menu interativo com todas as operações e carga dos arquivos de dados.<br>5.2 Roteiro da demonstração de 8 minutos e preparo da explicação das funções de cada integrante. | Todos |

## Equipe

| Nome | GitHub |
|---|---|
| Marcos Paulo da Silva Oliveira | [@hi-mask](https://github.com/hi-mask) |
| Samuel de Godoy Larroque | [@Chaveiro-do-cmd](https://github.com/Chaveiro-do-cmd) |

## Convenções do código

- Linguagem C compatível com Dev-C++: variáveis declaradas no topo das funções (nunca dentro do `for`) e sem acentos nos `printf`.
- Toda função leva um comentário `/* autor: Nome */`.
- Nomes em português e camelCase (`inserir`, `somaLinha`, `imprimirRecorte`).
- Os dados ficam **somente** na lista encadeada. Matriz comum (`int m[L][C]`) não é permitida.
- Todo código reaproveitado das aulas ou de fontes externas deve ser dominado e explicado pelo integrante responsável na defesa.

## Licença e uso acadêmico

Projeto desenvolvido para fins acadêmicos na disciplina de Estrutura de Dados II (PUC Goiás).