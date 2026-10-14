// #################################################
//  Instituto Federal da Bahia
//  Disciplina: INF029 - Laboratório de Programação
//  Trabalho 1 - Questão 8: Jogo da Velha
//
//  Compilar: gcc q8.c -o q8
//  Executar: ./q8
// #################################################
//  ----- Dados do Aluno -----
//  Nome: KAUAN LISBOA DO VALE
//  email: 20252160001@ifba.edu.br
//  Matrícula: 20252160001
//  Semestre: terceiro semestre

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MARCA_J1 'X'   // marca do jogador 1
#define MARCA_J2 'O'   // marca do jogador 2 (troque aqui se o professor quiser o zero '0')

char tab[3][3];

void limparTabuleiro(void)
{
    int l, c;
    for (l = 0; l < 3; l++)
        for (c = 0; c < 3; c++)
            tab[l][c] = ' ';
}

// mostra o tabuleiro com as linhas A, B, C e as colunas 1, 2, 3
void mostrarTabuleiro(void)
{
    int l;
    printf("\n      1   2   3\n");
    for (l = 0; l < 3; l++)
    {
        printf("  %c   %c | %c | %c \n", 'A' + l, tab[l][0], tab[l][1], tab[l][2]);
        if (l < 2)
            printf("     ---+---+---\n");
    }
    printf("\n");
}

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

// converte um texto como "B3" em linha/coluna. Retorna 1 se for uma célula válida
int lerCelula(const char *s, int *lin, int *col)
{
    int i = 0;
    while (isspace((unsigned char)s[i]))
        i++;

    char letra = (char)toupper((unsigned char)s[i]);
    char numero = s[i + 1];
    i += 2;

    while (isspace((unsigned char)s[i]))
        i++;

    if (letra < 'A' || letra > 'C' || numero < '1' || numero > '3' || s[i] != '\0')
        return 0;

    *lin = letra - 'A';
    *col = numero - '1';
    return 1;
}

// retorna 1 se a marca informada formou uma linha, coluna ou diagonal
int venceu(char m)
{
    int i;
    for (i = 0; i < 3; i++)
    {
        if (tab[i][0] == m && tab[i][1] == m && tab[i][2] == m) return 1; // linha
        if (tab[0][i] == m && tab[1][i] == m && tab[2][i] == m) return 1; // coluna
    }
    if (tab[0][0] == m && tab[1][1] == m && tab[2][2] == m) return 1;     // diagonal
    if (tab[0][2] == m && tab[1][1] == m && tab[2][0] == m) return 1;     // diagonal
    return 0;
}

int main(void)
{
    char entrada[100];
    int jogador = 1, jogadas = 0, lin, col, ganhou = 0;
    char marca;

    limparTabuleiro();

    while (jogadas < 9 && !ganhou)
    {
        marca = (jogador == 1) ? MARCA_J1 : MARCA_J2;

        mostrarTabuleiro();
        printf("Jogador %d (%c), informe sua jogada (ex: B3): ", jogador, marca);
        lerLinha(entrada, sizeof(entrada));

        if (!lerCelula(entrada, &lin, &col))
        {
            printf("\nCélula inválida! Use uma letra (A-C) e um número (1-3), por exemplo B3.\n");
            continue;
        }
        if (tab[lin][col] != ' ')
        {
            printf("\nEssa célula já está ocupada! Escolha outra.\n");
            continue;
        }

        tab[lin][col] = marca;
        jogadas++;

        if (venceu(marca))
            ganhou = 1;
        else
            jogador = (jogador == 1) ? 2 : 1;
    }

    mostrarTabuleiro();
    if (ganhou)
        printf("Fim de jogo! O ganhador foi o Jogador %d (%c).\n", jogador, marca);
    else
        printf("Fim de jogo! Deu velha: não houve ganhador.\n");

    return 0;
}