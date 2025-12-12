#include <stdio.h>
#include <string.h>
#include "save.h"
#include "tabuleiro.h"
#include "estruturas.h"

bool existeSave() {
    FILE *f = fopen("savegame.dat", "rb");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}

void salvarJogo(Player *jogador, int tam) {
    SaveGame save;
    
    strcpy(save.nomeJogador, jogador->nome);
    save.pontos = jogador->pontos;
    save.paresEncontrados = jogador->paresEncontrados;
    save.tamanho = tam;
    
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            save.tabuleiro[i][j] = cartas[i][j];
            save.visivelSave[i][j] = visivel[i][j];
        }
    }
    
    FILE *f = fopen("savegame.dat", "wb");
    if (f) {
        fwrite(&save, sizeof(SaveGame), 1, f);
        fclose(f);
        printf("\nJogo salvo com sucesso!\n");
    } else {
        printf("\nErro ao salvar o jogo!\n");
    }
}

bool carregarJogo(Player *jogador, int *tam) {
    FILE *f = fopen("savegame.dat", "rb");
    if (!f) {
        return false;
    }
    
    SaveGame save;
    if (fread(&save, sizeof(SaveGame), 1, f) != 1) {
        fclose(f);
        return false;
    }
    fclose(f);
    
    strcpy(jogador->nome, save.nomeJogador);
    jogador->pontos = save.pontos;
    jogador->paresEncontrados = save.paresEncontrados;
    *tam = save.tamanho;
    
    criarTabuleiro(*tam);
    
    for (int i = 0; i < *tam; i++) {
        for (int j = 0; j < *tam; j++) {
            cartas[i][j] = save.tabuleiro[i][j];
            visivel[i][j] = save.visivelSave[i][j];
        }
    }
    
    return true;
}

void deletarSave() {
    remove("savegame.dat");
}
