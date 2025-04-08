/*
 * labirinto.c - Funções relacionadas ao labirinto
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

#include "include/labirinto.h"

#include <stdio.h>
#include <stdlib.h>

#include "include/typedefs.h"

char *encontrar_adjacencias(char **matriz, int *tamanho, int linha, int coluna) {
  char *adjacentes = calloc(4, sizeof(char));

  if (adjacentes == NULL) {
    erro(L"Falha ao alocar memória para adjacentes.");
  }

  for (int i = 0; i < 4; ++i) {
    int *pos_adjacente = nova_posicao(linha, coluna, i);
    if (checar_coordenada(tamanho, pos_adjacente)) {
      adjacentes[i] = matriz[pos_adjacente[0]][pos_adjacente[1]];
    }
    free(pos_adjacente);
  }

  return adjacentes;
}

/**
 * Preenche a matriz com os caracteres do arquivo do labirinto, também carrega
 * as posições do jogador e da saída.
 */
void preencher_matriz(Labirinto *labirinto) {
  const int *tamanho = labirinto->tamanho;

  char **linhas   = alocar_matriz(tamanho[0], tamanho[1] * 2 + 1);
  labirinto->modo = 0;

  labirinto->trilha.stack = malloc(tamanho[0] * tamanho[1] * sizeof(int));
  labirinto->trilha.grafo = malloc(tamanho[0] * sizeof(Vertice *));

  for (int i = 0; i < tamanho[0]; ++i) {
    if (!fgets(linhas[i], tamanho[1] * 2 + 1, labirinto->arquivo)) {
      erro(L"Erro ao ler linha do arquivo.");
    }

    labirinto->trilha.grafo[i] = calloc(tamanho[1], sizeof(Vertice));

    int coluna = 0;
    for (int j = 0; j < tamanho[1] * 2 && linhas[i][j] != '\0'; ++j) {
      if (linhas[i][j] != '\n' && linhas[i][j] != ' ') {
        const char caractere = linhas[i][j];

        labirinto->matriz[i][coluna]          = caractere;
        labirinto->matriz_inicial[i][coluna]  = caractere;
        labirinto->trilha.matriz[i][coluna++] = caractere;

        if (caractere == JOGADOR || caractere == SAIDA) {
          int *vetor = caractere == JOGADOR ? labirinto->jogador.posicao_inicial : labirinto->posicao_saida;
          vetor[0]   = i;
          vetor[1]   = coluna - 1;
        }
      }
    }
  }

  limpar_matriz(linhas, tamanho[0]);
}

void matar_jogador(Labirinto *labirinto, char causa) {
  labirinto->matriz[labirinto->jogador.posicao[0]][labirinto->jogador.posicao[1]] = causa;
  imprimir_labirinto(labirinto);
  pausar(1);
  labirinto->jogador.tentativas++;
  restaurar_labirinto(labirinto);
}

void mover_jogador(Labirinto *labirinto, Direcoes direcao) {
  int *pos_jogador   = labirinto->jogador.posicao;
  int *pos_adjacente = nova_posicao(pos_jogador[0], pos_jogador[1], direcao);

  if (pos_adjacente[0] < 0 || pos_adjacente[0] >= labirinto->tamanho[0] || pos_adjacente[1] < 0 || pos_adjacente[1] >= labirinto->tamanho[1]) {
    return;
  }

  char caractere_adjacente = labirinto->matriz[pos_adjacente[0]][pos_adjacente[1]];

  labirinto->matriz[pos_jogador[0]][pos_jogador[1]] = (labirinto->matriz_inicial[pos_jogador[0]][pos_jogador[1]] == INIMIGO) ? '!' : CAMINHO;
  copiar_matriz(pos_adjacente, pos_jogador, sizeof(int[2]));
  labirinto->matriz[pos_adjacente[0]][pos_adjacente[1]] = (caractere_adjacente == SAIDA) ? 'V' : JOGADOR;

  if (labirinto->matriz_inicial[pos_adjacente[0]][pos_adjacente[1]] == INIMIGO) {
    pausar(0.5);
    if ((rand() % 100) > (50 + labirinto->jogador.inimigos_derrotados * 10)) {
      mensagem(labirinto, L"Jogador morto em combate :(");
      matar_jogador(labirinto, '+');
    } else {
      labirinto->jogador.inimigos_derrotados++;
    }
  }
  free(pos_adjacente);
}

void restaurar_labirinto(Labirinto *labirinto) {
  limpar_lateral(labirinto);

  copiar_matriz(labirinto->jogador.posicao_inicial, labirinto->jogador.posicao, 2 * sizeof(int));

  labirinto->jogador.inimigos_derrotados = 0;
  labirinto->trilha.tamanho              = 0;

  copiar_matriz_bidimensional(labirinto->modo ? labirinto->matriz_inicial : labirinto->trilha.matriz, labirinto->matriz,
                              labirinto->tamanho[0], labirinto->tamanho[1]);

  copiar_matriz(labirinto->jogador.posicao_inicial, labirinto->trilha.posicao, 2 * sizeof(int));

  fflush(stdout);
}

int encontrar_direcoes(Labirinto *labirinto, char *permitidos, int *direcoes, int posicao[2]) {
  int direcoes_possiveis = 0;
  char *adjacencias      = encontrar_adjacencias(labirinto->matriz, labirinto->tamanho, posicao[0], posicao[1]);

  for (int i = 0; i < 4; ++i) {
    if (adjacencias[i] && strchr(permitidos, adjacencias[i])) {
      direcoes[direcoes_possiveis++] = i;
    }
  }

  free(adjacencias);
  return direcoes_possiveis;
}

void resolver_tentativa_e_erro(Labirinto *labirinto) {
  labirinto->modo               = 1;
  labirinto->jogador.tentativas = 0;
  restaurar_labirinto(labirinto);

  int resolvido = 0;
  int *direcoes = malloc(4 * sizeof(int));

  while (!resolvido) {
    int *pos_jogador = labirinto->jogador.posicao;

    int direcoes_possiveis = encontrar_direcoes(labirinto, ".%$", direcoes, pos_jogador);

    if (!direcoes_possiveis) {
      mensagem(labirinto, L"Sem movimentos válidos! Reiniciando...");
      matar_jogador(labirinto, '?');
      continue;
    }

    mover_jogador(labirinto, direcoes[rand() % direcoes_possiveis]);
    atualizar_interface(labirinto);
    if (comparar_coordenadas(pos_jogador, labirinto->posicao_saida)) {
      resolvido = 1;
    }
    pausar(0.1);
  }
  free(direcoes);
}

void mover_trilha(Labirinto *labirinto, int direcao) {
  int *pos_trilha = labirinto->trilha.posicao;

  int *novo_pos = nova_posicao(pos_trilha[0], pos_trilha[1], direcao);

  char caractere_adjacente = labirinto->matriz[novo_pos[0]][novo_pos[1]];
  if (caractere_adjacente != SAIDA && caractere_adjacente != INIMIGO) {
    labirinto->matriz[novo_pos[0]][novo_pos[1]] = '-';
  }

  copiar_matriz(novo_pos, pos_trilha, sizeof(int[2]));
  labirinto->trilha.stack[labirinto->trilha.tamanho++] = direcao;
  free(novo_pos);
}

void retroceder_trilha(Labirinto *labirinto, int ignorando_inimigos) {
  if (labirinto->trilha.tamanho == 0) {
    return;
  }

  int direcao_anterior = labirinto->trilha.stack[--labirinto->trilha.tamanho];
  int direcao          = inverter_posicao(direcao_anterior);

  int *pos_trilha   = labirinto->trilha.posicao;
  int *pos_anterior = nova_posicao(pos_trilha[0], pos_trilha[1], direcao);

  char caractere = labirinto->matriz_inicial[pos_trilha[0]][pos_trilha[1]];

  labirinto->matriz[pos_trilha[0]][pos_trilha[1]] = (caractere == INIMIGO && ignorando_inimigos) ? '}' : (caractere == SAIDA ? '/' : '{');

  copiar_matriz(pos_anterior, pos_trilha, sizeof(int[2]));
  free(pos_anterior);
}

void resolver_diretamente(Labirinto *labirinto) {
  labirinto->modo               = 0;
  labirinto->jogador.tentativas = 0;

  int ignorando_inimigos = 0;
  restaurar_labirinto(labirinto);

  copiar_matriz_bidimensional(labirinto->matriz_inicial, labirinto->matriz, labirinto->tamanho[0], labirinto->tamanho[1]);

  copiar_matriz_bidimensional(labirinto->matriz_inicial, labirinto->trilha.matriz, labirinto->tamanho[0], labirinto->tamanho[1]);

  int resolvido   = 0;
  int achou_saida = 0;

  int *direcoes = malloc(4 * sizeof(int));

  int *pos_trilha = labirinto->trilha.posicao;
  int *pos_saida  = labirinto->posicao_saida;

  while (!resolvido) {
    atualizar_interface(labirinto);

    if (!achou_saida && comparar_coordenadas(pos_trilha, pos_saida)) {
      achou_saida = 1;
      mensagem(labirinto, L"Saída encontrada! Movendo o jogador...");
      copiar_matriz_bidimensional(labirinto->matriz, labirinto->trilha.matriz, labirinto->tamanho[0], labirinto->tamanho[1]);
    }

    int direcoes_possiveis;

    if (achou_saida) {
      int *pos_jogador = labirinto->jogador.posicao;

      if (comparar_coordenadas(pos_jogador, pos_saida)) {
        resolvido = 1;
        continue;
      }

      direcoes_possiveis = encontrar_direcoes(labirinto, "-}$", direcoes, pos_jogador);

      if (direcoes_possiveis) {
        mover_jogador(labirinto, direcoes[0]);
      }

      pausar(0.1);
      continue;
    }

    mensagem(labirinto, L"Realizando busca em profundidade");

    direcoes_possiveis = encontrar_direcoes(labirinto, ignorando_inimigos ? ".%$/" : ".$", direcoes, pos_trilha);

    pausar(0.05);

    if (!direcoes_possiveis) {
      if (labirinto->trilha.tamanho) {
        retroceder_trilha(labirinto, ignorando_inimigos);
      } else if (!ignorando_inimigos) {
        restaurar_labirinto(labirinto);
        ignorando_inimigos = 1;
      } else {
        mensagem(labirinto, L"Sem movimentos válidos!");
        pausar(5);
        break;
      }
      continue;
    }

    mover_trilha(labirinto, direcoes[0]);
  }
  free(direcoes);
}

void resolver_a_star(Labirinto *labirinto) {
  restaurar_labirinto(labirinto);

  copiar_matriz_bidimensional(labirinto->matriz_inicial, labirinto->matriz, labirinto->tamanho[0], labirinto->tamanho[1]);

  copiar_matriz_bidimensional(labirinto->matriz_inicial, labirinto->trilha.matriz, labirinto->tamanho[0], labirinto->tamanho[1]);

  for (int i = 0; i < labirinto->tamanho[0]; ++i) {
    memset(labirinto->trilha.matriz[i], 0, labirinto->tamanho[1] * sizeof(char));
  }

  const int linhas  = labirinto->tamanho[0];
  const int colunas = labirinto->tamanho[1];

  Vertice *stack = calloc(linhas * colunas, sizeof(Vertice));

  Vertice vertice_inicial = { .posicao = { labirinto->trilha.posicao[0], labirinto->trilha.posicao[1] },
                              .origem  = { -1, -1 },
                              .custo   = dist_manhattan(labirinto->trilha.posicao, labirinto->posicao_saida) };

  stack[labirinto->trilha.tamanho++] = vertice_inicial;

  char *caminhos = "-$";

  while (labirinto->trilha.tamanho > 0) {
    atualizar_interface(labirinto);

    int menor_custo = 0;

    for (int i = 1; i < labirinto->trilha.tamanho; ++i) {
      if (stack[i].custo < stack[menor_custo].custo) {
        menor_custo = i;
      }
    }

    Vertice vertice_topo = stack[menor_custo];
    stack[menor_custo]   = stack[--labirinto->trilha.tamanho];

    if (labirinto->trilha.matriz[vertice_topo.posicao[0]][vertice_topo.posicao[1]]) {
      continue;
    }

    labirinto->trilha.matriz[vertice_topo.posicao[0]][vertice_topo.posicao[1]] = 1;

    if (comparar_coordenadas(vertice_topo.posicao, labirinto->posicao_saida)) {
      int posicao_atual[2] = { vertice_topo.origem[0], vertice_topo.origem[1] };

      copiar_matriz_bidimensional(labirinto->matriz_inicial, labirinto->trilha.matriz, linhas, colunas);

      while (!comparar_coordenadas(posicao_atual, labirinto->trilha.posicao)) {
        if (labirinto->matriz_inicial[posicao_atual[0]][posicao_atual[1]] != INIMIGO) {
          labirinto->trilha.matriz[posicao_atual[0]][posicao_atual[1]] = '-';
        }

        atualizar_interface(labirinto);

        Vertice vertice_atual = labirinto->trilha.grafo[posicao_atual[0]][posicao_atual[1]];
        copiar_matriz(vertice_atual.origem, posicao_atual, 2 * sizeof(int));
      }

      copiar_matriz_bidimensional(labirinto->trilha.matriz, labirinto->matriz, linhas, colunas);

      int *direcoes          = malloc(4 * sizeof(int));
      int direcoes_possiveis = 0;

      while (!comparar_coordenadas(labirinto->jogador.posicao, labirinto->posicao_saida)) {
        direcoes_possiveis = encontrar_direcoes(labirinto, caminhos, direcoes, labirinto->jogador.posicao);

        if (!direcoes_possiveis) {
          if (caminhos[0] != '%') {
            caminhos = "%";
            continue;
          }
          mensagem(labirinto, L"Sem movimentos válidos!");
          pausar(2);
        }

        caminhos = "-$";
        mover_jogador(labirinto, direcoes[0]);
        atualizar_interface(labirinto);
        pausar(0.1);
      }

      free(direcoes);
      break;
    }

    for (int i = 0; i < 4; ++i) {
      int *pos_adjacente = nova_posicao(vertice_topo.posicao[0], vertice_topo.posicao[1], i);

      if (!checar_coordenada(labirinto->tamanho, pos_adjacente) || parede(labirinto->matriz[pos_adjacente[0]][pos_adjacente[1]]) ||
          labirinto->trilha.matriz[pos_adjacente[0]][pos_adjacente[1]]) {
        free(pos_adjacente);
        continue;
      }

      char caractere = labirinto->matriz[pos_adjacente[0]][pos_adjacente[1]];
      int peso       = vertice_topo.peso + 1 + (inimigo(caractere) ? 10 : 0);

      Vertice vertice_adjacente = { .peso    = peso,
                                    .custo   = peso + dist_manhattan(pos_adjacente, labirinto->posicao_saida),
                                    .posicao = { pos_adjacente[0], pos_adjacente[1] },
                                    .origem  = { vertice_topo.posicao[0], vertice_topo.posicao[1] } };

      stack[labirinto->trilha.tamanho++] = vertice_adjacente;

      if (!comparar_coordenadas(vertice_adjacente.origem, vertice_inicial.origem) && !inimigo(caractere)) {
        labirinto->matriz[pos_adjacente[0]][pos_adjacente[1]] = '-';
      }

      labirinto->trilha.grafo[pos_adjacente[0]][pos_adjacente[1]] = vertice_adjacente;
      free(pos_adjacente);
    }
    pausar(0.05);
  }
  free(stack);
}
