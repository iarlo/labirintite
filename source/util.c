/*
 * util.c - Funções auxiliares
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

#include "include/os.h"
#include "include/util.h"
#include <locale.h>
#include <time.h>
#ifdef _WIN32
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

wchar_t mapa_binario[16] = {
  L'\x2501', L'\x2501', L'\x2501', L'\x2501', L'\x2503', L'\x250F', L'\u2513', L'\x2533',
  L'\x2503', L'\x2517', L'\x251B', L'\x253B', L'\x2503', L'\x2523', L'\x252b', L'\x254B',
};

wchar_t mapa_binario_trilha[16] = {
  L'\x2500', L'\x2500', L'\x2500', L'\x2500', L'\x2502', L'\x250C', L'\x2510', L'\x252C',
  L'\x2502', L'\x2514', L'\x2518', L'\x2534', L'\x2502', L'\x251C', L'\x2524', L'\x253C',
};

wchar_t mapa_caracteres_estaticos[] = { CAMINHO_LIVRE, L'\x22C5', '/', L'\x22C5', '{', L'\x22C5', SAIDA, L'\x2691', 0, 0 };

Mapa_Cores mapa_cores[] = { { PAREDE, L"\e[0;94m" }, { TRILHA, L"\e[0;32m" },  { INIMIGO, L"\e[38;5;196m" },
                            { '}', L"\e[0;31m" },    { JOGADOR, L"\e[0;33m" }, { 0, 0 } };

void erro(const wchar_t *mensagem) {
  wprintf(RED L"\n[ERRO]: %ls\n\n" COLOR_RESET, mensagem);
  return exit(1);
}

int *nova_posicao(int linha, int coluna, Direcoes direcao) {
  const int dx[] = { -1, 1, 0, 0 };
  const int dy[] = { 0, 0, -1, 1 };

  int *pos = malloc(2 * sizeof(int));
  if (pos == NULL) {
    erro(L"Erro ao alocar memória");
  }

  int temp[2] = { linha + dx[direcao], coluna + dy[direcao] };
  copiar_matriz(temp, pos, sizeof(int[2]));
  return pos;
}

Direcoes inverter_posicao(Direcoes direcao) {
  return (direcao % 2 == 0) ? direcao + 1 : direcao - 1;
}

void limpar_tela() {
  wprintf(L"\033[7;1H\033[J");
}

void limpar_lateral(Labirinto *labirinto) {
  const int coluna = labirinto->tamanho[1] * 2 + 5;
  for (int i = 6; i <= max(labirinto->tamanho[0], 15); ++i) {
    wprintf(L"\033[%d;%dH", i, coluna);
    limpar_cursor();
  }
  fflush(stdout);
}

void mensagem(Labirinto *labirinto, wchar_t *mensagem) {
  const int coluna = labirinto->tamanho[1] * 2 + 5;
  mover_cursor(max(14, labirinto->tamanho[0]), coluna);
  wprintf(CLEAN_LINE);
  wprintf(HWHT L"%ls", mensagem);
  fflush(stdout);
}

int dist_manhattan(int ponto_atual[2], int destino[2]) {
  return abs(ponto_atual[0] - destino[0]) + abs(ponto_atual[1] - destino[1]);
}

int gerar_index_binaria(Labirinto *labirinto, char *caracteres, int linha, int coluna) {
  int index         = 0;
  char *adjacencias = encontrar_adjacencias(labirinto->matriz, labirinto->tamanho, linha, coluna);
  for (int i = 0; i < 4; ++i) {
    index |= (adjacencias[i] && strchr(caracteres, adjacencias[i])) << (3 - i);
  }
  free(adjacencias);
  return index;
}

void colorir_caractere(wchar_t caractere) {
  for (int i = 0; mapa_cores[i].caractere != 0; ++i) {
    if (mapa_cores[i].caractere == caractere) {
      wprintf(L"%ls", mapa_cores[i].cor);
    }
  }
}

wchar_t preencher_lacuna(Labirinto *labirinto, int linha, int coluna) {
  if (coluna >= labirinto->tamanho[1] - 1) {
    return L'\0';
  }

  wchar_t caractere         = labirinto->matriz[linha][coluna];
  wchar_t proximo_caractere = labirinto->matriz[linha][coluna + 1];

  if (parede(caractere) && parede(proximo_caractere)) {
    return L'\x2501';
  }

  if (trilha(caractere) && trilha(proximo_caractere)) {
    return L'\x2500';
  }

  return ESPACO;
}

wchar_t buscar_no_mapa(char caractere, wchar_t mapa[]) {
  for (int i = 0; mapa[i] != 0; i += 2) {
    if (mapa[i] == caractere) {
      return mapa[i + 1];
    }
  }

  return caractere;
}

wchar_t mudar_caractere(Labirinto *labirinto, int linha, int coluna) {
  char caractere = labirinto->matriz[linha][coluna];

  if (parede(caractere) || trilha(caractere)) {
    wchar_t *mapa    = parede(caractere) ? mapa_binario : mapa_binario_trilha;
    char *caracteres = parede(caractere) ? "#" : "-@";
    return mapa[gerar_index_binaria(labirinto, caracteres, linha, coluna)];
  }

  for (int i = 0; mapa_caracteres_estaticos[i] != 0; i += 2) {
    if (mapa_caracteres_estaticos[i] == caractere) {
      return mapa_caracteres_estaticos[i + 1];
    }
  }

  return caractere;
}

void pausar(float segundos) {
#ifdef _WIN32
  Sleep(segundos * 1000);
#else
  usleep(segundos * 1000 * 1000);
#endif
}

void mover_cursor(int linha, int coluna) {
  wprintf(L"\033[%d;%dH", linha, coluna);
}

void limpar_cursor() {
  wprintf(L"\033[K");
}

/**
 * Imprime a logo da UFS no terminal.
 * É impresso com cor azul, e usado na tela de saída
 */
void ufs_logo() {
  const wchar_t *ufs_logo_str = BLU LR"(
               ++++++++       
              ++       ++     
              +   +++++++     
   +++++ ++++++   ++++++++++  
   ++  + ++ +++   ++++    ++  
   ++  + ++ +++   +++  ++++   
   ++  ++++ +++   +++  +++++  
    ++     ++++   ++++     +++
     +++++++  +++++  ++++++  +
                    +++++++  +
                   +++      +
                   +++++++++  
    )";
  wprintf(L"%ls", ufs_logo_str);
}

void terminar_programa(int sig) {
  (void)sig;
  restaurar_terminal();
  wprintf(L"\033[?25h\033[0m\033[u");
  fflush(stdout);
  exit(0);
}

int comparar_coordenadas(int coord1[2], int coord2[2]) {
  return coord1[0] == coord2[0] && coord1[1] == coord2[1];
}

int checar_coordenada(int tamanho[2], int coordenada[2]) {
  return coordenada[0] >= 0 && coordenada[0] < tamanho[0] && coordenada[1] >= 0 && coordenada[1] < tamanho[1];
}
