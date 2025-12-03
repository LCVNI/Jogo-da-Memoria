#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#include<stdbool.h>

typedef struct {
    char nome[80];
    int pontos;
} Player;

// Variáveis globais para as cartas
char *simbols = "ZWXP@$C#89F2HGM";
char **cartas, **msave;

// FUNÇÕES DE ARQUIVO E LOGIN

void addPlayer(Player *player) {
    FILE *lista = fopen("lista.txt", "a");
    if (!lista) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }
    
    printf("Nome do novo jogador: ");
    fgets(player->nome, sizeof(player->nome), stdin);
    player->nome[strcspn(player->nome, "\n")] = '\0';
    player->pontos = 0;
    
    fprintf(lista, "%s %d\n", player->nome, player->pontos);
    fclose(lista);
    printf("Jogador criado com sucesso!\n");
}

Player login() {
    Player player;
    FILE *lista = fopen("lista.txt", "r");
    
    printf("=== BEM-VINDO AO JOGO DA MEMORIA ===\n");
    printf("Nome de Jogador: ");
    fgets(player.nome, sizeof(player.nome), stdin);
    player.nome[strcspn(player.nome, "\n")] = '\0';
    
    if (!lista) {
        printf("Arquivo não encontrado. Criando novo jogador...\n");
        player.pontos = 0;
        addPlayer(&player);
        return player;
    }
    
    char nomeLido[80];
    int pontosLido;
    bool encontrado = false;
    
    while (fscanf(lista, "%79s %d", nomeLido, &pontosLido) == 2) {
        if (strcmp(player.nome, nomeLido) == 0) {
            printf("\nBem-vindo de volta, %s!\n", nomeLido);
            printf("Seu recorde atual: %d pontos\n\n", pontosLido);
            player.pontos = pontosLido;
            encontrado = true;
            break;
        }
    }
    
    fclose(lista);
    
    if (!encontrado) {
        printf("Nenhum jogador encontrado com esse nome.\n");
        player.pontos = 0;
        addPlayer(&player);
    }
    
    return player;
}

// FUNÇÕES DE MANIPULAÇÃO DE CARTAS

void armazenaCartas(int tam, const char *simbols) {
    cartas = malloc(tam * sizeof(char*));
    msave = malloc(tam * sizeof(char*));
    
    for(int i = 0; i < tam; i++) {
        cartas[i] = malloc(tam * sizeof(char));
        msave[i] = malloc(tam * sizeof(char));
    }
    
    // Inicializa as matrizes
    for(int i = 0; i < tam; i++) {
        for(int j = 0; j < tam; j++) {
            cartas[i][j] = '?';
            msave[i][j] = '?';
        }
    }
    
    // Preenche com símbolos aleatórios em pares
    unsigned int idx = 0;
    srand(time(NULL));
    
    for(int i = 0; i < tam; i++) {
        for(int j = 0; j < tam; j++) {
            int r1, r2;
            do {
                r1 = rand() % tam;
                r2 = rand() % tam;
            } while(cartas[r1][r2] != '?');
            
            if(idx >= strlen(simbols)) {
                idx = 0;
            }
            
            cartas[r1][r2] = simbols[idx];
            
            if ((i * tam + j) % 2 == 1) {
                idx++;
            }
        }
    }
}

void mostraTabuleiro(int tam, int pontos) {
    system("cls");
    
    printf("\n=== JOGO DA MEMORIA ===\n");
    printf("Pontos: %d\n\n", pontos);
    
    // Cabeçalho com números das colunas
    printf("   ");
    for(int j = 0; j < tam; j++) {
        printf(" %d ", j);
    }
    printf("\n");
    
    // Linhas com letras e cartas
    for(int i = 0; i < tam; i++) {
        printf("%c  ", 'a' + i);
        for(int j = 0; j < tam; j++) {
            printf("[%c]", msave[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void liberaMemoria(int tam) {
    for(int i = 0; i < tam; i++) {
        free(cartas[i]);
        free(msave[i]);
    }
    free(cartas);
    free(msave);
}

// FUNÇÕES DO JOGO

void inicializaJogo(int tam) {
    armazenaCartas(tam, simbols);
    
    printf("\n=== MEMORIZE AS CARTAS! ===\n\n");
    
    // Mostra todas as cartas
    printf("   ");
    for(int j = 0; j < tam; j++) {
        printf(" %d ", j);
    }
    printf("\n");
    
    for(int i = 0; i < tam; i++) {
        printf("%c  ", 'a' + i);
        for(int j = 0; j < tam; j++) {
            printf("[%c]", cartas[i][j]);
        }
        printf("\n");
    }
    
    // Contador regressivo
    printf("\n");
    for(int i = 3; i > 0; i--) {
        printf("\rEscondendo em %d...", i);
        fflush(stdout);
        sleep(1);
    }
    printf("\rEscondendo em 0...\n");
    sleep(1);
}

int converteLetra(char c) {
    if(c >= 'a' && c <= 'z') return c - 'a';
    if(c >= 'A' && c <= 'Z') return c - 'A';
    return -1;
}

int turno(Player *jogador, int tam) {
    int lin1, col1, lin2, col2;
    char letraLin;
    
    // === ESCOLHE PRIMEIRA CARTA ===
    while(1) {
        mostraTabuleiro(tam, jogador->pontos);
        printf("%s, escolha a PRIMEIRA carta:\n", jogador->nome);
        printf("Linha (a-%c): ", 'a' + tam - 1);
        scanf(" %c", &letraLin);
        printf("Coluna (0-%d): ", tam - 1);
        scanf("%d", &col1);
        
        lin1 = converteLetra(letraLin);
        
        // Validações
        if(lin1 < 0 || lin1 >= tam || col1 < 0 || col1 >= tam) {
            printf("\nPosição inválida! Pressione ENTER...");
            getchar();
            getchar();
            continue;
        }
        
        if(msave[lin1][col1] != '?') {
            printf("\nEssa carta já foi descoberta! Pressione ENTER...");
            getchar();
            getchar();
            continue;
        }
        
        break;
    }
    
    // Revela primeira carta
    msave[lin1][col1] = cartas[lin1][col1];
    mostraTabuleiro(tam, jogador->pontos);
    printf("Primeira carta: [%c]\n", cartas[lin1][col1]);
    sleep(1);
    
    // === ESCOLHE SEGUNDA CARTA ===
    while(1) {
        printf("\n%s, escolha a SEGUNDA carta:\n", jogador->nome);
        printf("Linha (a-%c): ", 'a' + tam - 1);
        scanf(" %c", &letraLin);
        printf("Coluna (0-%d): ", tam - 1);
        scanf("%d", &col2);
        
        lin2 = converteLetra(letraLin);
        
        // Validações
        if(lin2 < 0 || lin2 >= tam || col2 < 0 || col2 >= tam) {
            printf("\nPosição inválida! Tente novamente...");
            sleep(1);
            mostraTabuleiro(tam, jogador->pontos);
            printf("Primeira carta: [%c] na posição %c%d\n", 
                   cartas[lin1][col1], 'a' + lin1, col1);
            continue;
        }
        
        if(msave[lin2][col2] != '?') {
            printf("\nEssa carta já foi descoberta! Tente novamente...");
            sleep(1);
            mostraTabuleiro(tam, jogador->pontos);
            printf("Primeira carta: [%c] na posição %c%d\n", 
                   cartas[lin1][col1], 'a' + lin1, col1);
            continue;
        }
        
        if(lin1 == lin2 && col1 == col2) {
            printf("\nEscolha uma carta DIFERENTE! Tente novamente...");
            sleep(1);
            mostraTabuleiro(tam, jogador->pontos);
            printf("Primeira carta: [%c] na posição %c%d\n", 
                   cartas[lin1][col1], 'a' + lin1, col1);
            continue;
        }
        
        break;
    }
    
    // Revela segunda carta
    msave[lin2][col2] = cartas[lin2][col2];
    mostraTabuleiro(tam, jogador->pontos);
    printf("Primeira carta: [%c] na posição %c%d\n", 
           cartas[lin1][col1], 'a' + lin1, col1);
    printf("Segunda carta: [%c] na posição %c%d\n", 
           cartas[lin2][col2], 'a' + lin2, col2);
    sleep(2);
    
    // === VERIFICA SE ACERTOU ===
    if(cartas[lin1][col1] == cartas[lin2][col2]) {
        printf("\n✓ ACERTOU! +10 pontos\n");
        jogador->pontos += 10;
        printf("\nPressione ENTER para continuar...");
        getchar();
        getchar();
        return 1;
    } else {
        printf("\n✗ ERROU! -10 pontos\n");
        jogador->pontos -= 10;
        
        // Esconde as cartas novamente
        msave[lin1][col1] = '?';
        msave[lin2][col2] = '?';
        
        printf("\nPressione ENTER para continuar...");
        getchar();
        getchar();
        return 0;
    }
}

void loopJogo(Player *jogador, int tam) {
    int paresEncontrados = 0;
    int totalPares = (tam * tam) / 2;
    
    while(1) {
        // Verifica condições de fim
        if(jogador->pontos <= 0) {
            mostraTabuleiro(tam, jogador->pontos);
            printf("\n=== GAME OVER! ===\n");
            printf("Seus pontos chegaram a zero!\n");
            break;
        }
        
        if(paresEncontrados >= totalPares) {
            mostraTabuleiro(tam, jogador->pontos);
            printf("\n=== PARABÉNS! VOCÊ VENCEU! ===\n");
            printf("Todos os pares foram encontrados!\n");
            break;
        }
        
        // Executa um turno
        int acertou = turno(jogador, tam);
        if(acertou) {
            paresEncontrados++;
        }
    }
}

void finalizaJogo(Player *jogador, int tam) {
    printf("\n========== FIM DE JOGO ==========\n");
    printf("Jogador: %s\n", jogador->nome);
    printf("Pontuação final: %d pontos\n", jogador->pontos);
    
    // Atualiza pontuação no arquivo
    FILE *lista = fopen("lista.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if(!lista || !temp) {
        printf("Erro ao atualizar pontuação!\n");
        if(lista) fclose(lista);
        if(temp) fclose(temp);
        liberaMemoria(tam);
        return;
    }
    
    char nomeLido[80];
    int pontosLido;
    bool atualizado = false;
    
    while(fscanf(lista, "%79s %d", nomeLido, &pontosLido) == 2) {
        if(strcmp(nomeLido, jogador->nome) == 0) {
            // Atualiza SE a pontuação for maior que o recorde
            if(jogador->pontos > pontosLido) {
                fprintf(temp, "%s %d\n", jogador->nome, jogador->pontos);
                printf("\n🎉 NOVO RECORDE PESSOAL! %d -> %d pontos!\n", 
                       pontosLido, jogador->pontos);
                atualizado = true;
            } else {
                fprintf(temp, "%s %d\n", nomeLido, pontosLido);
                printf("\nSeu recorde continua sendo: %d pontos\n", pontosLido);
            }
        } else {
            fprintf(temp, "%s %d\n", nomeLido, pontosLido);
        }
    }
    
    fclose(lista);
    fclose(temp);
    
    // Substitui o arquivo original
    remove("lista.txt");
    rename("temp.txt", "lista.txt");
    
    // Libera memória
    liberaMemoria(tam);
    
    printf("\nPressione ENTER para voltar ao menu...");
    getchar();
    getchar();
}

void dificuldade(Player *jogador) {
    int modo, tam;
    
    system("cls");
    printf("\n=== ESCOLHA A DIFICULDADE ===\n");
    printf("1 -> Fácil (4x4 = 8 pares)\n");
    printf("2 -> Normal (6x6 = 18 pares)\n");
    printf("3 -> Difícil (8x8 = 32 pares)\n");
    printf("4 -> Impossível (10x10 = 50 pares)\n");
    printf("\nOpção: ");
    scanf("%d", &modo);
    
    switch(modo) {
        case 1: tam = 4; break;
        case 2: tam = 6; break;
        case 3: tam = 8; break;
        case 4: tam = 10; break;
        default:
            printf("Opção inválida!\n");
            sleep(2);
            return;
    }
    
    system("cls");
    printf("\n=== REGRAS ===\n");
    printf("• Você começa com 100 pontos\n");
    printf("• Cada acerto: +10 pontos\n");
    printf("• Cada erro: -10 pontos\n");
    printf("• Se os pontos chegarem a 0, você perde!\n");
    printf("\nPressione ENTER para começar...");
    getchar();
    getchar();
    
    // Inicializa pontuação do jogo
    jogador->pontos = 100;
    
    // Sequência do jogo
    inicializaJogo(tam);
    loopJogo(jogador, tam);
    finalizaJogo(jogador, tam);
}

void mostrarRanking() {
    system("cls");
    printf("\n=== RANKING - TOP 10 ===\n\n");
    
    FILE *lista = fopen("lista.txt", "r");
    if(!lista) {
        printf("Nenhum jogador cadastrado ainda!\n");
        printf("\nPressione ENTER para voltar...");
        getchar();
        getchar();
        return;
    }
    
    // Lê todos os jogadores
    Player players[100];
    int total = 0;
    
    while(fscanf(lista, "%79s %d", players[total].nome, &players[total].pontos) == 2) {
        total++;
        if(total >= 100) break;
    }
    fclose(lista);
    
    // Ordena por pontos (bubble sort simples)
    for(int i = 0; i < total - 1; i++) {
        for(int j = 0; j < total - i - 1; j++) {
            if(players[j].pontos < players[j+1].pontos) {
                Player temp = players[j];
                players[j] = players[j+1];
                players[j+1] = temp;
            }
        }
    }
    
    // Mostra top 10
    int limite = (total < 10) ? total : 10;
    for(int i = 0; i < limite; i++) {
        printf("%2d. %-20s %d pontos\n", i+1, players[i].nome, players[i].pontos);
    }
    
    printf("\nPressione ENTER para voltar...");
    getchar();
    getchar();
}

int main() {
    int menu;
    
    do {
        system("cls");
        printf("\n=================================\n");
        printf("     JOGO DA MEMORIA - MENU\n");
        printf("=================================\n");
        printf("1 -> Modo Individual\n");
        printf("2 -> Modo 2 Jogadores (em breve)\n");
        printf("3 -> Ranking\n");
        printf("4 -> Sair\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &menu);
        
        switch(menu) {
            case 1:
                getchar();
                Player jogador = login();
                dificuldade(&jogador);
                break;
                
            case 2:
                printf("\nModo 2 jogadores ainda não implementado!\n");
                printf("Pressione ENTER...");
                getchar();
                getchar();
                break;
                
            case 3:
                getchar();
                mostrarRanking();
                break;
                
            case 4:
                printf("\nObrigado por jogar! Até logo!\n");
                break;
                
            default:
                printf("\nOpção inválida!\n");
                sleep(1);
                break;
        }
    } while(menu != 4);
    
    return 0;
}