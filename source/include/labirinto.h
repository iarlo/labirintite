/*
 * labirinto.c - Protótipos das funções relacionadas ao labirinto
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

#ifndef LABIRINTO_H
#define LABIRINTO_H

#include "mem.h"
#include "typedefs.h"
#include "ui.h"
#include <stdio.h>

/**
 * Encontra as adjacências de um caractere na matriz do labirinto.
 *
 * @param matriz A matriz contendo o labirinto.
 * @param tamanho O tamanho da matriz.
 * @param linha A linha do caractere.
 * @param coluna A coluna do caractere.
 *
 * @return Uma matriz de 4 caracteres, representando as adjacências na ordem
 * acima, abaixo, esquerda e direita.
 */
char *encontrar_adjacencias(char **matriz, int *tamanho, int linha, int coluna);

/**
 * Preenche a matriz com os caracteres do arquivo do labirinto, também carrega
 * as posições do jogador e da saída.
 *
 * @param labirinto A instância do labirinto.
 */
void preencher_matriz(Labirinto *labirinto);

/**
 * Move o jogador para uma direção específica.
 *
 * @param labirinto A instância do labirinto.
 * @param direcao A direção para a qual o jogador deve ser movido.
 */
void mover_jogador(Labirinto *labirinto, Direcoes direcao);

/**
 * Move a trilha para uma direção específica.
 *
 * @param labirinto A instância do labirinto.
 * @param direcao A direção para a qual a trilha deve ser movida.
 */
void mover_trilha(Labirinto *labirinto, int direcao);

/**
 * Retrocede a trilha um passo, movendo para a casa anterior. Usado apenas em busca em profundidade
 *
 * Se o segundo parâmetro for verdadeiro, a função tenta procurar um caminho sem
 * inimigos
 *
 * @param labirinto A instância do labirinto
 * @param ignorando_inimigos Autoexplicativo.
 */
void retroceder_trilha(Labirinto *labirinto, int ignorando_inimigos);

/**
 * Encontra as direções que o jogador pode ir, considerando as paredes e
 * inimigos.
 *
 * @param labirinto A instância do labirinto.
 * @param permitidos Caracteres que o jogador pode passar por cima.
 * @param direcoes Matriz para guardar as direções encontradas.
 * @param posicao A posição do jogador na matriz.
 *
 * @return A quantidade de direções encontradas.
 */
int encontrar_direcoes(Labirinto *labirinto, char *permitidos, int *direcoes, int posicao[2]);

/**
 * Tenta resolver o labirinto por tentativa e erro.
 *
 * @param labirinto A instância do labirinto.
 */
void resolver_tentativa_e_erro(Labirinto *labirinto);

/**
 * Resolve o labirinto com uma busca em profundidade.
 *
 * @param labirinto A instância do labirinto.
 */
void resolver_diretamente(Labirinto *labirinto);

/**
 * Resolve o labirinto com A*.
 *
 * @param labirinto A instância do labirinto.
 */
void resolver_a_star(Labirinto *labirinto);

/**
 * Restaura o labirinto para o estado inicial, removendo todas as alterações
 * feitas pelo jogador.
 *
 * @param labirinto A instância do labirinto.
 */
void restaurar_labirinto(Labirinto *labirinto);

#endif
