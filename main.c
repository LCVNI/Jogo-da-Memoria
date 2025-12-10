#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"
#include "utilidades.h"
#include "tabuleiro.h"
#include "jogador.h"
#include "save.h"
#include "gameplay.h"

char *simbolos = "ZWXP@$C#89F2HGM";
char **cartas = NULL;
char **visivel = NULL;

int main() {
    int opcao;
    
    do {
        system(LIMPAR);
        printf("\n================================\n");
        printf("      JOGO DA MEMORIA\n");
        printf("================================\n");
        
        if (existeSave()) {
            printf("1 - Continuar jogo salvo\n");
            printf("2 - Novo jogo individual\n");
            printf("3 - Modo 2 Jogadores\n");
            printf("4 - Ranking\n");
            printf("5 - Sair\n");
        } else {
            printf("1 - Modo Individual\n");
            printf("2 - Modo 2 Jogadores\n");
            printf("3 - Ranking\n");
            printf("4 - Sair\n");
        }
        
        printf("\nEscolha: ");
        opcao = lerInt();
        
        if (existeSave()) {
            switch (opcao) {
                case 1:
                    continuarJogo();
                    break;
                case 2:
                    modoIndividual();
                    break;
                case 3:
                    modoMultiplayer();
                    break;
                case 4:
                    system(LIMPAR);
                    mostrarRanking();
                    pausar();
                    break;
                case 5:
                    printf("\nObrigado por jogar!\n");
                    break;
                default:
                    printf("\nOpcao invalida!\n");
                    SLEEP(1);
            }
        } else {
            switch (opcao) {
                case 1:
                    modoIndividual();
                    break;
                case 2:
                    modoMultiplayer();
                    break;
                case 3:
                    system(LIMPAR);
                    mostrarRanking();
                    pausar();
                    break;
                case 4:
                    printf("\nObrigado por jogar!\n");
                    break;
                default:
                    printf("\nOpcao invalida!\n");
                    SLEEP(1);
            }
        }
    } while ((existeSave() && opcao != 5) || (!existeSave() && opcao != 4));
    
    return 0;
}