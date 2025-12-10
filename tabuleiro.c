#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tabuleiro.h"
#include "utilidades.h"
#include "estruturas.h"

void criarTabuleiro(int tam) {
    cartas = malloc(tam * sizeof(char*));
    visivel = malloc(tam * sizeof(char*));
    
    for (int i = 0; i < tam; i++) {
        cartas[i] = malloc(tam * sizeof(char));
        visivel[i] = malloc(tam * sizeof(char));
        memset(cartas[i], 0, tam);
        memset(visivel[i], '?', tam);
    }
    
    srand(time(NULL));
    int idx = 0;
    
    for (int count = 0; count < tam * tam; count++) {
        int i, j;
        do {
            i = rand() % tam;
            j = rand() % tam;
        } while (cartas[i][j] != 0);
        
        cartas[i][j] = simbolos[idx];
        
        if (count % 2 == 1) idx++;
        if (idx >= (int)strlen(simbolos)) idx = 0;
    }
}

void destruirTabuleiro(int tam) {
    for (int i = 0; i < tam; i++) {
        free(cartas[i]);
        free(visivel[i]);
    }
    free(cartas);
    free(visivel);
}

void mostrarTabuleiro(int tam, Player jogadores[], int numJogadores) {
    system(LIMPAR);
    
    printf("\n========== JOGO DA MEMORIA ==========\n\n");
    
    for (int p = 0; p < numJogadores; p++) {
        printf("%s: %d pontos", jogadores[p].nome, jogadores[p].pontos);
        if (numJogadores > 1) {
            printf(" (%d pares)", jogadores[p].paresEncontrados);
        }
        printf("\n");
    }
    printf("\n");
    
    printf("    ");
    for (int j = 0; j < tam; j++) {
        printf(" %d  ", j);
    }
    printf("\n");
    
    for (int i = 0; i < tam; i++) {
        printf(" %c  ", 'a' + i);
        for (int j = 0; j < tam; j++) {
            printf("[%c] ", visivel[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void mostrarMemorizacao(int tam) {
    printf("\n========== MEMORIZE! ==========\n\n");
    
    printf("    ");
    for (int j = 0; j < tam; j++) {
        printf(" %d  ", j);
    }
    printf("\n");
    
    for (int i = 0; i < tam; i++) {
        printf(" %c  ", 'a' + i);
        for (int j = 0; j < tam; j++) {
            printf("[%c] ", cartas[i][j]);
        }
        printf("\n");
    }
    
    printf("\n");
    for (int i = 15; i > 0; i--) {
        printf("\rEscondendo em %d...", i);
        fflush(stdout);
        SLEEP(1);
    }
    printf("\rEscondendo em 0...\n");
    SLEEP(1);
}

void lerPosicao(int tam, int *lin, int *col, const char *msg) {
    char buffer[100];
    char letra;
    
    while (1) {
        printf("%s\n", msg);
        printf("Linha (a-%c): ", 'a' + tam - 1);
        lerLinha(buffer, sizeof(buffer));
        letra = buffer[0];
        
        printf("Coluna (0-%d): ", tam - 1);
        *col = lerInt();
        
        *lin = letraParaIndice(letra);
        
        if (*lin < 0 || *lin >= tam || *col < 0 || *col >= tam) {
            printf("\nPosicao invalida!\n");
            SLEEP(1);
            continue;
        }
        
        if (visivel[*lin][*col] != '?') {
            printf("\nCarta ja descoberta!\n");
            SLEEP(1);
            continue;
        }
        
        break;
    }
}
