/*
 * labirintite.c - Provavelmente o resolvedor de labirintos mais tonto que você já viu
 *
 * Copyright (C) 2025 Bruno Sá <iarlo.sa@dcomp.ufs.br>
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

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

#undef max
#undef min
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

typedef struct
{
    char nome[50];
    FILE *arquivo;
    int tamanho[2];
    char **matriz;
    int posicao_jogador[2];
    int posicao_saida[2];
} Labirinto;

typedef enum
{
    ACIMA = 0,
    ABAIXO = 1,
    ESQUERDA = 2,
    DIREITA = 3
} Direcoes;

int opcao_selecionada = 0;
char lista_de_opcoes[4][50] = {"Resolver diretamente", "Tentativa e erro", "Salvar em um arquivo", "Sair"};

/**
 * Imprime mensagens de erro formatadas.
 *
 * Mensagens coloridas - https://www.theurbanpenguin.com/4184-2/
 */
void erro(char *mensagem)
{
    printf("\033[0;31m");
    printf("\n[ERRO]: %s\n", mensagem);
    printf("\033[0m \n");
    return exit(1);
}

/**
 * Alocar matriz dinâmica para o labirinto com base nas linhas e colunas
 * extraidas do arquivo do labirinto. Cria uma matriz bidimensional de char
 */
char **alocar_matriz(int linhas, int colunas)
{
    char **matriz = malloc(linhas * sizeof *matriz);

    if (matriz == NULL)
        erro("Falha ao alocar linhas da matriz.");

    for (int i = 0; i < linhas + 1; i++)
    {
        matriz[i] = malloc((colunas + 1) * sizeof *matriz[i]);
        if (matriz[i] == NULL)
            erro("Falha ao alocar colunas da matriz.");
    }

    return matriz;
}

/**
 * Libera iterativamente a memoria alocada por uma matriz.
 */
void limpar_matriz(char **matriz, int linhas)
{
    for (int i = 0; i < linhas; i++)
        free(matriz[i]);

    free(matriz);
}

/**
 * Abre um arquivo em modo de leitura.
 * Caso o arquivo nao exista, imprime uma mensagem de erro e encerra o programa.
 */
FILE *abrir_arquivo(const char *nome)
{
    FILE *arquivo = fopen(nome, "r");
    if (arquivo == NULL)
        erro("Arquivo não encontrado");

    return arquivo;
}

/**
 * Lê arquivo do labirinto e carrega dados no struct
 *
 * 1- Lê a primeira linha do arquivo e extrai as informações de tamanho
 * 2- Verifica se o tamanho informado e valido (20x20 no máximo).
 * 3- Carrega o tamanho do labirinto no struct e aloca a matriz
 *    dinamicamente para armazenar o labirinto.
 */
void ler_arquivo(Labirinto *labirinto_atual)
{
    char primeira_linha[50];

    if (fgets(primeira_linha, sizeof primeira_linha, labirinto_atual->arquivo) == NULL)
        erro("Erro ao ler o tamanho do labirinto.");

    if (sscanf(primeira_linha, "%d %d", &labirinto_atual->tamanho[0], &labirinto_atual->tamanho[1]) != 2)
        erro("Tamanho do labirinto inválido.");

    if (labirinto_atual->tamanho[0] <= 0 || labirinto_atual->tamanho[1] <= 0 || labirinto_atual->tamanho[0] > 20 || labirinto_atual->tamanho[1] > 20)
        erro("Tamanho do labirinto inválido. O labirinto deve ter entre 1 e 20 linhas e colunas.");

    labirinto_atual->matriz = alocar_matriz(labirinto_atual->tamanho[0], labirinto_atual->tamanho[1]);
}

/**
 * Salva o estado atual do labirinto em um arquivo.
 */
void salvar_arquivo(Labirinto *labirinto)
{
    char nome_arquivo[100];
    printf("\nDigite o nome do arquivo: ");
    scanf("%50s", nome_arquivo);
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL)
        erro("Erro ao salvar o arquivo.");

    fprintf(arquivo, "%d %d\n", labirinto->tamanho[0], labirinto->tamanho[1]);
    for (int i = 0; i < labirinto->tamanho[0]; i++)
    {
        for (int j = 0; j < labirinto->tamanho[1]; j++)
            fprintf(arquivo, "%c ", labirinto->matriz[i][j]);

        fprintf(arquivo, "\n");
    }
    fclose(arquivo);
}

/**
 * Preenche a matriz com os caracteres do arquivo do labirinto, também carrega
 * as posições do jogador e da saída.
 */
void preencher_matriz(Labirinto *labirinto)
{
    char **linhas = alocar_matriz(labirinto->tamanho[0], labirinto->tamanho[1]);

    for (int i = 0; i < labirinto->tamanho[0]; i++)
    {
        if (fgets(linhas[i], 50, labirinto->arquivo) == NULL)
            erro("Erro ao ler linha do arquivo.");

        int coluna_atual = 0;
        for (int j = 0; j < 50 && coluna_atual < labirinto->tamanho[1] && linhas[i][j] != '\0'; j++)
            if (linhas[i][j] != '\n' && linhas[i][j] != ' ')
            {
                switch (linhas[i][j])
                {
                case '@':
                    labirinto->posicao_jogador[0] = i;
                    labirinto->posicao_jogador[1] = coluna_atual;
                    break;
                case '$':
                    labirinto->posicao_saida[0] = i;
                    labirinto->posicao_saida[1] = coluna_atual;
                    break;
                }

                labirinto->matriz[i][coluna_atual] = linhas[i][j];
                coluna_atual++;
            }
    }

    // limpar_matriz(linhas, labirinto->tamanho[0]);
}

/**
 * Encontra as adjacencias de um caractere na matriz do labirinto.
 */
char *encontrar_adjacencias(Labirinto *labirinto, int linha, int coluna)
{
    char *vizinhos = malloc(4 * sizeof(char));

    if (!vizinhos)
        erro("Falha ao alocar memória para vizinhos.");

    for (int k = 0; k < 4; k++)
        vizinhos[k] = '\0';

    if (linha > 0)
        vizinhos[ACIMA] = labirinto->matriz[linha - 1][coluna];
    if (linha < labirinto->tamanho[0] - 1)
        vizinhos[ABAIXO] = labirinto->matriz[linha + 1][coluna];
    if (coluna > 0)
        vizinhos[ESQUERDA] = labirinto->matriz[linha][coluna - 1];
    if (coluna < labirinto->tamanho[1] - 1)
        vizinhos[DIREITA] = labirinto->matriz[linha][coluna + 1];

    return vizinhos;
}

#ifdef _WIN32
/**
 * Imprime o labirinto na tela, com cada caractere separado por um espaço.
 */
void imprimir_labirinto(Labirinto *labirinto)
{
    for (int i = 0; i < labirinto->tamanho[0]; i++)
    {
        for (int j = 0; j < labirinto->tamanho[1]; j++)
            printf("%c ", labirinto->matriz[i][j]);
        printf("\n");
    }
}

/**
 * Imprime uma opção do menu com a formatação de lista.
 * ! Apenas windows, no linux a função é outra
 */
void imprimir_opcoes()
{
    for (int i = 0; i < 4; i++)
    {
        printf("[%d] %s    ", i + 1, lista_de_opcoes[i]);
    }
}

#else

// O windows não suporta caracteres especiais, por isso só vamos definir para linux
wchar_t mapa_binario[16] = {
    L'\x2501',
    L'\x2501',
    L'\x2501',
    L'\x2501',
    L'\x2503',
    L'\x250F',
    L'\u2513',
    L'\x2533',
    L'\x2503',
    L'\x2517',
    L'\x251B',
    L'\x253B',
    L'\x2503',
    L'\x2523',
    L'\x252b',
    L'\x254B',
};

/**
 * Verifica se um caractere é uma parede. Isso vai ajudar
 * nos algoritmos de substituição de paredes para o Labirinto
 */
int verificar_parede(int caractere)
{
    return caractere == '#';
}

/**
 * Muda um caractere de um labirinto de acordo com as paredes ao redor.
 *
 * Se o caractere for uma parede, o algoritmo verifica quais paredes existem
 * ao redor e substitui o caractere por um caractere especial que represente
 * melhor as paredes. Caso contrário, o caractere permanece o mesmo.
 */
wchar_t mudar_caractere(Labirinto *labirinto, int linha, int coluna)
{
    char caractere = labirinto->matriz[linha][coluna];
    char *adjacencias = encontrar_adjacencias(labirinto, linha, coluna);

    wchar_t novo_caractere = caractere;

    if (caractere == '#')
    {
        int index = (verificar_parede(adjacencias[ACIMA]) << 3) |
                    (verificar_parede(adjacencias[ABAIXO]) << 2) |
                    (verificar_parede(adjacencias[ESQUERDA]) << 1) |
                    verificar_parede(adjacencias[DIREITA]);
        novo_caractere = mapa_binario[index];
    }
    else if (caractere == '$')
        novo_caractere = L'\x2691';
    else if (caractere == '%')
        novo_caractere = L'\x0001D526';
    else if (caractere == '.')
        novo_caractere = L'\x22C5';

    free(adjacencias);
    return novo_caractere;
}

/**
 * Imprime uma opção do menu com a formatação de lista.
 * ! Apenas linux, no windows a função é outra
 */
int imprimir_opcoes(char opcoes[4][50], int index, int opcao_selecionada)
{
    if (index == 0)
        return printf("    Selecione uma opção:");

    if (index < 2 || index > 5)
        return printf("  ");

    char display[50];

    snprintf(display, sizeof(display), "%*s%s",
             opcao_selecionada == index - 2 ? 2 : 3, opcao_selecionada == index - 2 ? ">  " : "", opcoes[index - 2]);

    return printf("    %s", display);
}

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
 */
void imprimir_labirinto(Labirinto *labirinto, int menu)
{
    for (int i = 0; i < labirinto->tamanho[0]; i++)
    {
        for (int j = 0; j < labirinto->tamanho[1]; j++)
        {
            wchar_t caractere = mudar_caractere(labirinto, i, j);
            wchar_t lacuna = labirinto->matriz[i][j] == '#' && (verificar_parede(labirinto->matriz[i][j + 1]) & 0b01) ? L'\x2501' : L' ';

            printf("%lc%lc", caractere, lacuna);
        }
        if (menu)
            imprimir_opcoes(lista_de_opcoes, i - 1, opcao_selecionada);
        printf("\n");
    }
}

/**
 * Configura o terminal para não repetir as teclas pressionadas e ler caractere por caractere,
 * ao invés de linha por linha. Vai ser útil para o menu interativo funcionar
 *
 * https://gist.github.com/dagon666/8194870
 */
void configurar_terminal(void)
{
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

/**
 * Restaura as configurações padrão do terminal, habilitando o modo canônico
 * e a exibição de caracteres digitados, permitindo que o terminal funcione
 * normalmente após a leitura de entrada em modo não-canônico.
 *
 * https://gist.github.com/dagon666/8194870
 */
void restaurar_terminal()
{
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

/**
 * Se a tecla pressionada for uma das setas, retorna o valor correspondente
 * (ACIMA, ABAIXO, ESQUERDA, DIREITA).
 */
char ler_tecla_pressionada()
{
    char buffer[3];

    read(STDIN_FILENO, buffer, 3);

    if (buffer[0] == 27 && buffer[1] == 91)
    {
        switch (buffer[2])
        {
        case 65:
            return ACIMA;
        case 66:
            return ABAIXO;
        }
    }

    return buffer[0];
}
#endif

void imprimir_titulo(void)
{
    const char *titulo =
        "             __       __            ___     ___   ___\n"
        "|      /\\   |__)  |  |__)  |  |\\ |   |   |   |   |__\n"
        "|___  /--\\  |__)  |  |  \\  |  | \\|   |   |   |   |___\n\n"
        "O resolvedor de labirintos mais tonto que você já viu\n"
        "-----------------------------------------------------\n";

    printf("\033[H\033[J%s", titulo);
}

/**
 * Imprime o menu interativo no Linux, e um menu básico no Windows. Se o arquivo
 * não estiver defindo, pergunta o nome do arquivo e o abre. Caso contrário, lê o
 * arquivo automaticamente e preenche a matriz.
 */
void imprimir_tela(Labirinto *labirinto)
{
    imprimir_titulo();

    if (labirinto->arquivo == NULL)
    {
        printf("Digite o nome do arquivo: ");
        scanf("%49s", labirinto->nome);
        labirinto->arquivo = abrir_arquivo(labirinto->nome);
    }

    ler_arquivo(labirinto);
    preencher_matriz(labirinto);

#ifdef _WIN32
    printf("\n");
    imprimir_labirinto(labirinto);
    printf("\n");
    imprimir_opcoes();
    printf("\n\nSelecione uma opção: ");
    while ((scanf(" %d", &opcao_selecionada) != 1) || (opcao_selecionada < 0) || (opcao_selecionada > 4))
    {
        fflush(stdin);
        printf("\n\nSelecione uma opção: ");
    }

    switch (opcao_selecionada)
    {
    case 1:
        erro("Função ainda não implementada");
        // resolver_diretamente(labirinto);
        break;
    case 2:
        erro("Função ainda não implementada");
        // tentativa_e_erro(labirinto);
        break;
    case 3:
        salvar_arquivo(labirinto);
        exit(0);
        break;
    case 4:
        printf("Tchauuu!\n");
        exit(0);
        break;
    }
#else
    // Menu interativo no sistema Linux
    configurar_terminal();

    while (1)
    {
        imprimir_titulo();
        imprimir_labirinto(labirinto, 1);

        switch (ler_tecla_pressionada())
        {
        case ACIMA:
            opcao_selecionada = max(0, opcao_selecionada - 1);
            break;
        case ABAIXO:
            opcao_selecionada = min(3, opcao_selecionada + 1);
            break;
        case '\n':
            switch (opcao_selecionada)
            {
            case 0:
                restaurar_terminal();
                erro("Função ainda não implementada");
                // resolver_diretamente(labirinto);
                break;
            case 1:
                restaurar_terminal();
                erro("Função ainda não implementada");
                // tentativa_e_erro(labirinto);
                break;
            case 2:
                restaurar_terminal();
                salvar_arquivo(labirinto);
                exit(0);
                break;
            case 3:
                printf("Tchauuu!\n");
                restaurar_terminal();
                exit(0);
                break;
            }
            break;
        }
    }
#endif
}

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");

    Labirinto labirinto_atual = {0};

    if (argc >= 2)
    {
        labirinto_atual.arquivo = abrir_arquivo(argv[1]);
        strncpy(labirinto_atual.nome, argv[1], sizeof(labirinto_atual.nome) - 1);
        labirinto_atual.nome[sizeof(labirinto_atual.nome) - 1] = '\0';
    }

    imprimir_tela(&labirinto_atual);
}
