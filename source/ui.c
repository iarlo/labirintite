/*
 * ui.h - Funções de interface
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
#include "include/ui.h"
#include "include/util.h"

const char *lista_de_opcoes[] = { "Resolver diretamente", "Tentativa e erro", "Salvar em um arquivo", "Sair" };

const char *lista_de_opcoes_submenu[] = { "Busca em profundidade", "A-Estrela", "Voltar" };
int opcao_selecionada                 = 0;
int menu_selecionado                  = 0;

void imprimir_titulo() {
  const wchar_t *titulo = INICIO BLU LR"(               __         __              ___      ___    ___
|       /\    |__)   |   |__)   |   |\ |   |    |   |    |__
|___   /--\   |__)   |   |  \   |   | \|   |    |   |    |___)";

  const wchar_t *subtitulo = BLUB L"\e[1m\e[38;5;255m    O resolvedor de labirintos mais tonto que você já viu    ";

  wprintf(MOVE_TOP_LEFT CLEAN_LINE_END L"%ls\n\n%ls" COLOR_RESET, titulo, subtitulo);
  fflush(stdout);
}

void imprimir_labirinto(Labirinto *labirinto) {
  wprintf(ABAIXO_TITULO);
  for (int i = 0; i < labirinto->tamanho[0]; i++)
    for (int j = 0; j < labirinto->tamanho[1]; j++) {
      wchar_t caractere      = labirinto->matriz[i][j];
      wchar_t novo_caractere = mudar_caractere(labirinto, i, j);
      wchar_t lacuna         = preencher_lacuna(labirinto, i, j);

      colorir_caractere(caractere);
      wprintf(L"%c%lc%lc" RESET, j % labirinto->tamanho[1] ? '\0' : '\n', novo_caractere, lacuna);
    }
}

void imprimir_opcoes(Labirinto *labirinto) {
  const int coluna = labirinto->tamanho[1] * 2 + 5;
  mover_cursor(7, coluna);
  wprintf(L"Selecione uma opção:");
  for (int i = 0; i < 4; i++) {
    mover_cursor(9 + i, coluna);
    wprintf(L"\033[0m%s  %s\033[0m\n", opcao_selecionada == i ? "\033[48;5;236m>" : "\033[0m ", lista_de_opcoes[i]);
  }
}

void imprimir_submenu(Labirinto *labirinto) {
  const int coluna = labirinto->tamanho[1] * 2 + 5;
  mover_cursor(7, coluna);
  wprintf(L"Selecione uma opção:");
  for (int i = 0; i < 3; i++) {
    mover_cursor(9 + i, coluna);
    wprintf(L"\033[0m%s  %s\033[0m\n", opcao_selecionada == i ? "\033[48;5;236m>" : "\033[0m ", lista_de_opcoes_submenu[i]);
  }
}

void imprimir_status(Labirinto *labirinto) {
  const int coluna              = labirinto->tamanho[1] * 2 + 5;
  const wchar_t *const status[] = { BLU L"\033[8;%dHTentativas: %d" RESET, BLU L"\033[9;%dHInimigos derrotados: %d" RESET };

  wprintf(status[0], coluna, labirinto->jogador.tentativas);
  wprintf(status[1], coluna, labirinto->jogador.inimigos_derrotados);
  mover_cursor(11, labirinto->tamanho[1] * 2 + 5);
}

void atualizar_interface(Labirinto *labirinto) {
  imprimir_labirinto(labirinto);
  imprimir_status(labirinto);
  fflush(stdout);
}

void selecionar_opcao(Labirinto *labirinto) {
  switch (opcao_selecionada) {
  case 0:
    if (menu_selecionado) {
      limpar_tela();
      menu_selecionado = 0;
      resolver_diretamente(labirinto);
      limpar_lateral(labirinto);
      break;
    }
    limpar_lateral(labirinto);
    menu_selecionado = 1;
    break;
  case 1:
    if (menu_selecionado) {
      menu_selecionado  = 0;
      opcao_selecionada = 0;
      resolver_a_star(labirinto);
      limpar_lateral(labirinto);
      break;
    }
    limpar_tela();
    resolver_tentativa_e_erro(labirinto);
    limpar_lateral(labirinto);
    break;
  case 2:
    if (menu_selecionado) {
      menu_selecionado  = 0;
      opcao_selecionada = 0;
      limpar_lateral(labirinto);
      break;
    }
    limpar_tela();
    restaurar_terminal();
    wprintf(ABAIXO_TITULO);
    salvar_arquivo(labirinto);
    exit(0);
    break;
  case 3:
    limpar_tela();
    const wchar_t *creditos = BLU LR"(
                              * (C) 2025 Iarlo Bruno Damasceno De Sá
                              * (C) 2025 Karlus Dória Da Silva Júnior
                              * (C) 2025 Jaqueline Nascimento
                       )";
    wprintf(L"\033[%d;30H\033[?25l%ls", 10, creditos);
    wprintf(ABAIXO_TITULO);
    ufs_logo();
    fechar_labirinto(labirinto);
    terminar_programa(0);
    break;
  }
}

void imprimir_tela(Labirinto *labirinto) {
  imprimir_titulo();

  if (labirinto->arquivo == NULL) {
    wprintf(ABAIXO_TITULO L"\nDigite o nome do arquivo: ");
    scanf("%49s", labirinto->nome);
    labirinto->arquivo = abrir_arquivo(labirinto->nome);
  }

  imprimir_titulo();
  labirinto->menu = 1;
  ler_arquivo(labirinto);
  preencher_matriz(labirinto);
  configurar_terminal();
  imprimir_labirinto(labirinto);
  wprintf(L"\n\n\033[s");
  imprimir_opcoes(labirinto);
  while (1) {
    int tecla = ler_tecla_pressionada();

    switch (tecla) {
    case ACIMA: opcao_selecionada = max(0, opcao_selecionada - 1); break;
    case ABAIXO: opcao_selecionada = min(3, opcao_selecionada + 1); break;
    case '\n': selecionar_opcao(labirinto); break;
    }

    if (!menu_selecionado) {
      imprimir_opcoes(labirinto);
      continue;
    }
    imprimir_submenu(labirinto);
  }
}
