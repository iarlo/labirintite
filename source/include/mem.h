/*
 * mem.h - Protótipos das funções relacionadas ao gerenciamento de memória
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

#ifndef MEM_H
#define MEM_H

#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Libera iterativamente a memoria alocada por uma matriz bidimensional.
 * Usado para limpar a memória da matriz do labirinto, adjacencias, etc...
 *
 * @param matriz a matriz que será removida
 * @param linhas o número de linhas da matriz
 */
void limpar_matriz(char **matriz, int linhas);

/**
 * Aloca uma matriz dinâmica para o labirinto com base nas linhas e colunas
 * extraídas do arquivo do labirinto. Cria uma matriz bidimensional de char.
 *
 * @param linhas - O número de linhas da matriz.
 * @param colunas - O número de colunas da matriz.
 * @return Um ponteiro para a matriz alocada.
 */
char **alocar_matriz(int linhas, int colunas);

/**
 * Copia um vetor unidimensional.
 *
 * @param origem - O ponteiro para o vetor que vai ser copiado.
 * @param destino - O ponteiro para o vetor que receberá a cópia.
 * @param tamanho - O tamanho do bloco de memória que vai ser copiado.
 */
void copiar_matriz(void *origem, void *destino, size_t tamanho);

/**
 * Copia a matriz origem para a matriz destino.
 *
 * LEMBRETE: As matrizes devem ter as mesmas dimensões
 *
 * @param origem - A matriz que vai ser copiada
 * @param destino - A matriz que receberá a cópia
 * @param linhas - A quantidade de linhas da matriz
 * @param colunas - A quantidade de colunas da matriz
 */
void copiar_matriz_bidimensional(char **origem, char **destino, int linhas, int colunas);

/**
 * Fecha o arquivo do labirinto e libera a memória alocada para as matrizes
 * do labirinto e do labirinto inicial.
 *
 * @param labirinto Uma instância do labirinto.
 */
void fechar_labirinto(Labirinto *labirinto);

#endif