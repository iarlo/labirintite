/*
 * This is free and unencumbered software released into the public domain.
 *
 * For more information, please refer to <https://unlicense.org>
 *
 * Retirado de
 * https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
 */
#ifndef COLOR_H
#define COLOR_H

#include <wchar.h>

// Regular text
#define BLK L"\e[0;30m"
#define RED L"\e[0;31m"
#define GRN L"\e[0;32m"
#define YEL L"\e[0;33m"
#define BLU L"\e[0;34m"
#define MAG L"\e[0;35m"
#define CYN L"\e[0;36m"
#define WHT L"\e[0;37m"

// Regular bold text
#define BBLK L"\e[1;30m"
#define BRED L"\e[1;31m"
#define BGRN L"\e[1;32m"
#define BYEL L"\e[1;33m"
#define BBLU L"\e[1;34m"
#define BMAG L"\e[1;35m"
#define BCYN L"\e[1;36m"
#define BWHT L"\e[1;37m"

// Regular underline text
#define UBLK L"\e[4;30m"
#define URED L"\e[4;31m"
#define UGRN L"\e[4;32m"
#define UYEL L"\e[4;33m"
#define UBLU L"\e[4;34m"
#define UMAG L"\e[4;35m"
#define UCYN L"\e[4;36m"
#define UWHT L"\e[4;37m"

// Regular background
#define BLKB L"\e[40m"
#define REDB L"\e[41m"
#define GRNB L"\e[42m"
#define YELB L"\e[43m"
#define BLUB L"\e[44m"
#define MAGB L"\e[45m"
#define CYNB L"\e[46m"
#define WHTB L"\e[47m"

// High intensty background
#define BLKHB L"\e[0;100m"
#define REDHB L"\e[0;101m"
#define GRNHB L"\e[0;102m"
#define YELHB L"\e[0;103m"
#define BLUHB L"\e[0;104m"
#define MAGHB L"\e[0;105m"
#define CYNHB L"\e[0;106m"
#define WHTHB L"\e[0;107m"

// High intensty text
#define HBLK L"\e[0;90m"
#define HRED L"\e[0;91m"
#define HGRN L"\e[0;92m"
#define HYEL L"\e[0;93m"
#define HBLU L"\e[0;94m"
#define HMAG L"\e[0;95m"
#define HCYN L"\e[0;96m"
#define HWHT L"\e[0;97m"

// Bold high intensity text
#define BHBLK L"\e[1;90m"
#define BHRED L"\e[1;91m"
#define BHGRN L"\e[1;92m"
#define BHYEL L"\e[1;93m"
#define BHBLU L"\e[1;94m"
#define BHMAG L"\e[1;95m"
#define BHCYN L"\e[1;96m"
#define BHWHT L"\e[1;97m"

// Reset
#define RESET L"\e[0m"
#define CRESET L"\e[0m"
#define COLOR_RESET L"\e[0m"

// Cursor
#define HIDE_CURSOR L"\e[?25l"
#define SHOW_CURSOR L"\e[?25h"
#define RESET_CURSOR L"\e[?25h\e[u\e[0m"
#define MOVE_TOP_LEFT L"\e[H"

#define CLEAN_LINE L"\e[K"
#define CLEAN_LINE_END L"\e[J"
#define CLEAN_LINE_START L"\e[1K"
#define CLEAN_LINE_BOTH L"\e[2K"

typedef struct {
  wchar_t caractere;
  wchar_t *cor;
} Mapa_Cores;

#endif