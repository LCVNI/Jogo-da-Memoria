#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "estruturas.h"

void jogar(Player jogadores[], int numJogadores, int tam, bool permitirSave);
void modoIndividual();
void modoMultiplayer();
void continuarJogo();

#endif