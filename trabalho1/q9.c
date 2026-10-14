// #################################################
//  Instituto Federal da Bahia
//  Disciplina: INF029 - Laboratório de Programação
//  Trabalho 1 - Questão 9: Batalha Naval
//
//  Compilar: gcc q9.c -o q9
//  Executar: ./q9
//
//  Regras implementadas:
//   - Tabuleiro de cada jogador: matriz 10 x 10 (linhas A-J, colunas 1-10)
//   - NUM_NAVIOS (abaixo) é o único valor a mudar para aumentar a quantidade de navios.
//     Composição: 1 navio de tamanho 4, 1 de tamanho 3, 3 de tamanho 1 e
//     os demais (NUM_NAVIOS - 5) obrigatoriamente de tamanho 2. Mínimo: 5 navios.
//   - Mapa: ' ' mar sem nada | 'N' navio | '0' navio abatido | 'X' tiro na água
// #################################################
//  ----- Dados do Aluno -----
//  Nome: KAUAN LISBOA DO VALE
//  email: 20252160001@ifba.edu.br
//  Matrícula: 20252160001
//  Semestre: terceiro semestre

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define NUM_NAVIOS 5   // parâmetro global: mínimo 5 (para mais navios, só mude esse valor)

#if NUM_NAVIOS < 5
#error "NUM_NAVIOS deve ser no minimo 5 (1 de tamanho 4, 1 de tamanho 3 e 3 de tamanho 1)"
#endif

#define TAM 10

#define MAR    ' '
#define NAVIO  'N'
#define ACERTO '0'
#define TIRO   'X'

// tamanho do navio de número i (0 .. NUM_NAVIOS-1)
int tamanhoNavio(int i)
{
    if (i == 0) return 4;
    if (i == 1) return 3;
    if (i < 5)  return 1;
    return 2;
}

// ---------------------------------------------------------------
// Entrada de dados
// ---------------------------------------------------------------

// lê uma linha do teclado (sem deixar sobras no buffer)
void lerLinha(char *buf, int n)
{
    int ch, i = 0;
    ch = getchar();
    if (ch == EOF)
    {
        printf("\nEntrada encerrada.\n");
        exit(0);
    }
    while (ch != '\n' && ch != EOF)
    {
        if (i < n - 1)
            buf[i++] = (char)ch;
        ch = getchar();
    }
    buf[i] = '\0';
}

// converte um texto como "B3" ou "B3 H" em linha/coluna (e direção, se informada).
// Retorna 1 se for válido. *dir recebe 'H', 'V' ou 0 (quando não informada).
int lerCelula(const char *s, int *lin, int *col, char *dir)
{
    int i = 0, c = 0, digitos = 0;
    char letra, d = 0;

    while (isspace((unsigned char)s[i]))
        i++;
    if (!isalpha((unsigned char)s[i]))
        return 0;
    letra = (char)toupper((unsigned char)s[i]);
    i++;

    while (isdigit((unsigned char)s[i]))
    {
        c = c * 10 + (s[i] - '0');
        digitos++;
        i++;
        if (digitos > 2)
            return 0;
    }
    if (digitos == 0)
        return 0;

    while (isspace((unsigned char)s[i]))
        i++;
    if (s[i] != '\0')
    {
        d = (char)toupper((unsigned char)s[i]);
        i++;
        while (isspace((unsigned char)s[i]))
            i++;
        if (s[i] != '\0' || (d != 'H' && d != 'V'))
            return 0;
    }

    if (letra < 'A' || letra >= 'A' + TAM || c < 1 || c > TAM)
        return 0;

    *lin = letra - 'A';
    *col = c - 1;
    *dir = d;
    return 1;
}

// ---------------------------------------------------------------
// Tabuleiros
// ---------------------------------------------------------------

void limparTabuleiro(char t[TAM][TAM])
{
    int l, c;
    for (l = 0; l < TAM; l++)
        for (c = 0; c < TAM; c++)
            t[l][c] = MAR;
}

// desenha um tabuleiro. Se esconderNavios = 1 (tabuleiro do adversário), os 'N' aparecem como mar
void imprimirTabuleiro(char t[TAM][TAM], int esconderNavios)
{
    int l, c;
    char ch;

    printf("   ");
    for (c = 1; c <= TAM; c++)
        printf(" %-2d", c);
    printf("\n");

    for (l = 0; l < TAM; l++)
    {
        printf(" %c ", 'A' + l);
        for (c = 0; c < TAM; c++)
        {
            ch = t[l][c];
            if (esconderNavios && ch == NAVIO)
                ch = MAR;
            printf("[%c]", ch);
        }
        printf("\n");
    }
}

// função para listar os tabuleiros do jogador
void listarTabuleiros(char tab[2][TAM][TAM], int jogador)
{
    printf("\n=== SEU TABULEIRO (Jogador %d) ===\n", jogador + 1);
    printf("(N = seu navio, 0 = navio atingido, X = tiro do oponente na água)\n");
    imprimirTabuleiro(tab[jogador], 0);

    printf("\n=== TABULEIRO DO ADVERSARIO ===\n");
    printf("(0 = seus tiros certeiros, X = seus tiros errados)\n");
    imprimirTabuleiro(tab[1 - jogador], 1);
    printf("\n");
}

// "limpa a tela" para o outro jogador não ver o tabuleiro
void esconderTela(void)
{
    char buf[100];
    printf("\nPressione ENTER para continuar...");
    lerLinha(buf, sizeof(buf));
    int i;
    for (i = 0; i < 40; i++)
        printf("\n");
}

// ---------------------------------------------------------------
// Posicionamento dos navios
// ---------------------------------------------------------------

// verifica se o navio cabe no tabuleiro e não encosta em outro. Retorna 1 se for válido
int posicaoValida(char t[TAM][TAM], int lin, int col, char dir, int tam)
{
    int k, l, c;
    for (k = 0; k < tam; k++)
    {
        l = lin + (dir == 'V' ? k : 0);
        c = col + (dir == 'H' ? k : 0);
        if (l >= TAM || c >= TAM)
            return 0;               // saiu do tabuleiro
        if (t[l][c] != MAR)
            return 0;               // já tem navio nessa posição
    }
    return 1;
}

void colocarNavio(char t[TAM][TAM], int lin, int col, char dir, int tam)
{
    int k;
    for (k = 0; k < tam; k++)
    {
        if (dir == 'V')
            t[lin + k][col] = NAVIO;
        else
            t[lin][col + k] = NAVIO;
    }
}

// cada jogador informa onde fica cada um dos seus navios
void posicionarNavios(char t[TAM][TAM], int jogador)
{
    char entrada[100], dir;
    int i, tam, lin, col;

    printf("\n===== Jogador %d: posicione seus navios =====\n", jogador + 1);
    printf("Informe a célula inicial (linha A-J e coluna 1-%d), e a direção:\n", TAM);
    printf("  H = horizontal (para a direita)   V = vertical (para baixo)\n");
    printf("  Exemplo: B3 H     (navios de tamanho 1 não precisam de direção: B3)\n");

    for (i = 0; i < NUM_NAVIOS; i++)
    {
        tam = tamanhoNavio(i);
        while (1)
        {
            imprimirTabuleiro(t, 0);
            printf("Navio %d de %d (tamanho %d): ", i + 1, NUM_NAVIOS, tam);
            lerLinha(entrada, sizeof(entrada));

            if (!lerCelula(entrada, &lin, &col, &dir))
            {
                printf("\nEntrada inválida! Exemplo: B3 H\n\n");
                continue;
            }
            if (tam == 1)
                dir = 'H';
            else if (dir == 0)
            {
                printf("\nInforme também a direção (H ou V). Exemplo: B3 H\n\n");
                continue;
            }
            if (!posicaoValida(t, lin, col, dir, tam))
            {
                printf("\nPosição inválida: o navio sai do tabuleiro ou bate em outro navio.\n\n");
                continue;
            }

            colocarNavio(t, lin, col, dir, tam);
            break;
        }
    }
    imprimirTabuleiro(t, 0);
    printf("Navios posicionados!\n");
}

// ---------------------------------------------------------------
// Jogo
// ---------------------------------------------------------------

// função para atirar: o jogador escolhe uma célula do tabuleiro do adversário.
// Retorna 1 se acertou um navio, 0 se foi na água.
int atirar(char alvo[TAM][TAM], int *restantes)
{
    char entrada[100], dir;
    int lin, col;

    while (1)
    {
        printf("Informe a célula do tiro (ex: B3): ");
        lerLinha(entrada, sizeof(entrada));

        if (!lerCelula(entrada, &lin, &col, &dir) || dir != 0)
        {
            printf("Célula inválida! Use uma letra (A-J) e um número (1-%d).\n", TAM);
            continue;
        }
        if (alvo[lin][col] == ACERTO || alvo[lin][col] == TIRO)
        {
            printf("Você já atirou nessa célula! Escolha outra.\n");
            continue;
        }
        break;
    }

    if (alvo[lin][col] == NAVIO)
    {
        alvo[lin][col] = ACERTO;
        (*restantes)--;
        printf("\n>>> ACERTOU um navio!\n");
        return 1;
    }
    alvo[lin][col] = TIRO;
    printf("\n>>> Tiro na água.\n");
    return 0;
}

int main(void)
{
    char tab[2][TAM][TAM];
    int restantes[2];
    int total = 0, i, jogador, vez = 0, acao;
    char entrada[100];

    for (i = 0; i < NUM_NAVIOS; i++)
        total += tamanhoNavio(i);
    if (total > TAM * TAM)
    {
        printf("NUM_NAVIOS grande demais para um tabuleiro %dx%d.\n", TAM, TAM);
        return 1;
    }

    printf("===== BATALHA NAVAL =====\n");
    printf("Cada jogador tem %d navios.\n", NUM_NAVIOS);

    for (jogador = 0; jogador < 2; jogador++)
    {
        limparTabuleiro(tab[jogador]);
        restantes[jogador] = total;
        posicionarNavios(tab[jogador], jogador);
        esconderTela();
    }

    // cada jogador joga uma vez, depois é a vez do oponente
    while (restantes[0] > 0 && restantes[1] > 0)
    {
        printf("\n===== Vez do Jogador %d =====\n", vez + 1);
        while (1)
        {
            printf("1 - Listar tabuleiros\n2 - Atirar\nEscolha: ");
            lerLinha(entrada, sizeof(entrada));
            acao = atoi(entrada);

            if (acao == 1)
                listarTabuleiros(tab, vez);
            else if (acao == 2)
            {
                atirar(tab[1 - vez], &restantes[1 - vez]);
                break;
            }
            else
                printf("Opção inválida!\n");
        }

        if (restantes[1 - vez] == 0)
            break;

        esconderTela();
        vez = 1 - vez;
    }

    printf("\n===== FIM DE JOGO =====\n");
    printf("O Jogador %d venceu, afundando todos os navios do oponente!\n", vez + 1);
    printf("\nTabuleiro final do Jogador 1:\n");
    imprimirTabuleiro(tab[0], 0);
    printf("\nTabuleiro final do Jogador 2:\n");
    imprimirTabuleiro(tab[1], 0);

    return 0;
}