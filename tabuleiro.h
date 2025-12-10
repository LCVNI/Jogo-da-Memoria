#ifndef TABULEIRO_H
#define TABULEIRO_H

#include "estruturas.h"

void criarTabuleiro(int tam);
void destruirTabuleiro(int tam);
void mostrarTabuleiro(int tam, Player jogadores[], int numJogadores);
void mostrarMemorizacao(int tam);
void lerPosicao(int tam, int *lin, int *col, const char *msg);

#endif