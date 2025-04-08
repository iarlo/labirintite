/*
 * os.c - Funções específicas para diferentes sistemas operacionais
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
#include <locale.h>
#include <time.h>
#include <wchar.h>

#ifdef _WIN32
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <windows.h>

HANDLE entrada_terminal;
DWORD config_terminal;

void configurar_terminal() {
  entrada_terminal = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(entrada_terminal, &config_terminal);
  SetConsoleMode(entrada_terminal, config_terminal & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
  wprintf(HIDE_CURSOR);
}

void restaurar_terminal() {
  SetConsoleMode(entrada_terminal, config_terminal | ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
  wprintf(RESET_CURSOR);
  fflush(stdout);
}

int ler_tecla_pressionada() {
  int caractere = _getch();
  return (caractere == 224) ? (_getch() == 72 ? ACIMA : ABAIXO) : (caractere == '\r' || caractere == '\n' ? '\n' : -1);
}

#else
#include <termios.h>
#include <unistd.h>

void configurar_terminal() {
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
  wprintf(HIDE_CURSOR);
}

void restaurar_terminal() {
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag |= ICANON | ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
  wprintf(RESET_CURSOR);
}

int ler_tecla_pressionada() {
  char buffer[3];
  if (read(STDIN_FILENO, buffer, sizeof(buffer)) == sizeof(buffer) && buffer[0] == 27 && buffer[1] == 91) {
    if (buffer[2] == 65)
      return ACIMA;
    if (buffer[2] == 66)
      return ABAIXO;
  }
  return buffer[0];
}
#endif

void configurar_idioma() {
  setlocale(LC_CTYPE, "en_US.UTF-8");
  setlocale(LC_ALL, "");
  srand(time(NULL)); // Sem isso a função rand() sempre escolhe o mesmo caminho

#ifdef _WIN32
  _setmode(_fileno(stdout), _O_U16TEXT);
  SetConsoleCP(65001);
  SetConsoleOutputCP(65001);
#endif
}