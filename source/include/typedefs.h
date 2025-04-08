/*
 * typedefs.h - Arquivo de tipos usados pelo programa
 *
 * Copyright (C) 2025 Iarlo Bruno Damasceno De Sá <iarlo.sa@dcomp.ufs.br>
 * Copyright (C) 2025 Karlus Dória Da Silva Júnior <karlus@academico.ufs.br>
 * Copyright (C) 2025 Jaqueline Nascimento <jaquelinenascimento@academico.ufs.br>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdio.h>

#define INICIO L"\033[H\033[J"
#define ABAIXO_TITULO L"\033[6;0H"

typedef enum { ACIMA, ABAIXO, ESQUERDA, DIREITA } Direcoes;

typedef enum {
  PAREDE        = '#',
  ESPACO        = ' ',
  CAMINHO       = '*',
  CAMINHO_LIVRE = '.',
  INIMIGO       = '%',
  JOGADOR       = '@',
  SAIDA         = '$',
  TRILHA        = '-'
} Celulas;

typedef struct {
  int posicao[2];
  int origem[2];
  int peso;
  int custo;
} Vertice;

typedef struct {
  char **matriz;
  int *stack;
  int posicao[2];
  int tamanho;
  Vertice **grafo;
} Trilha;

typedef struct {
  int posicao[2];
  int posicao_inicial[2];
  int inimigos_derrotados;
  int tentativas;
} Jogador;

typedef struct {
  char nome[50];
  FILE *arquivo;
  int modo;
  int tamanho[2];
  char **matriz;
  char **matriz_inicial;
  int menu;
  Trilha trilha;
  Jogador jogador;
  int posicao_saida[2];
} Labirinto;

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

#define parede(x) ((x) == PAREDE)
#define inimigo(x) ((x) == INIMIGO)
#define trilha(x) ((x) == TRILHA)


#endif