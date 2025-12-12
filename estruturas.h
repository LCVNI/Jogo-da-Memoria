#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
    #define LIMPAR "cls"
    #define SLEEP(x) Sleep((x)*1000)
#else
    #include <unistd.h>
    #define LIMPAR "clear"
    #define SLEEP(x) sleep(x)
#endif

typedef struct {
    char nome[80];
    int pontos;
    int paresEncontrados;
} Player;

typedef struct {
    char nomeJogador[80];
    int pontos;
    int paresEncontrados;
    int tamanho;
    char tabuleiro[10][10];
    char visivelSave[10][10];
} SaveGame;

extern char *simbolos;
extern char **cartas;
extern char **visivel;

#endif