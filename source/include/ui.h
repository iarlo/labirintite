/*
 * ui.h - Protótipos das funções de interface
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

#ifndef UI_H
#define UI_H

#include "typedefs.h"
#include "util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/**
 * Imprime o título do jogo.
 */
void imprimir_titulo();

/**
 * Imprime o menu de opções.
 *
 * @param labirinto A instância do labirinto.
 */
void imprimir_opcoes(Labirinto *labirinto);

/**
 * Imprime o labirinto com caracteres especiais para representar as paredes.
 * O labirinto é percorrido vértice por vértice, verificando as adjacências
 * e cada caractere é potencialmente substituído por um caractere especial
 * com base nisso.
 *
 * Durante o desenvolvimento, percebemos um problema: Como é adicionado um
 * espaço entre cada vértice do labirinto, as paredes apresentavam lacunas.
 * Para corrigir isso, criamos um algoritmo que verifica se existem paredes
 * adjacentes e adiciona um caractere especial entre elas. Caso contrário,
 * adicionamos um espaço em branco.
 *
 * @param labirinto A instância do labirinto.
 */
void imprimir_labirinto(Labirinto *labirinto);

/**
 * Imprime o status do jogo na tela, com informações sobre o número de tentativas
 * e o número de inimigos derrotados.
 *
 * @param labirinto A instância do labirinto.
 */
void imprimir_status(Labirinto *labirinto);

/**
 * Imprime o menu interativo e o labirinto na tela, permitindo que o jogador escolha uma opção.
 *
 * @param labirinto A instância do labirinto.
 */
void imprimir_tela(Labirinto *labirinto);

/**
 * Seleciona a opção escolhida pelo jogador e a executa.
 *
 * @param labirinto A instância do labirinto.
 */
void selecionar_opcao(Labirinto *labirinto);

/**
 * Atualiza a interface do jogo, imprimindo o labirinto e o status.
 *
 * @param labirinto A instância do labirinto.
 */
void atualizar_interface(Labirinto *labirinto);

#endif