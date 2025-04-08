/*
 * labirinto.c - Protótipos das funções de entrada e saída de dados
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

#ifndef IO_H
#define IO_H

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
 * Abre um arquivo SOMENTE em modo de leitura.
 *
 * @param nome O caminho do arquivo que vai ser aberto.
 * @returns Um ponteiro do arquivo aberto.
 */
FILE *abrir_arquivo(const char *nome);

/**
 * Lê arquivo do labirinto e carrega dados na instância do labirinto
 *
 * Primeiro, leremos o tamanho do labirinto e vamos verificar se é um tamanho válido. Depois, vamos carregar o tamanho do labirinto e
 * alocar a matriz dinamicamente para armazenar o labirinto.
 *
 * @param labirinto Uma instância do labirinto.
 */
void ler_arquivo(Labirinto *labirinto);

/**
 * Salva o estado atual do labirinto em um arquivo.
 *
 * O programa vai pedir por um nome para o arquivo e escrever o tamanho do
 * labirinto e o labirinto em si conforme o modelo padrão do labirinto.
 *
 * @param labirinto Uma instância do labirinto.
 */
void salvar_arquivo(const Labirinto *labirinto);

#endif