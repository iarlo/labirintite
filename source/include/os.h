/*
 * os.h - Protótipos das funções específicas para diferentes SO
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

#ifndef OS_H
#define OS_H

#include "ansi-color-codes.h"
#include "typedefs.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Configura o terminal para não repetir as teclas pressionadas e ler caractere
 * por caractere, ao invés de linha por linha. Vai ser útil para o menu
 * interativo funcionar
 *
 * A função também esconde o cursor do terminal, pra resolver um problema onde a
 * tela parecia piscar a cada impressão do labirinto, pois o cursor estava em
 * constante movimento
 *
 * https://gist.github.com/dagon666/8194870
 * https://learn.microsoft.com/en-us/windows/console/setconsolemode
 */
void configurar_terminal();

/**
 * Restaura as configurações padrão do terminal, habilitando o modo canônico e a
 * exibição de caracteres digitados, permitindo que o terminal funcione
 * normalmente após a leitura de entrada em modo não-canônico.
 *
 * O terminal volta a mostrar o cursor e sua do cursor é restaurada para o local
 * onde o menu estava antes de o menu ser aberto.
 *
 * https://gist.github.com/dagon666/8194870
 * https://learn.microsoft.com/en-us/windows/console/setconsolemode
 */
void restaurar_terminal();

/**
 * Se a tecla pressionada for uma das setas, retorna o valor correspondente
 */

/**
 * Se a tecla pressionada for uma das setinhas, retorna o valor correspondente
 * ou -1 se a tecla não for uma das setas. É necessário ativar o modo
 * não-canônico, pra garantir que a tecla seja lida imediatamente, sem precisar
 * dar enter.
 */
int ler_tecla_pressionada();

/**
 * Configura a saída para UTF-8 e configura o gerador de números aleatórios para
 * gerar diferentes resultados em cada execução do programa.
 *
 * Caso o programa esteja rodando no Windows, configura a saída padrão para usar
 * UTF-16LE.
 */
void configurar_idioma();

#endif
