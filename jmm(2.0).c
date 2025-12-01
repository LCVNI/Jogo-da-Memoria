//ESSA VERSAO NAO TEM INTERECAO POR CURSOR NO TERMINAL

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h> //Para a funcao sleep
#include<stdbool.h>



typedef struct {//struct de jogador
    char nome[80];
    int pontos;
}Player;

int pontuacao = 100;

//FUNCOES DE LOGIN/CRIACAO DE PLAYERS

void addPlayer(Player players) {//Funcao que adiciona player
    
    FILE *lista= fopen("lista.txt","a");
    printf("Nome do novo jogador: ");
    fgets(players.nome, sizeof(players.nome), stdin);
    players.pontos = 0;
    players.nome[strcspn(players.nome, "\n")] = '\0';
    fprintf(lista, "%s %d\n", players.nome, players.pontos);
    fclose(lista);
}

void login(Player *p) { // Aceita um ponteiro para Player
    FILE *lista= fopen("lista.txt","r");

    char nomeLido[80]; // Variável temporária para nome lido do arquivo
    int pontosLidos;
    bool encontrado = false;

    printf("Bem vindo!\n");
    printf("Nome de Jogador: ");
    fgets(p->nome, sizeof(p->nome), stdin);
    p->nome[strcspn(p->nome, "\n")] = '\0';
    p->pontos = 0; // Inicializa pontos

    // Use fscanf para ler corretamente o par nome/pontos
    while (fscanf(lista, "%79s %d", nomeLido, &pontosLidos) == 2) {
        if (!strcmp(p->nome, nomeLido)) {
            printf("Bem-Vindo %s!\n", nomeLido);
            printf("Seu maior recorde foi: %d pontos!\n", pontosLidos);
            encontrado = true;
            break;
        }
    }

    fclose(lista);

    if (!encontrado) {
        printf("Nenhum jogador com esse nick encontrado!\n");
        addPlayer(*p); // Chama addPlayer com a struct preenchida
    }
}

    fclose(lista);
    printf("Nenhum jogador com esse nick encontrado!\n");
    addPlayer(players);
    
}
//--------------------------------------------------------------------------------------------//

//Variaveis para inicializar as cartas

char *simbols={"ZWXP@$C#89F2HGM"}; // Simbolos que vao aparecer nas cartas
char **cartas, **verify, **msave; //Ponteiro para ponteiro que vai virar uma matriz

//------------------------------------------------------------------------------------------//

//FUNCOES QUE ARMAZENAM E GERAM AS CARTAS

void armazena(int tam, const char *simbols){//Funcao que armazena as cartas em posicoes
                                            // aleatorias da matriz

   verify = malloc(tam * sizeof(char*));
   cartas = malloc(tam * sizeof(char*));// Transforma o ponteiro para ponteiro em vetor
   msave = malloc(tam * sizeof(char*));
   for(int i =0; i < tam; i++){
        cartas[i] = malloc(tam * sizeof(char)); //Transforma o vetor em matriz
        verify[i] = malloc(tam * sizeof(char));
        msave[i] = malloc(tam * sizeof(char));
   }

   //Inicializando a matriz com ?
   for(int i = 0; i < tam; i++){
    for(int j = 0; j < tam; j++){
        cartas[i][j] = '?';
        verify[i][j] = '!';
        msave[i][j] = '?';
    }
   }

    unsigned int idx=0;
    srand(time(NULL));
    
    for(int i=0; i < tam; i++){
        for(int j=0; j < tam; j++){
            int r1= rand() % tam; //Gerando as posicoes aleatórias
            int r2= rand () % tam;
            while(cartas[r1][r2] != '?'){ //Loop de confirmacao para evitar sobrescrever
                r1=rand() % tam;
                r2= rand() % tam;
            }
        if(idx >= strlen(simbols)){ // Quando o array de simbolos chega na posicao final
             idx=0;                 //ele e reiniciado 
        }

            cartas[r1][r2] = simbols[idx]; // Alocando os simbolos nas posicoes aleatorias

             if ((i * tam + j) % 2 == 1) { // Condicao que garante pares de simbolos 
               idx++;                      //repetindo o indice duas vezes
             }
        }
    }
}


void geracartas(int tam, char *simbols){ //Funcao que gera as cartas
    armazena(tam, simbols);
    printf("Memorize!\n");
    for(int i=0; i < tam; i++){
        for(int j=0; j < tam; j++){
            printf("[%c]",cartas[i][j]);
        }
    printf("\n");
    }

    for(int i = 3; i >= 0; i--){
        sleep(1);
        printf("\r%d...", i); // Contador de tempo

    }
    system("cls");

    for(int i=0; i < tam; i++){
        for(int j=0; j < tam; j++){
            printf("[?]");
        }
    printf("\n");
    }
}


//-------------------------------------------------------------------------------------------//

//FUNCOES QUE SALVAM O JOGO

void salvar(int tam, int pountuacao, int y, int x){
    msave[y][x] = verify[y][x];
    FILE *saved = fopen("save.bin", "wb");
    fwrite(msave, sizeof(char), tam*tam, saved);
    fprintf(saved,"%d",pontuacao);
    fclose(saved);
    printf("Salvo com sucesso!");
    return;
}

//FUNCAO QUE FINALIZA O JOGO, MOSTTRANDO STATUS

/*void finalizaJogo(){
    if(pontuacao <= 0){
        printf("Voce perdeu");
    }
    else{
        printf("Voce venceu! Sua pontuacao foi: %d", pontuacao);
    }
    FILE *lista= fopen("lista.txt","r");

    Player players;
    char nomePlayer[80];
    int pontos;
    while (fgets(nomePlayer, sizeof(nomePlayer), lista) != NULL) {

    fscanf(lista, "%79s %d", nomePlayer, &pontos);
    players.nome[strcspn(players.nome, "\n")] = '\0'; // Tirar o \n
    nomePlayer[strcspn(nomePlayer, "\n")] = '\0'; // Tirar o \n

        if (!strcmp(players.nome, nomePlayer)) {
            FILE *lista = fopen("lista.txt", "r+");
            
            
        }

    }
    
}
*/
//FUNCOES PARA A ESCOLHA E VERIFICACAO DE CARTAS

int verificacartas(int tam){ // Verificando posicoes
    int cont=0;
    for(int i = 0; i < tam; i++){
        for(int j=0; j < tam; j++){
            if(verify[i][j] == cartas[i][j]){
                cont++;
            }
        }
    }
    for(int i = 0; i < tam; i++){ //Reinicializando para nao dar conflito
        for(int j = 0; j < tam; j++){
            verify[i][j] = '!';
        }
    }
    if(cont == 2) return 1;
    else return 0;

}



void escolhecarta(int tam){//Funcao que possibilita o usuario escolher a carta
    Player players;
    if(msave == cartas){
        return;
    }
    Player players;
    enum linhas{a, b, c, d, e, f, g, h, i, j, k};
    int lin, col;
    char tmp;
    printf("Informe a linha e a coluna:/n");
    printf("Linha:   ");
    printf("Coluna:  ");
    scanf(" %c %d", &tmp, &col);
    

    if(!(verificacartas(tam))){
        players.pontos -= 10;
        pontuacao -= 10;
        return;
    }
    return;
}

void dificuldade(){ // Funcao para o usuario escolher o nivel de jogo
    int modo;
    do{
    printf("Nivel de dificuldade:");
    printf("\n1 -> Facil (16 pares)");
    printf("\n2 -> Normal (25 pares)");
    printf("\n3 -> Dificil (49 pares)\n");
    printf("4 -> ??????????\n");
    scanf("%d", &modo);
    system("cls");
    printf("Atencao!\n Voce tem 100 pontos e a cada erro voce perde 10!\n");
    printf("\nPressione Qualquer tecla para iniciar...");
    getchar();
    getchar();
    system("cls");
    switch(modo){
        case 1:
        geracartas(4, simbols);
        escolhecarta(4);
        finalizaJogo();
        break;

        case 2:
        geracartas(6, simbols);
        break;

        case 3:
        geracartas(8, simbols);
        break;

        case 4:
        printf("VOCE DESBLOQUEOU O MODO IMPOSSIVEL\n");
        geracartas(10, simbols);
        break;

        default:
        printf("Opcao invalida!");
        break;
    }
    }while(modo > 4);

}


int main(){
   Player players;
   int menu;
    do{
    printf("Escolha o modo desejado:");
    printf("\n1 -> Individual ");
    printf("\n2 -> Contra (2 Jogadores)");
    printf("\n3 -> Ranking de Players");
    printf("\n4 -> Sair\n");
    scanf("%d", &menu);
    system("cls");
    switch(menu){

        case 1:
        getchar(); // Evitar problemas com o buffer \n
        login();
        dificuldade();
        break;
        case 2:
        printf("Chamar modo 2 jogdores");
        break;
        case 3:
        printf("\nChamar funcao que mostra o ranking (Arquivo) ");
        break;
        case 4:
        printf("Saindo...");
        break;
        default:
        printf("Opcao inválida!");
        break;
    }
    }while(menu != 4);
    printf("\nSaiu!");
    return 0;
}

// Agora o que eu preciso é salvar, quando acabar o jogo (individual), o jogador na última posição livre, com seu nome e sua pontuação final.
// Preciso implementar o código de um jeito que funcione para 2 jogadores também
// A struct só vai ter as posições quando for ler os 10 primeiros players da lista
// Para colocar na posição certa, vou precisar comparar as 10 maiores pontuações de todo o arquivo.