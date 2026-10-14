// #################################################
//  Instituto Federal da Bahia
//  Salvador - BA
//  Curso de Análise e Desenvolvimento de Sistemas http://ads.ifba.edu.br
//  Disciplina: INF029 - Laboratório de Programação
//  Professor: Renato Novais - renato@ifba.edu.br

//  ----- Orientações gerais -----
//  Descrição: esse arquivo deve conter as questões do trabalho do aluno.
//  O aluno deve preencher seus dados abaixo, e implementar as questões do trabalho

//  ----- Dados do Aluno -----
//  Nome:
//  email:
//  Matrícula:
//  Semestre:

//  Copyright © 2016 Renato Novais. All rights reserved.
// Última atualização: 07/05/2021 - 19/08/2016 - 17/10/2025

// #################################################

#include <stdio.h>
#include "trabalho1.h" 
#include <stdlib.h>

DataQuebrada quebraData(char data[]);

/*
## função utilizada para testes  ##

 somar = somar dois valores
@objetivo
    Somar dois valores x e y e retonar o resultado da soma
@entrada
    dois inteiros x e y
@saida
    resultado da soma (x + y)
 */
int somar(int x, int y)
{
    int soma;
    soma = x + y;
    return soma;
}

/*
## função utilizada para testes  ##

 fatorial = fatorial de um número
@objetivo
    calcular o fatorial de um número
@entrada
    um inteiro x
@saida
    fatorial de x -> x!
 */
int fatorial(int x)
{ //função utilizada para testes
  int i, fat = 1;
    
  for (i = x; i > 1; i--)
    fat = fat * i;
    
  return fat;
}

int teste(int a)
{
    int val;
    if (a == 2)
        val = 3;
    else
        val = 4;

    return val;
}

/* ---------- funções auxiliares de data (usadas na q1 e na q2) ---------- */

static int ehBissexto(int ano)
{
  return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

static int diasNoMes(int mes, int ano)
{
  int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (mes == 2 && ehBissexto(ano)) return 29;
  return dias[mes - 1];
}

/*
 Q1 = validar data
@objetivo
    Validar uma data
@entrada
    uma string data. Formatos que devem ser aceitos: dd/mm/aaaa, onde dd = dia, mm = mês, e aaaa, igual ao ano. dd em mm podem ter apenas um digito, e aaaa podem ter apenas dois digitos.
@saida
    0 -> se data inválida
    1 -> se data válida
 @restrições
    Não utilizar funções próprias de string (ex: strtok)   
    pode utilizar strlen para pegar o tamanho da string
 */
int q1(char data[])
{
  int i, barras = 0, digitos = 0;

  // pré-validação: só dígitos e duas barras, com 1-2 dígitos em dia/mês e 2 ou 4 no ano
  for (i = 0; data[i] != '\0'; i++)
  {
    if (data[i] == '/')
    {
      if (digitos < 1 || digitos > 2) return 0;
      barras++;
      digitos = 0;
    }
    else if (data[i] >= '0' && data[i] <= '9')
    {
      digitos++;
      if (digitos > 4) return 0;
    }
    else
      return 0;
  }
  if (barras != 2 || (digitos != 2 && digitos != 4)) return 0;

  DataQuebrada dq = quebraData(data);
  if (!dq.valido) return 0;

  int dia = dq.iDia, mes = dq.iMes, ano = dq.iAno;
  if (ano < 100) ano += 2000; // ano com 2 dígitos: assume 20xx

  if (ano < 1 || mes < 1 || mes > 12 || dia < 1) return 0;

  int maxDia = diasNoMes(mes, ano);

  return dia <= maxDia;
}



/*
 Q2 = diferença entre duas datas
 @objetivo
    Calcular a diferença em anos, meses e dias entre duas datas
 @entrada
    uma string datainicial, uma string datafinal. 
 @saida
    Retorna um tipo DiasMesesAnos. No atributo retorno, deve ter os possíveis valores abaixo
    1 -> cálculo de diferença realizado com sucesso
    2 -> datainicial inválida
    3 -> datafinal inválida
    4 -> datainicial > datafinal
    Caso o cálculo esteja correto, os atributos qtdDias, qtdMeses e qtdAnos devem ser preenchidos com os valores correspondentes.
 */
DiasMesesAnos q2(char datainicial[], char datafinal[])
{
    DiasMesesAnos dma;

    if (q1(datainicial) == 0){
      dma.retorno = 2;
      return dma;
    }else if (q1(datafinal) == 0){
      dma.retorno = 3;
      return dma;
    }else{
      DataQuebrada ini = quebraData(datainicial);
      DataQuebrada fim = quebraData(datafinal);

      // ano com 2 dígitos: mesma regra da q1 (20xx)
      if (ini.iAno < 100) ini.iAno += 2000;
      if (fim.iAno < 100) fim.iAno += 2000;

      // verifica se a data final não é menor que a inicial
      if (fim.iAno < ini.iAno ||
         (fim.iAno == ini.iAno && fim.iMes < ini.iMes) ||
         (fim.iAno == ini.iAno && fim.iMes == ini.iMes && fim.iDia < ini.iDia)){
        dma.retorno = 4;
        return dma;
      }

      // 29/02 em ano final não bissexto: o "aniversário" cai em 28/02
      if (ini.iDia == 29 && ini.iMes == 2 && !ehBissexto(fim.iAno))
        ini.iDia = 28;

      int dias = fim.iDia - ini.iDia;
      int meses = fim.iMes - ini.iMes;
      int anos = fim.iAno - ini.iAno;

      if (dias < 0){
        // empresta os dias do mês anterior ao da data final
        int mesAnterior = fim.iMes - 1;
        int anoMesAnterior = fim.iAno;
        if (mesAnterior == 0){
          mesAnterior = 12;
          anoMesAnterior--;
        }
        dias += diasNoMes(mesAnterior, anoMesAnterior);
        meses--;
      }
      if (meses < 0){
        meses += 12;
        anos--;
      }

      dma.qtdDias = dias;
      dma.qtdMeses = meses;
      dma.qtdAnos = anos;
      dma.retorno = 1;
      return dma;
    }
}

/* Devolve a letra "base" de uma vogal acentuada em UTF-8 (á -> a, É -> E ...).
   Para qualquer outro caractere devolve o próprio caractere.
   *tam recebe quantos bytes foram consumidos (1 ou 2). */
static char caractereBase(const char *p, int *tam)
{
  unsigned char a = (unsigned char)p[0];
  unsigned char b;
  char base = 0;

  *tam = 1;
  if (a != 0xC3 || p[1] == '\0') return p[0];

  b = (unsigned char)p[1];
  if (b >= 0x80 && b <= 0x84) base = 'A';        // À Á Â Ã Ä
  else if (b >= 0x88 && b <= 0x8B) base = 'E';   // È É Ê Ë
  else if (b >= 0x8C && b <= 0x8F) base = 'I';   // Ì Í Î Ï
  else if (b >= 0x92 && b <= 0x96) base = 'O';   // Ò Ó Ô Õ Ö
  else if (b >= 0x99 && b <= 0x9C) base = 'U';   // Ù Ú Û Ü
  else if (b >= 0xA0 && b <= 0xA4) base = 'a';   // à á â ã ä
  else if (b >= 0xA8 && b <= 0xAB) base = 'e';   // è é ê ë
  else if (b >= 0xAC && b <= 0xAF) base = 'i';   // ì í î ï
  else if (b >= 0xB2 && b <= 0xB6) base = 'o';   // ò ó ô õ ö
  else if (b >= 0xB9 && b <= 0xBC) base = 'u';   // ù ú û ü

  if (base == 0) return p[0];
  *tam = 2;
  return base;
}

/*
 Q3 = encontrar caracter em texto
 @objetivo
    Pesquisar quantas vezes um determinado caracter ocorre em um texto
 @entrada
    uma string texto, um caracter c e um inteiro que informa se é uma pesquisa Case Sensitive ou não. Se isCaseSensitive = 1, a pesquisa deve considerar diferenças entre maiúsculos e minúsculos.
        Se isCaseSensitive != 1, a pesquisa não deve  considerar diferenças entre maiúsculos e minúsculos.
 @saida
    Um número n >= 0.
 */
int q3(char *texto, char c, int isCaseSensitive)
{
    int qtdOcorrencias = 0, i = 0, tam;
    char alvo = c, atual;

    if (isCaseSensitive != 1 && alvo >= 'A' && alvo <= 'Z')
        alvo = alvo + ('a' - 'A');

    while (texto[i] != '\0')
    {
        atual = caractereBase(&texto[i], &tam); // acentos nas vogais são ignorados
        if (isCaseSensitive != 1 && atual >= 'A' && atual <= 'Z')
            atual = atual + ('a' - 'A');
        if (atual == alvo)
            qtdOcorrencias++;
        i += tam;
    }

    return qtdOcorrencias;
}

/*
 Q4 = encontrar palavra em texto
 @objetivo
    Pesquisar todas as ocorrências de uma palavra em um texto
 @entrada
    uma string texto base (strTexto), uma string strBusca e um vetor de inteiros (posicoes) que irá guardar as posições de início e fim de cada ocorrência da palavra (strBusca) no texto base (texto).
 @saida
    Um número n >= 0 correspondente a quantidade de ocorrências encontradas.
    O vetor posicoes deve ser preenchido com cada entrada e saída correspondente. Por exemplo, se tiver uma única ocorrência, a posição 0 do vetor deve ser preenchido com o índice de início do texto, e na posição 1, deve ser preenchido com o índice de fim da ocorrencias. Se tiver duas ocorrências, a segunda ocorrência será amazenado nas posições 2 e 3, e assim consecutivamente. Suponha a string "Instituto Federal da Bahia", e palavra de busca "dera". Como há uma ocorrência da palavra de busca no texto, deve-se armazenar no vetor, da seguinte forma:
        posicoes[0] = 13;
        posicoes[1] = 16;
        Observe que o índice da posição no texto deve começar ser contado a partir de 1.
        O retorno da função, n, nesse caso seria 1;

 */
int q4(char *strTexto, char *strBusca, int posicoes[30])
{
    int qtdOcorrencias = 0;
    int i = 0, j;
    int pos = 0;           // posição em CARACTERES (um 'á' em UTF-8 tem 2 bytes, mas conta 1)
    int tamBusca = 0;      // tamanho da palavra em bytes
    int charsBusca = 0;    // tamanho da palavra em caracteres

    while (strBusca[tamBusca] != '\0')
    {
        if (((unsigned char)strBusca[tamBusca] & 0xC0) != 0x80)
            charsBusca++;
        tamBusca++;
    }
    if (tamBusca == 0)
        return 0;

    while (strTexto[i] != '\0')
    {
        // byte de continuação de um caractere UTF-8: não é um novo caractere
        if (((unsigned char)strTexto[i] & 0xC0) == 0x80)
        {
            i++;
            continue;
        }
        pos++;

        j = 0;
        while (j < tamBusca && strTexto[i + j] != '\0' && strTexto[i + j] == strBusca[j])
            j++;

        if (j == tamBusca)
        {
            if (qtdOcorrencias * 2 + 1 < 30) // o vetor tem 30 posições
            {
                posicoes[qtdOcorrencias * 2] = pos;
                posicoes[qtdOcorrencias * 2 + 1] = pos + charsBusca - 1;
            }
            qtdOcorrencias++;
            // pula a ocorrência inteira (ocorrências não se sobrepõem)
            i += tamBusca;
            pos += charsBusca - 1;
            continue;
        }
        i++;
    }

    return qtdOcorrencias;
}

/*
 Q5 = inverte número
 @objetivo
    Inverter número inteiro
 @entrada
    uma int num.
 @saida
    Número invertido
 */

int q5(int num)
{
    int negativo = 0, invertido = 0;

    if (num < 0)
    {
        negativo = 1;
        num = -num;
    }

    while (num > 0)
    {
        invertido = invertido * 10 + num % 10;
        num = num / 10;
    }

    return negativo ? -invertido : invertido;
}

/*
 Q6 = ocorrência de um número em outro
 @objetivo
    Verificar quantidade de vezes da ocorrência de um número em outro
 @entrada
    Um número base (numerobase) e um número de busca (numerobusca).
 @saida
    Quantidade de vezes que número de busca ocorre em número base
 */

int q6(int numerobase, int numerobusca)
{
    int qtdOcorrencias = 0;
    long long base = numerobase, busca = numerobusca, p = 10, t;

    if (base < 0) base = -base;
    if (busca < 0) busca = -busca;

    // p = 10^(quantidade de dígitos de busca)
    t = busca;
    while (t >= 10)
    {
        p *= 10;
        t /= 10;
    }

    if (base == 0)
        return busca == 0 ? 1 : 0;

    // olha os últimos dígitos de base; se casar, "consome" a ocorrência (sem sobreposição)
    while (base > 0)
    {
        if (base % p == busca)
        {
            qtdOcorrencias++;
            base /= p;
        }
        else
            base /= 10;
    }

    return qtdOcorrencias;
}

/*
 Q7 = jogo busca palavras
 @objetivo
    Verificar se existe uma string em uma matriz de caracteres em todas as direções e sentidos possíves
 @entrada
    Uma matriz de caracteres e uma string de busca (palavra).
 @saida
    1 se achou 0 se não achou
 */

 int q7(char matriz[8][10], char palavra[5])
 {
     // 8 direções: direita, esquerda, baixo, cima e as 4 diagonais
     int dl[8] = {0, 0, 1, -1, 1, 1, -1, -1};
     int dc[8] = {1, -1, 0, 0, 1, -1, 1, -1};
     int tam = 0, l, c, d, k, nl, nc;

     while (tam < 5 && palavra[tam] != '\0')
         tam++;
     if (tam == 0)
         return 0;

     for (l = 0; l < 8; l++)
     {
         for (c = 0; c < 10; c++)
         {
             for (d = 0; d < 8; d++)
             {
                 for (k = 0; k < tam; k++)
                 {
                     nl = l + dl[d] * k;
                     nc = c + dc[d] * k;
                     if (nl < 0 || nl >= 8 || nc < 0 || nc >= 10 || matriz[nl][nc] != palavra[k])
                         break;
                 }
                 if (k == tam)
                     return 1;
             }
         }
     }
     return 0;
 }



DataQuebrada quebraData(char data[]){
  DataQuebrada dq;
  char sDia[3];
	char sMes[3];
	char sAno[5];
	int i; 

	for (i = 0; data[i] != '/'; i++){
		sDia[i] = data[i];	
	}
	if(i == 1 || i == 2){ // testa se tem 1 ou dois digitos
		sDia[i] = '\0';  // coloca o barra zero no final
	}else {
		dq.valido = 0;
    return dq;
  }  
	

	int j = i + 1; //anda 1 cada para pular a barra
	i = 0;

	for (; data[j] != '/'; j++){
		sMes[i] = data[j];
		i++;
	}

	if(i == 1 || i == 2){ // testa se tem 1 ou dois digitos
		sMes[i] = '\0';  // coloca o barra zero no final
	}else {
		dq.valido = 0;
    return dq;
  }
	

	j = j + 1; //anda 1 cada para pular a barra
	i = 0;
	
	for(; data[j] != '\0'; j++){
	 	sAno[i] = data[j];
	 	i++;
	}

	if(i == 2 || i == 4){ // testa se tem 2 ou 4 digitos
		sAno[i] = '\0';  // coloca o barra zero no final
	}else {
		dq.valido = 0;
    return dq;
  }

  dq.iDia = atoi(sDia);
  dq.iMes = atoi(sMes);
  dq.iAno = atoi(sAno); 

	dq.valido = 1;
    
  return dq;
}