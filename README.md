# Matriz Esparsa de Matrículas Universitárias

Matriz esparsa **aluno × disciplina** implementada em **C** com **lista encadeada ordenada** por `(linha, coluna)`. O projeto mostra, com dados do próprio sistema, quanta memória se economiza ao guardar só as matrículas que existem, em vez de uma tabela gigante cheia de zeros.

> Trabalho 2 da N2 de **Estrutura de Dados II** — PUC Goiás — Prof. Dr. Rovilson Mezencio.

---

## Sobre o projeto

Em uma universidade, cada aluno cursa poucas disciplinas de um catálogo enorme. Visto como tabela, o sistema é uma matriz em que a linha é o aluno, a coluna é a disciplina e a célula guarda a nota. Quase tudo é zero (o aluno não está matriculado), e uma matriz comum `int m[L][C]` gastaria memória com milhões de zeros.

A matriz esparsa guarda só as células diferentes de zero, em uma lista encadeada mantida sempre ordenada por `(linha, coluna)`. A matriz comum acessa uma célula em O(1), mas a lista paga O(k) na busca, onde k é o número de não-nulos. O ganho está na memória, e é isso que o relatório final mede.

## O tema

| Item | Definição |
|---|---|
| **Linha** | Aluno — código do aluno (`0` a `999`) |
| **Coluna** | Disciplina — código da disciplina (`0` a `499`) |
| **Valor** | Nota inteira de `1` a `10` |
| **Célula vazia (zero)** | O aluno não está matriculado na disciplina |

Como o valor `0` nunca é armazenado, a menor nota guardada é `1`.

## O nó da lista

```c
typedef struct No {
    int linha;            /* codigo do aluno */
    int coluna;           /* codigo da disciplina */
    int valor;            /* nota (1 a 10); zero nunca e armazenado */
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

| Operação | Significado no sistema de matrículas |
|---|---|
| `inserir(i, j, v)` | Matricular o aluno `i` na disciplina `j` com nota `v`. Posição repetida atualiza a nota; `v = 0` não insere |
| `buscar(i, j)` | Consultar a nota do aluno `i` na disciplina `j` (`0` = não matriculado) |
| `remover(i, j)` | Cancelar a matrícula do aluno `i` na disciplina `j` |
| `somaLinha(i)` | Total de pontos do aluno `i` (soma das notas de todas as disciplinas) |
| `contarLinha(i)` | Quantidade de disciplinas em que o aluno `i` está matriculado |
| `somaColuna(j)` | Total de pontos da turma da disciplina `j` |
| `contarColuna(j)` | Quantidade de alunos matriculados na disciplina `j` |
| **Transposta** | Troca aluno × disciplina por disciplina × aluno: de "quais disciplinas o aluno cursa" para "quais alunos estão na disciplina" (diário de classe) |
| **Adição** | Nota da 1ª avaliação + nota da 2ª avaliação = pontos acumulados no semestre. Resultado zero (ex.: ajuste que anula a nota) não é armazenado |

Cada saída é impressa com a frase do tema, e não só o número:

```
Aluno 42 cursa 8 disciplinas
Aluno 42 acumulou 61 pontos em 8 disciplinas (media 7.6)
Disciplina 17 tem 35 alunos matriculados
```

## Funcionalidades

**Entrega 2 — Núcleo da matriz esparsa (em sala)**

- [ ] `struct No` com `linha`, `coluna`, `valor` e `prox`
- [ ] `inserir()` ordenado por (linha, coluna): posição repetida atualiza o valor e valor zero não insere
- [ ] `buscar(i, j)`: devolve a nota ou `0` se a célula não existe
- [ ] Pelo menos 15 matrículas inseridas fora de ordem, com a lista impressa já ordenada
- [ ] `imprimirRecorte()`: mostra um pedaço da matriz (ex.: linhas 0 a 5, colunas 0 a 5) com os zeros, como tabela
- [ ] Ocupação em %: não-nulos / (linhas × colunas)

**Entrega Final — Sistema completo**

- [ ] `remover(i, j)` (cancelamento de matrícula)
- [ ] `somaLinha`, `somaColuna`, `contarLinha` e `contarColuna` com as frases do tema
- [ ] Carga de arquivo `.txt` (`linha;coluna;valor`) com matriz de pelo menos 500 × 500 e 1000+ não-nulos
- [ ] Adição de duas matrizes (merge da Aula 3), tratando o cancelamento
- [ ] Transposta, com o resultado também ordenado
- [ ] Menu interativo no console com todas as operações
- [ ] Relatório de memória medido com `sizeof`, comparado com a estimativa da E1
- [ ] Histórico do aluno: lista as disciplinas e notas de um aluno
- [ ] Diário da disciplina: lista os alunos e notas de uma disciplina
- [ ] Média do aluno e média da turma
- [ ] Situação na disciplina: aprovados e reprovados (nota de corte configurável)

**SURPREENDA — funcionalidade extra (1,0 ponto)**

- [ ] Ranking dos 5 alunos com maior média *(ideia registrada na E1; pode mudar até a E2)*

## Estrutura do repositório

```
.
├── src/
│   ├── matrizEsparsa.h   # structs No e Tripla, protótipos
│   ├── matrizEsparsa.c   # lista ordenada: inserir, buscar, remover, somas, adição, transposta
│   └── main.c            # menu e testes
├── data/
│   ├── matriz_av1.txt    # carga de dados (500x500 ou mais, 1000+ não-nulos)
│   └── matriz_av2.txt    # segunda matriz, usada na adição
├── entregas/             # versões em arquivo único enviadas ao Teams
│   ├── T2_E2_<grupo>.c
│   └── T2_EF_<grupo>.c
├── docs/                 # E1 (.docx), roteiro do trabalho e relatório de memória
└── README.md
```

## Como compilar e executar

Requer um compilador C (o projeto é compatível com o **Dev-C++**).

**Com GCC:**

```bash
gcc src/main.c src/matrizEsparsa.c -o esparsa
./esparsa          # no Windows: esparsa.exe
```

Execute o programa a partir da raiz do repositório, para que ele encontre os arquivos de `data/`.

**Com Dev-C++:** abra os arquivos de `src/` em um projeto e compile (F11). Para as entregas, use o arquivo único em `entregas/`.

## Formato do arquivo de carga

Uma matrícula por linha, campos separados por `;`:

```
linha;coluna;valor
```

Exemplo (aluno 0 na disciplina 3 com nota 8, e assim por diante):

```
0;3;8
0;17;7
1;0;9
1;7;6
4;2;7
```

Os registros podem vir fora de ordem: a lista se mantém ordenada a cada inserção.

## Exemplo de saída esperada (Entrega 2)

```
Lista: (0,3,8) (1,0,9) (1,7,6) (4,2,7) ...
buscar(1,7) = 6   buscar(2,2) = 0
Ocupacao: 15 de 2500 celulas = 0.60%
```

## Cenário e estimativa de memória (E1)

> Valores propostos para a E1. Ajustar caso o grupo escolha outras dimensões.

| Medida | Valor |
|---|---|
| Dimensões | 1000 alunos × 500 disciplinas |
| Total de células | 500.000 |
| Não-nulos estimados (k) | ≈ 6.000 (cerca de 6 disciplinas por aluno) |
| Ocupação estimada | ≈ 1,2% (abaixo do limite de 5%) |

## Relatório de memória

> A preencher na Entrega Final, com a matriz carregada do arquivo e os tamanhos medidos com `sizeof` no próprio programa.

| Representação | Fórmula | Estimado (E1) | Medido (EF) |
|---|---|---|---|
| Matriz densa | linhas × colunas × `sizeof(int)` | 2.000.000 bytes | — |
| Vetor de triplas | k × `sizeof(Tripla)` | 72.000 bytes | — |
| Lista encadeada | k × `sizeof(No)` | 96.000 a 144.000 bytes | — |

O `sizeof(No)` muda com a plataforma: 16 bytes em compilador de 32 bits e 24 bytes em 64 bits (por causa do alinhamento do ponteiro). Por isso o relatório final usa o valor medido, e não o estimado.

## Entregas e pontuação

| Entrega | Onde | Pontos | Manhã | Noite |
|---|---|---|---|---|
| E1 — Proposta, EAP e cálculo de memória | Em sala | 0,5 | 19/11 | 20/11 |
| E2 — Núcleo da matriz esparsa | Em sala | 1,0 | 23/11 | 24/11 |
| EF — Sistema completo e defesa | Casa + defesa | 2,5 | 30/11 ou 03/12 | 01/12 ou 04/12 |
| SURPREENDA — funcionalidade extra | Casa + defesa | 1,0 | 30/11 ou 03/12 | 01/12 ou 04/12 |
| **Total** | | **5,0** | | |

Datas sujeitas a ajuste pelo professor. N2 = Trabalho 1 + Trabalho 2 (5,0).

## Organização do trabalho (EAP)

| Pacote | Responsável |
|---|---|
| 1. Dados do tema | Marcos Paulo da Silva Oliveira |
| 2. Núcleo da matriz esparsa | Samuel de Godoy Larroque |
| 3. Funcionalidades | Samuel de Godoy Larroque |
| 4. Relatório de memória | Marcos Paulo da Silva Oliveira |
| 5. Apresentação e defesa | Todos |

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
