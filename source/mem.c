/*
 * mem.c - Funções relacionadas a alocação e gerenciamento de memória
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

#include "include/mem.h"

void limpar_matriz(char **matriz, int linhas) {
  for (int i = 0; i < linhas; ++i)
    free(matriz[i]);

  free(matriz);
}

char **alocar_matriz(int linhas, int colunas) {
  char **matriz = malloc(linhas * sizeof(char *));
  if (!matriz)
    erro(L"Falha ao alocar linhas da matriz.");

  for (int i = 0; i < linhas; ++i) {
    matriz[i] = malloc(colunas * sizeof(char));
    if (!matriz[i]) {
      limpar_matriz(matriz, i);
      erro(L"Falha ao alocar colunas da matriz.");
    }
    memset(matriz[i], 0, colunas * sizeof(char));
  }

  return matriz;
}

void copiar_matriz(void *origem, void *destino, size_t tamanho) {
  memcpy(destino, origem, tamanho);
}

void copiar_matriz_bidimensional(char **origem, char **destino, int linhas, int colunas) {
  for (int i = 0; i < linhas; ++i)
    copiar_matriz(origem[i], destino[i], colunas * sizeof(char));
}

void fechar_labirinto(Labirinto *labirinto) {
  const int linhas = labirinto->tamanho[0];
  fclose(labirinto->arquivo);
  limpar_matriz(labirinto->matriz, linhas);
  limpar_matriz(labirinto->matriz_inicial, linhas);
  limpar_matriz(labirinto->trilha.matriz, linhas);
  limpar_matriz((char **)labirinto->trilha.grafo, linhas);
  free(labirinto->trilha.stack);
}