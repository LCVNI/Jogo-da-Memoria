#ifndef SAVE_H
#define SAVE_H

#include "estruturas.h"

bool existeSave();
void salvarJogo(Player *jogador, int tam);
bool carregarJogo(Player *jogador, int *tam);
void deletarSave();

#endif