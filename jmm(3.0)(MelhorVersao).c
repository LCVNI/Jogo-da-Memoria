#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
    #define LIMPAR "cls"
    #define SLEEP(x) Sleep((x)*1000)
#else
    #include <unistd.h>
    #define LIMPAR "clear"
    #define SLEEP(x) sleep(x)
#endif

// ==================== ESTRUTURAS ====================

typedef struct {
    char nome[80];
    int pontos;
    int paresEncontrados;
} Player;

typedef struct {
    char nomeJogador[80];
    int pontos;
    int paresEncontrados;
    int tamanho;
    char tabuleiro[10][10];    // Cartas reais
    char visivelSave[10][10];  // Estado visivel
} SaveGame;

// ==================== VARIAVEIS GLOBAIS ====================

char *simbolos = "ZWXP@$C#89F2HGM";
char **cartas;      // Tabuleiro com as respostas
char **visivel;     // O que o jogador ve

// ==================== DECLARACOES ANTECIPADAS ====================

void criarTabuleiro(int tam);
void destruirTabuleiro(int tam);

// ==================== FUNCOES AUXILIARES ====================

// Le uma linha inteira e remove o \n
void lerLinha(char *buffer, int tamanho) {
    fgets(buffer, tamanho, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

// Le um numero inteiro
int lerInt() {
    char buffer[100];
    lerLinha(buffer, sizeof(buffer));
    return atoi(buffer);
}

// Pausa ate pressionar ENTER
void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

// ==================== FUNCOES DE SAVE/LOAD ====================

// Verifica se existe um save
bool existeSave() {
    FILE *f = fopen("savegame.dat", "rb");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}

// Salva o estado atual do jogo
void salvarJogo(Player *jogador, int tam) {
    SaveGame save;
    
    strcpy(save.nomeJogador, jogador->nome);
    save.pontos = jogador->pontos;
    save.paresEncontrados = jogador->paresEncontrados;
    save.tamanho = tam;
    
    // Copia os tabuleiros
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

// Carrega o jogo salvo
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
    
    // Restaura dados do jogador
    strcpy(jogador->nome, save.nomeJogador);
    jogador->pontos = save.pontos;
    jogador->paresEncontrados = save.paresEncontrados;
    *tam = save.tamanho;
    
    // Cria os tabuleiros
    criarTabuleiro(*tam);
    
    // Restaura o estado dos tabuleiros
    for (int i = 0; i < *tam; i++) {
        for (int j = 0; j < *tam; j++) {
            cartas[i][j] = save.tabuleiro[i][j];
            visivel[i][j] = save.visivelSave[i][j];
        }
    }
    
    return true;
}

// Deleta o save
void deletarSave() {
    remove("savegame.dat");
}

// ==================== FUNCOES DE ARQUIVO ====================

// Carrega ou cria um jogador
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

// Salva recorde do jogador
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

// Mostra o ranking
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
    
    // Ordena por pontos (bubble sort)
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

// ==================== FUNCOES DO TABULEIRO ====================

// Cria e inicializa o tabuleiro
void criarTabuleiro(int tam) {
    cartas = malloc(tam * sizeof(char*));
    visivel = malloc(tam * sizeof(char*));
    
    for (int i = 0; i < tam; i++) {
        cartas[i] = malloc(tam * sizeof(char));
        visivel[i] = malloc(tam * sizeof(char));
        memset(cartas[i], 0, tam);
        memset(visivel[i], '?', tam);
    }
    
    // Preenche com pares aleatorios
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

// Mostra o tabuleiro
void mostrarTabuleiro(int tam, Player jogadores[], int numJogadores) {
    system(LIMPAR);
    
    printf("\n========== JOGO DA MEMORIA ==========\n\n");
    
    // Info dos jogadores
    for (int p = 0; p < numJogadores; p++) {
        printf("%s: %d pontos", jogadores[p].nome, jogadores[p].pontos);
        if (numJogadores > 1) {
            printf(" (%d pares)", jogadores[p].paresEncontrados);
        }
        printf("\n");
    }
    printf("\n");
    
    // Cabecalho
    printf("    ");
    for (int j = 0; j < tam; j++) {
        printf(" %d  ", j);
    }
    printf("\n");
    
    // Tabuleiro
    for (int i = 0; i < tam; i++) {
        printf(" %c  ", 'a' + i);
        for (int j = 0; j < tam; j++) {
            printf("[%c] ", visivel[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Libera memoria
void destruirTabuleiro(int tam) {
    for (int i = 0; i < tam; i++) {
        free(cartas[i]);
        free(visivel[i]);
    }
    free(cartas);
    free(visivel);
}

// Mostra cartas para memorizacao
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

// ==================== LOOP PRINCIPAL ====================

// Converte letra para indice
int letraParaIndice(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a';
    if (c >= 'A' && c <= 'Z') return c - 'A';
    return -1;
}

// Le uma posicao valida
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

// Loop do jogo (funciona para 1 ou 2 jogadores)
void jogar(Player jogadores[], int numJogadores, int tam, bool permitirSave) {
    int totalPares = (tam * tam) / 2;
    int vezDe = 0;
    
    // CRITICO: Inicializa IMEDIATAMENTE no inicio
    for (int i = 0; i < numJogadores; i++) {
        // Se pontos esta zerado OU tem lixo (valor absurdo), inicializa
        if (jogadores[i].pontos <= 0 || jogadores[i].pontos > 10000) {
            jogadores[i].pontos = 100;
        }
        // Sempre reseta pares (contador do jogo atual)
        if (jogadores[i].paresEncontrados < 0 || jogadores[i].paresEncontrados > 100) {
            jogadores[i].paresEncontrados = 0;
        }
    }
    
    // Loop principal
    while (1) {
        Player *atual = &jogadores[vezDe];
        
        // Verifica se perdeu por falta de pontos
        if (atual->pontos <= 0) {
            mostrarTabuleiro(tam, jogadores, numJogadores);
            printf("\n%s ficou sem pontos!\n", atual->nome);
            
            if (numJogadores > 1) {
                Player *outro = &jogadores[1 - vezDe];
                printf("\n*** %s VENCEU! ***\n", outro->nome);
            } else {
                printf("\n*** GAME OVER ***\n");
                if (permitirSave) deletarSave();
            }
            break;
        }
        
        // Verifica se todos os pares foram encontrados
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
                // Determina vencedor
                printf("\n");
                for (int i = 0; i < numJogadores; i++) {
                    printf("%s: %d pares, %d pontos\n", 
                           jogadores[i].nome, 
                           jogadores[i].paresEncontrados,
                           jogadores[i].pontos);
                }
                
                if (jogadores[0].paresEncontrados > jogadores[1].paresEncontrados) {
                    printf("\n*** %s VENCEU! ***\n", jogadores[0].nome);
                } else if (jogadores[1].paresEncontrados > jogadores[0].paresEncontrados) {
                    printf("\n*** %s VENCEU! ***\n", jogadores[1].nome);
                } else {
                    // Empate em pares, decide por pontos
                    if (jogadores[0].pontos > jogadores[1].pontos) {
                        printf("\n*** %s VENCEU por pontos! ***\n", jogadores[0].nome);
                    } else if (jogadores[1].pontos > jogadores[0].pontos) {
                        printf("\n*** %s VENCEU por pontos! ***\n", jogadores[1].nome);
                    } else {
                        printf("\n*** EMPATE TOTAL! ***\n");
                    }
                }
            }
            break;
        }
        
        // Mostra tabuleiro com opcao de salvar (apenas single player)
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
        
        // Executa turno
        int lin1, col1, lin2, col2;
        
        // Primeira carta
        lerPosicao(tam, &lin1, &col1, "Escolha a PRIMEIRA carta:");
        
        visivel[lin1][col1] = cartas[lin1][col1];
        mostrarTabuleiro(tam, jogadores, numJogadores);
        printf("Primeira: [%c] em %c%d\n", cartas[lin1][col1], 'a' + lin1, col1);
        SLEEP(1);
        
        // Segunda carta
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
        
        // Verifica
        int acertou = 0;
        if (cartas[lin1][col1] == cartas[lin2][col2]) {
            printf("\n*** ACERTOU! ***\n");
            atual->paresEncontrados++;
            pausar();
            acertou = 1;
        } else {
            printf("\n*** ERROU! -10 pontos ***\n");
            atual->pontos -= 10;
            visivel[lin1][col1] = '?';
            visivel[lin2][col2] = '?';
            pausar();
        }
        
        // Alterna jogador se errou (apenas em multiplayer)
        if (!acertou && numJogadores > 1) {
            vezDe = 1 - vezDe;
        }
    }
}

// ==================== MENUS ====================

void modoIndividual() {
    system(LIMPAR);
    
    char nome[80];
    printf("\n========== MODO INDIVIDUAL ==========\n\n");
    printf("Nome do jogador: ");
    lerLinha(nome, sizeof(nome));
    
    Player jogador = carregarJogador(nome);
    
    // Zera pares e pontos para jogo novo
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
    
    // Marca que eh um save (pontos > 0) para nao resetar paresEncontrados
    int pontosBackup = jogador.pontos;
    int paresBackup = jogador.paresEncontrados;
    
    // Salva o estado visivel atual
    char **visivelTemp = malloc(tam * sizeof(char*));
    for (int i = 0; i < tam; i++) {
        visivelTemp[i] = malloc(tam * sizeof(char));
        for (int j = 0; j < tam; j++) {
            visivelTemp[i][j] = visivel[i][j];
        }
    }
    
    // Mostra todas as cartas temporariamente
    mostrarMemorizacao(tam);
    
    // Restaura o estado visivel anterior (cartas ja descobertas)
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            visivel[i][j] = visivelTemp[i][j];
        }
        free(visivelTemp[i]);
    }
    free(visivelTemp);
    
    // Restaura valores que podem ter sido resetados
    jogador.pontos = pontosBackup;
    jogador.paresEncontrados = paresBackup;
    
    jogar(&jogador, 1, tam, true);
    
    if (jogador.pontos > 0) {
        salvarRecorde(&jogador);
    }
    
    destruirTabuleiro(tam);
    pausar();
}

void modoMultiplayer() {
    system(LIMPAR);
    
    Player jogadores[2] = {0}; // Inicializa tudo com zeros
    
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
    printf("- Cada um comeca com 100 pontos\n");
    printf("- Acerto: +1 par e joga de novo\n");
    printf("- Erro: -10 pontos e passa a vez\n");
    printf("- Vence quem fizer mais pares\n");
    pausar();
    
    criarTabuleiro(tam);
    mostrarMemorizacao(tam);
    jogar(jogadores, 2, tam, false);
    destruirTabuleiro(tam);
    pausar();
}

// ==================== MAIN ====================

int main() {
    int opcao;
    
    do {
        system(LIMPAR);
        printf("\n================================\n");
        printf("      JOGO DA MEMORIA\n");
        printf("================================\n");
        
        // Verifica se existe save
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
        
        // Menu adaptativo baseado em save
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