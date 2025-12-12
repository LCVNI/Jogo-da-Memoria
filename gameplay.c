#include <stdio.h>
#include <stdlib.h>
#include "gameplay.h"
#include "tabuleiro.h"
#include "jogador.h"
#include "save.h"
#include "utilidades.h"
#include "estruturas.h"

void jogar(Player jogadores[], int numJogadores, int tam, bool permitirSave) {
    int totalPares = (tam * tam) / 2;
    int vezDe = 0;
    
    for (int i = 0; i < numJogadores; i++) {
        if (jogadores[i].pontos <= 0 || jogadores[i].pontos > 10000) {
            jogadores[i].pontos = 100;
        }
        if (jogadores[i].paresEncontrados < 0 || jogadores[i].paresEncontrados > 100) {
            jogadores[i].paresEncontrados = 0;
        }
    }
    
    while (1) {
        Player *atual = &jogadores[vezDe];
        
        if (numJogadores == 1 && atual->pontos <= 0) {
            mostrarTabuleiro(tam, jogadores, numJogadores);
            printf("\n%s ficou sem pontos!\n", atual->nome);
            printf("\n*** GAME OVER ***\n");
            if (permitirSave) deletarSave();
            break;
        }
        
        int totalEncontrados = 0;
        for (int i = 0; i < numJogadores; i++) {
            totalEncontrados += jogadores[i].paresEncontrados;
        }
        
        if (totalEncontrados >= totalPares) {
            mostrarTabuleiro(tam, jogadores, numJogadores);
            printf("\n========== FIM DE JOGO ==========\n");
            
            if (numJogadores == 1) {
                printf("\n*** VOCE VENCEU! ***\n");
                printf("Pontuacao final: %d pontos\n", jogadores[0].pontos);
                if (permitirSave) deletarSave();
            } else {
                printf("\n");
                for (int i = 0; i < numJogadores; i++) {
                    printf("%s: %d pares\n", jogadores[i].nome, jogadores[i].paresEncontrados);
                }
                
                if (jogadores[0].paresEncontrados > jogadores[1].paresEncontrados) {
                    printf("\n*** %s VENCEU! ***\n", jogadores[0].nome);
                } else if (jogadores[1].paresEncontrados > jogadores[0].paresEncontrados) {
                    printf("\n*** %s VENCEU! ***\n", jogadores[1].nome);
                } else {
                    printf("\n*** EMPATE! ***\n");
                }
            }
            break;
        }
        
        mostrarTabuleiro(tam, jogadores, numJogadores);
        if (numJogadores == 1 && permitirSave) {
            printf("Digite 'S' para salvar e sair, ou pressione ENTER para continuar\n");
            char buffer[10];
            lerLinha(buffer, sizeof(buffer));
            
            if (buffer[0] == 'S' || buffer[0] == 's') {
                salvarJogo(&jogadores[0], tam);
                destruirTabuleiro(tam);
                pausar();
                return;
            }
        }
        
        printf("Vez de: %s\n\n", atual->nome);
        
        int lin1, col1, lin2, col2;
        
        lerPosicao(tam, &lin1, &col1, "Escolha a PRIMEIRA carta:");
        
        visivel[lin1][col1] = cartas[lin1][col1];
        mostrarTabuleiro(tam, jogadores, numJogadores);
        printf("Primeira: [%c] em %c%d\n", cartas[lin1][col1], 'a' + lin1, col1);
        SLEEP(1);
        
        while (1) {
            lerPosicao(tam, &lin2, &col2, "\nEscolha a SEGUNDA carta:");
            
            if (lin1 == lin2 && col1 == col2) {
                printf("\nEscolha uma carta DIFERENTE!\n");
                SLEEP(1);
                mostrarTabuleiro(tam, jogadores, numJogadores);
                printf("Primeira: [%c] em %c%d\n", cartas[lin1][col1], 'a' + lin1, col1);
                continue;
            }
            break;
        }
        
        visivel[lin2][col2] = cartas[lin2][col2];
        mostrarTabuleiro(tam, jogadores, numJogadores);
        printf("Primeira: [%c] em %c%d\n", cartas[lin1][col1], 'a' + lin1, col1);
        printf("Segunda:  [%c] em %c%d\n", cartas[lin2][col2], 'a' + lin2, col2);
        SLEEP(2);
        
        int acertou = 0;
        if (cartas[lin1][col1] == cartas[lin2][col2]) {
            printf("\n*** ACERTOU! ***\n");
            atual->paresEncontrados++;
            pausar();
            acertou = 1;
        } else {
            if (numJogadores == 1) {
                printf("\n*** ERROU! -10 pontos ***\n");
                atual->pontos -= 10;
            } else {
                printf("\n*** ERROU! ***\n");
            }
            visivel[lin1][col1] = '?';
            visivel[lin2][col2] = '?';
            pausar();
        }
        
        if (!acertou && numJogadores > 1) {
            vezDe = 1 - vezDe;
        }
    }
}
void modoIndividual() {
    system(LIMPAR);
    
    char nome[80];
    printf("\n========== MODO INDIVIDUAL ==========\n\n");
    printf("Nome do jogador: ");
    lerLinha(nome, sizeof(nome));
    
    Player jogador = carregarJogador(nome);
    
    jogador.pontos = 0;
    jogador.paresEncontrados = 0;
    
    printf("\n=== DIFICULDADE ===\n");
    printf("1 - Facil (4x4)\n");
    printf("2 - Normal (6x6)\n");
    printf("3 - Dificil (8x8)\n");
    printf("4 - Impossivel (10x10)\n");
    printf("\nEscolha: ");
    
    int opcao = lerInt();
    int tam;
    
    switch (opcao) {
        case 1: tam = 4; break;
        case 2: tam = 6; break;
        case 3: tam = 8; break;
        case 4: tam = 10; break;
        default:
            printf("Opcao invalida!\n");
            SLEEP(2);
            return;
    }
    
    system(LIMPAR);
    printf("\n=== REGRAS ===\n");
    printf("- Voce comeca com 100 pontos\n");
    printf("- Acerto: mantem pontos\n");
    printf("- Erro: -10 pontos\n");
    printf("- Zerar = Game Over\n");
    pausar();
    
    criarTabuleiro(tam);
    mostrarMemorizacao(tam);
    jogar(&jogador, 1, tam, true);
    
    if (jogador.pontos > 0) {
        salvarRecorde(&jogador);
    }
    
    destruirTabuleiro(tam);
    pausar();
}

void modoMultiplayer() {
    system(LIMPAR);
    
    Player jogadores[2] = {0};
    
    printf("\n========== MODO 2 JOGADORES ==========\n\n");
    
    printf("Nome do Jogador 1: ");
    lerLinha(jogadores[0].nome, sizeof(jogadores[0].nome));
    jogadores[0].pontos = 100;
    jogadores[0].paresEncontrados = 0;
    
    printf("Nome do Jogador 2: ");
    lerLinha(jogadores[1].nome, sizeof(jogadores[1].nome));
    jogadores[1].pontos = 100;
    jogadores[1].paresEncontrados = 0;
    
    printf("\n=== DIFICULDADE ===\n");
    printf("1 - Facil (4x4)\n");
    printf("2 - Normal (6x6)\n");
    printf("3 - Dificil (8x8)\n");
    printf("4 - Impossivel (10x10)\n");
    printf("\nEscolha: ");
    
    int opcao = lerInt();
    int tam;
    
    switch (opcao) {
        case 1: tam = 4; break;
        case 2: tam = 6; break;
        case 3: tam = 8; break;
        case 4: tam = 10; break;
        default:
            printf("Opcao invalida!\n");
            SLEEP(2);
            return;
    }
    
    system(LIMPAR);
    printf("\n=== REGRAS - 2 JOGADORES ===\n");
    printf("- Nao ha pontos neste modo\n");
    printf("- Acerto: +1 par e joga de novo\n");
    printf("- Erro: passa a vez\n");
    printf("- Vence quem fizer mais pares\n");
    pausar();
    
    criarTabuleiro(tam);
    mostrarMemorizacao(tam);
    jogar(jogadores, 2, tam, false);
    destruirTabuleiro(tam);
    pausar();
}

void continuarJogo() {
    system(LIMPAR);
    
    if (!existeSave()) {
        printf("\nNenhum jogo salvo encontrado!\n");
        pausar();
        return;
    }
    
    Player jogador;
    int tam;
    
    if (!carregarJogo(&jogador, &tam)) {
        printf("\nErro ao carregar o jogo salvo!\n");
        pausar();
        return;
    }
    
    printf("\n========== JOGO CARREGADO ==========\n");
    printf("Jogador: %s\n", jogador.nome);
    printf("Pontos: %d\n", jogador.pontos);
    printf("Pares encontrados: %d\n", jogador.paresEncontrados);
    printf("\nVamos relembrar as cartas...\n");
    pausar();
    
    int pontosBackup = jogador.pontos;
    int paresBackup = jogador.paresEncontrados;
    
    char **visivelTemp = malloc(tam * sizeof(char*));
    for (int i = 0; i < tam; i++) {
        visivelTemp[i] = malloc(tam * sizeof(char));
        for (int j = 0; j < tam; j++) {
            visivelTemp[i][j] = visivel[i][j];
        }
    }
    
    mostrarMemorizacao(tam);
    
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            visivel[i][j] = visivelTemp[i][j];
        }
        free(visivelTemp[i]);
    }
    free(visivelTemp);
    
    jogador.pontos = pontosBackup;
    jogador.paresEncontrados = paresBackup;
    
    jogar(&jogador, 1, tam, true);
    
    if (jogador.pontos > 0) {
        salvarRecorde(&jogador);
    }
    
    destruirTabuleiro(tam);
    pausar();
}