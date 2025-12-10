#include <stdio.h>
#include <string.h>
#include "jogador.h"
#include "estruturas.h"

Player carregarJogador(const char *nome) {
    Player p;
    strcpy(p.nome, nome);
    p.pontos = 0;
    p.paresEncontrados = 0;
    
    FILE *f = fopen("lista.txt", "r");
    if (f) {
        char nomeLido[80];
        int pontosLido;
        
        while (fscanf(f, "%79s %d", nomeLido, &pontosLido) == 2) {
            if (strcmp(nome, nomeLido) == 0) {
                p.pontos = pontosLido;
                printf("\nBem-vindo de volta, %s!\n", nome);
                printf("Seu recorde: %d pontos\n", pontosLido);
                fclose(f);
                return p;
            }
        }
        fclose(f);
    }
    
    printf("\nNovo jogador criado: %s\n", nome);
    return p;
}

void salvarRecorde(Player *p) {
    FILE *f = fopen("lista.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (!temp) return;
    
    char nomeLido[80];
    int pontosLido;
    bool encontrado = false;
    
    if (f) {
        while (fscanf(f, "%79s %d", nomeLido, &pontosLido) == 2) {
            if (strcmp(p->nome, nomeLido) == 0) {
                if (p->pontos > pontosLido) {
                    fprintf(temp, "%s %d\n", p->nome, p->pontos);
                    printf("\n*** NOVO RECORDE! %d -> %d pontos ***\n", pontosLido, p->pontos);
                } else {
                    fprintf(temp, "%s %d\n", nomeLido, pontosLido);
                }
                encontrado = true;
            } else {
                fprintf(temp, "%s %d\n", nomeLido, pontosLido);
            }
        }
        fclose(f);
    }
    
    if (!encontrado) {
        fprintf(temp, "%s %d\n", p->nome, p->pontos);
    }
    
    fclose(temp);
    remove("lista.txt");
    rename("temp.txt", "lista.txt");
}

void mostrarRanking() {
    FILE *f = fopen("lista.txt", "r");
    if (!f) {
        printf("\nNenhum jogador cadastrado ainda!\n");
        return;
    }
    
    Player lista[100];
    int total = 0;
    
    while (fscanf(f, "%79s %d", lista[total].nome, &lista[total].pontos) == 2) {
        total++;
        if (total >= 100) break;
    }
    fclose(f);
    
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (lista[j].pontos < lista[j+1].pontos) {
                Player temp = lista[j];
                lista[j] = lista[j+1];
                lista[j+1] = temp;
            }
        }
    }
    
    printf("\n========== TOP 10 ==========\n\n");
    int limite = (total < 10) ? total : 10;
    for (int i = 0; i < limite; i++) {
        printf("%2d. %-20s %d pontos\n", i+1, lista[i].nome, lista[i].pontos);
    }
}
