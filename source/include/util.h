/*
 * util.h - Protótipos das funções auxiliares
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

#ifndef UTIL_H
#define UTIL_H

#include "ansi-color-codes.h"
#include "labirinto.h"
#include "typedefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

/**
 * Imprime uma mensagem de erro e encerra o programa.
 *
 * @param mensagem A mensagem de erro que vai aparecer.
 */
void erro(const wchar_t *mensagem);

/**
 * Cria um novo vetor de inteiros com 2 posições e o preenche com as informações de uma nova posição (linha e coluna) baseada na
 * direção recebida.
 *
 * @param linha A linha atual.
 * @param coluna A coluna atual.
 * @param direcao A direção que deseja ir.
 * @return Um ponteiro para o vetor criado.
 */
int *nova_posicao(int linha, int coluna, Direcoes direcao);

/**
 * Retorna a direção oposta. Sabendo que
 *
 * 0 = acima
 * 1 = abaixo
 * 2 = esquerda
 * 3 = direita
 *
 * A função retorna
 *
 * 0 = baixo
 * 1 = acimas
 * 2 = direita
 * 3 = esquerda
 *
 * @param direcao A direção original.
 * @return A direção oposta.
 */
Direcoes inverter_posicao(Direcoes direcao);

/**
 * Limpa a tela.
 *
 * Apenas uma chamada pra printf com ANSI escape codes pra limpar a tela.
 */
void limpar_tela();

/**
 * A função percorre as adjacências de uma determinada linha e coluna num
 * labirinto e gera um index.
 *
 * @param labirinto A instância do labirinto.
 * @param caracteres Os caracteres que devem ser verificados.
 * @param linha A linha do caractere.
 * @param coluna A coluna do caractere.
 * @return O index gerado.
 */
int gerar_index_binaria(Labirinto *labirinto, char *caracteres, int linha, int coluna);

/**
 * A função busca um caractere num vetor de pares key-value e retorna o
 * valor correspondente.
 *
 * @param caractere O caractere a ser buscado.
 * @param mapa O vetor de pares.
 * @return O valor adjacente ao caractere.
 */
wchar_t buscar_no_mapa(char caractere, wchar_t mapa[]);

/**
 * Colora um caractere de acordo com as cores definidas no sistema.
 *
 * A função busca um caractere num vetor de pares key-value e imprime o
 * caractere com a cor correspondente.
 *
 * @param caractere O caractere a ser colorido.
 */
void colorir_caractere(wchar_t caractere);

/**
 * Preenche lacunas entre paredes.
 *
 * A função verifica se os caracteres da esquerda e da direita forem
 * paredes e, se sim, adiciona uma parede na horizontal entre elas.
 *
 * @param labirinto A instância do labirinto.
 * @param linha A linha do caractere.
 * @param coluna A coluna do caractere.
 * @return O caractere preenchido.
 */
wchar_t preencher_lacuna(Labirinto *labirinto, int linha, int coluna);

/**
 * Muda um caractere de um labirinto de acordo com as paredes ao redor.
 *
 * Se o caractere for uma parede, o algoritmo verifica quais paredes existem
 * ao redor e substitui o caractere por um caractere especial que represente
 * melhor as paredes. Caso contrário, o caractere permanece o mesmo.
 *
 * @param labirinto A instância do labirinto
 * @param linha A linha do caractere
 * @param coluna A coluna do caractere
 * @return O caractere modificado
 */
wchar_t mudar_caractere(Labirinto *labirinto, int linha, int coluna);

/**
 * Calcula a distância de manhattan entre dois pontos no labirinto.
 *
 * @param ponto_atual O ponto atual na matriz do labirinto.
 * @param destino O ponto de destino na matriz do labirinto.
 * @return A distância de manhattan entre os dois pontos.
 */
int dist_manhattan(int ponto_atual[2], int destino[2]);

/**
 * Compara se dois pontos do labirinto são iguais.
 *
 * @param coord1 As coordenadas do primeiro ponto.
 * @param coord2 As coordenadas do segundo ponto.
 * @return Boolean
 */
int comparar_coordenadas(int coord1[2], int coord2[2]);

/**
 * Pausa o programa por um determinado tempo em segundos.
 *
 * @param segundos O tempo em segundos para pausar o programa.
 */
void pausar(float segundos);

/**
 * Move o cursor para uma determinada linha e coluna.
 *
 * @param linha A linha para mover o cursor.
 * @param coluna A coluna para mover o cursor.
 */
void mover_cursor(int linha, int coluna);

/**
 * Limpa o cursor da tela.
 */
void limpar_cursor();

/**
 * Libera todas as memórias alocadas
 */
void terminar_programa(int sig);

/**
 * Mostra o logo da UFS.
 */
void ufs_logo();

/**
 * Limpa a lateral do labirinto.
 *
 * @param labirinto A instância do labirinto.
 */
void limpar_lateral(Labirinto *labirinto);

/**
 * Mostra uma mensagem na tela abaixo dos status.
 *
 * @param labirinto A instância do labirinto.
 * @param mensagem A mensagem a ser exibida.
 */
void mensagem(Labirinto *labirinto, wchar_t *mensagem);

/**
 * Checa se um ponto do labirinto está dentro do tamanho do labirinto.
 *
 * @param tamanho As dimensões do labirinto.
 * @param coordenada As coordenadas do ponto.
 * @return Boolean
 */
int checar_coordenada(int tamanho[2], int coordenada[2]);

#endif