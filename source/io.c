/*
 * labirinto.c - Funções de entrada e saída de dados
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

#include "include/io.h"
#include "include/os.h"
#include "include/util.h"

FILE *abrir_arquivo(const char *nome) {
  FILE *arquivo = fopen(nome, "r");

  if (arquivo == NULL)
    erro(L"Arquivo não encontrado");

  return arquivo;
}

void ler_arquivo(Labirinto *labirinto) {
  int *tamanho = labirinto->tamanho;
  char primeira_linha[10];

  if (fgets(primeira_linha, sizeof primeira_linha, labirinto->arquivo) == NULL)
    erro(L"Erro ao ler o tamanho do labirinto.");

  if (sscanf(primeira_linha, "%d %d", &tamanho[0], &tamanho[1]) != 2)
    erro(L"Tamanho do labirinto inválido.");

  if (tamanho[0] < 2 || tamanho[1] < 2 || tamanho[0] > 20 || tamanho[1] > 20)
    erro(L"Tamanho do labirinto inválido. O labirinto deve ter entre 2 e 20 "
         L"linhas e colunas.");

  labirinto->matriz         = alocar_matriz(tamanho[0], tamanho[1]);
  labirinto->matriz_inicial = alocar_matriz(tamanho[0], tamanho[1]);
  labirinto->trilha.matriz  = alocar_matriz(tamanho[0], tamanho[1]);
}

// TODO Tirar o scanf, está quebrando o principio de responsabilidade única
void salvar_arquivo(const Labirinto *labirinto) {
  char nome_arquivo[100];

  wprintf(ABAIXO_TITULO L"\nDigite o nome do arquivo: ");
  scanf("%99s", nome_arquivo);

  FILE *arquivo = fopen(nome_arquivo, "w");
  if (!arquivo)
    erro(L"Erro ao salvar o arquivo.");

  fprintf(arquivo, "%d %d\n", labirinto->tamanho[0], labirinto->tamanho[1]);

  for (int i = 0; i < labirinto->tamanho[0]; i++)
    for (int j = 0; j < labirinto->tamanho[1]; j++) {
      fputc(labirinto->matriz[i][j] == '/' || labirinto->matriz[i][j] == '{' ? CAMINHO_LIVRE :
            labirinto->matriz[i][j] == '}'                                   ? INIMIGO :
                                                                               labirinto->matriz[i][j],
            arquivo);
      fputc(j < labirinto->tamanho[1] - 1 ? ' ' : '\n', arquivo);
    }

  fclose(arquivo);
}
