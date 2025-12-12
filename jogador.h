#ifndef JOGADOR_H
#define JOGADOR_H

#include "estruturas.h"

Player carregarJogador(const char *nome);
void salvarRecorde(Player *p);
void mostrarRanking();

#endif
