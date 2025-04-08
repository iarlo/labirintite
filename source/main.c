/*
 * main.c
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
#include "include/typedefs.h"
#include "include/ui.h"
#include "include/util.h"

#include <signal.h>
#include <string.h>

int main(int argc, char **argv) {
  signal(SIGINT, terminar_programa);
  configurar_idioma();
  Labirinto labirinto_atual = { 0 };

  if (argc >= 2) {
    labirinto_atual.arquivo = abrir_arquivo(argv[1]);
    strncpy(labirinto_atual.nome, argv[1], sizeof labirinto_atual.nome - 1);
    labirinto_atual.nome[sizeof(labirinto_atual.nome) - 1] = '\0';
  }

  imprimir_tela(&labirinto_atual);
  return 0;
}
