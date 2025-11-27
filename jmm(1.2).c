#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h> //Para a funcao sleep
#include<stdbool.h>
//#include <windows.h>


typedef struct {//struct de jogador
    char nome[80];
    int pontos;
}Player;
int pontuacao = 100;

//FUNCOES DE LOGIN/CRIACAO DE PLAYERS

int acha_pos(Player players[], int tamanho) {//Funcao de posicao livre na struct
    for (int i = 1; i < tamanho; i++) {
        if (players[i].nome[0] == '\0') {
            return i; // posição vazia
        }
    }
    return -1; // cheio
}

void addPlayer(Player players[], int tamanho) {//Funcao que adiciona player
    int pos = acha_pos(players, tamanho);
    if (pos == -1) {
        printf("Lista cheia!\n");
        return;
    }
    FILE *lista= fopen("lista.txt","a");
    printf("Nome do novo jogador: ");
    fgets(players[pos].nome, sizeof(players[pos].nome), stdin);
    players[pos].pontos = 0;
    players[pos].nome[strcspn(players[pos].nome, "\n")] = '\0';
    fprintf(lista, "%s %d\n", players[pos].nome, players[pos].pontos);
    fclose(lista);
}

void login(){//Funcao de login (Verifica se existe algum player com o nome escrito)
    FILE *lista= fopen("lista.txt","r");

    Player players[11]={0};
    char nomeArquivo[80];
    int pontos;
    bool chamaadd = true;
    printf("Bem vindo!\n");
    printf("Nome de Jogador: ");
    fgets(players[0].nome, sizeof(players[0].nome), stdin);

   while (fgets(nomeArquivo, sizeof(nomeArquivo), lista) != NULL) {

    fscanf(lista, "%79s %d", nomeArquivo, &pontos);
    players[0].nome[strcspn(players[0].nome, "\n")] = '\0'; // Tirar o \n
    nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0'; // Tirar o \n

        if (!strcmp(players[0].nome, nomeArquivo)) {
            printf("Bem-Vindo %s!\n",nomeArquivo);
            printf("Seu maior recorde foi: %d pontos!\n",pontos);
            chamaadd = false;
            break;
        }

    }
    fclose(lista);
    if(chamaadd){
    printf("Nenhum jogador com esse nick encontrado!\n");
    addPlayer(players, 11);
    }
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

void finaliza(){
    if(pontuacao <= 0){
        printf("Voce perdeu");
    }
    else{
        printf("Voce venceu! Sua pontuacao foi: %d", pontuacao);
    }
}

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


/*void habilitarANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    GetConsoleMode(hOut, &modo);
    modo |= 0x0004;
    SetConsoleMode(hOut, modo);
}
    */

void escolhecarta(int tam){//Funcao que possibilita o usuario escolher a carta
    if(msave == cartas){
        return;
    }
    Player players[11];
   // habilitarANSI();
    int x=0, y=0;
    char c;
    for(int i=0 ;i < 2; i++){
        while (1) {
            c = getchar();  // sem precisar apertar Enter

            if (c == 'w' && y > 0) y--;
            if (c == 's' && y < tam-1) y++;
            if (c == 'a' && x > 0) x--;
            if (c == 'd' && x < tam-1) x++;

            if (c == '\r') {  // Enter
                if(verify[y][x] != '!'){
                continue;
                }
            verify[y][x] = cartas[y][x]; 
            salvar(tam, pontuacao, y, x);
            }

        }
    }

    if(!(verificacartas(tam))){
        players[0].pontos -= 10;
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
        finaliza();
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
   struct player;
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

//Agora o que eu preciso é salvar, quando acabar o jogo (individual), o jogador na última posição livre, com seu nome e sua pontuação final.
//Preciso também implementar o código de um jeito que funcione para 2 jogadores também