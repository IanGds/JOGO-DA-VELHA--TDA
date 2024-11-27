#include <stdio.h>   
#include <stdlib.h>  // malloc & free
#include <string.h> 
#include <time.h>    

#define TAMANHO 3    





// armazena informações dos jogadores no ranking - isso vai ser usado depois no código
typedef struct {

    char nome[50];   
    int vitorias;   
} Jogador;


void limparTela() {
    #ifdef _WIN32
        system("cls");   // windows
    #else
        system("clear"); //linux
    #endif

    
}


// inicializa o tabuleiro - malloc (alocação dinamica)
char** inicializarTabuleiro() {
    
    char** tabuleiro = (char**)malloc(TAMANHO * sizeof(char*));
   
    // Inicializa tudo em branco
    for (int i = 0; i < TAMANHO; i++) {
        tabuleiro[i] = (char*)malloc(TAMANHO * sizeof(char));
        for (int j = 0; j < TAMANHO; j++) {
            tabuleiro[i][j] = ' ';  // esapço em branco
        }
    }
    return tabuleiro;



}


void liberarTabuleiro(char** tabuleiro) {
    for (int i = 0; i < TAMANHO; i++) {
        free(tabuleiro[i]);  
    }
    free(tabuleiro);  // Libera o ponteiro


}

// coloca separadores no tabuleiro
void imprimirTabuleiro(char** tabuleiro) {



    printf("\n");
    for (int i = 0; i < TAMANHO; i++) {
        

        printf(" %c | %c | %c \n", tabuleiro[i][0], tabuleiro[i][1], tabuleiro[i][2]);
        if (i < TAMANHO - 1) {
            printf("---+---+---\n");
        }
    }
    printf("\n");
}

// Verifica se já há vencedor
int verificarVencedor(char** tabuleiro, char jogador) {


    // linhas
    for (int i = 0; i < TAMANHO; i++) {
        if (tabuleiro[i][0] == jogador &&
            tabuleiro[i][1] == jogador &&
            tabuleiro[i][2] == jogador) {


            return 1;  // Linha completa
        }
    }
   
    //colunas
    for (int j = 0; j < TAMANHO; j++) {
        if (tabuleiro[0][j] == jogador &&
            tabuleiro[1][j] == jogador &&
            tabuleiro[2][j] == jogador) {
            return 1;  // Coluna completa
        }
    }
   
    //diagonais
    if ((tabuleiro[0][0] == jogador &&
         tabuleiro[1][1] == jogador &&
         tabuleiro[2][2] == jogador) ||
        (tabuleiro[0][2] == jogador &&
         tabuleiro[1][1] == jogador &&
         tabuleiro[2][0] == jogador)) {


        return 1;  // Diagonal completa
    }
   
    return 0;  // Sem um vencedor ainda
}

// tabuleiro completo ?
int tabuleiroCompleto(char** tabuleiro) {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (tabuleiro[i][j] == ' ') {
                return 0;  // 0 = tabuleiro  vazio
            }
        }
    }
    return 1;  // 
}

//loopcomputador: só joga em lugares livres
void jogadaComputador(char** tabuleiro) {
    int linha, coluna;
    do {
        linha = rand() % TAMANHO;
        coluna = rand() % TAMANHO;
    } while (tabuleiro[linha][coluna] != ' ');  //vazio
   
    tabuleiro[linha][coluna] = 'O';  //jogada
}

// ranking txt
void atualizarRanking(const char* nome) {
    FILE* arquivo = fopen("ranking.txt", "a+");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de ranking!\n"); // mensagem de erro
        return;
    }
   
    Jogador jogadores[100];  
    int numJogadores = 0;
    int encontrado = 0;
   
    //jogadores já existentes
    while (fscanf(arquivo, "%s %d", jogadores[numJogadores].nome,
                 &jogadores[numJogadores].vitorias) == 2) {
        // add vitórias a esses jogadores
        if (strcmp(jogadores[numJogadores].nome, nome) == 0) {
            jogadores[numJogadores].vitorias++;
            encontrado = 1;
        }
        numJogadores++;
    }
   
    // caso o jogador não esteja cadastrado
    if (!encontrado) {
        strcpy(jogadores[numJogadores].nome, nome);
        jogadores[numJogadores].vitorias = 1;
        numJogadores++;
    }
   
    // atualiza ranking
    fclose(arquivo);
    arquivo = fopen("ranking.txt", "w");
   
    for (int i = 0; i < numJogadores; i++) {
        fprintf(arquivo, "%s %d\n", jogadores[i].nome, jogadores[i].vitorias);
    }
   
    fclose(arquivo);
}
// exibe o arquivo atualizado com as informações que foram obtidas
void mostrarRanking() {
    FILE* arquivo = fopen("ranking.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum ranking disponível ainda!\n");
        return;
    }
   
    printf("\n=== RANKING ===\n");
    char nome[50];
    int vitorias;
    while (fscanf(arquivo, "%s %d", nome, &vitorias) == 2) {
        printf("%s: %d vitória(s)\n", nome, vitorias);
    }
   
    fclose(arquivo);
}

//créditos
void mostrarCreditos() {
    printf("\n=== CRÉDITOS ===\n");
    printf("Jogo da Velha v1.0\n");
    printf("Descrição: Projeto de um jogo da velha em C com suporte a dois modos de jogo\n");
    printf("Desenvolvido por: Ian Lucas, Miguel Araújo, Italo Henrique e João Victor\n");
    printf("Mentor: Walace Sartori Bonfim\n");
    printf("Data: 26/11/2024\n");
}


//função principal do jogo inicializada quando o usuário tecla 1
void jogar() {
    
    char** tabuleiro = inicializarTabuleiro();
    char nome1[50];  
    char nome2[50];  
    int modo;
   
    //nome 
    printf("Digite seu nome (Jogador 1): ");
    scanf("%s", nome1);
   
    //modo
    printf("Escolha o modo de jogo:\n");
    printf("1 - Jogador vs Computador\n");
    printf("2 - Jogador 1 vs Jogador 2\n");
    scanf("%d", &modo);
   
    // nome do player 2
    if (modo == 2) {
        printf("Digite o nome do Jogador 2: ");
        scanf("%s", nome2);
    }
   
    int jogadorAtual = 1;  
    int linha, coluna;
   
    // Loop da partida
    while (1) {
        limparTela();
        imprimirTabuleiro(tabuleiro);
       
        //jogada com base nas posições da matriz
        if (jogadorAtual == 1) {
            printf("Vez do %s (X)\n", nome1);
            printf("Digite a linha (0-2) e coluna (0-2): ");
            scanf("%d %d", &linha, &coluna);
           
            // vê se a posição está livre ou não
            if (linha >= 0 && linha < TAMANHO &&
                coluna >= 0 && coluna < TAMANHO &&
                tabuleiro[linha][coluna] == ' ') {
                tabuleiro[linha][coluna] = 'X';
               
                
                if (verificarVencedor(tabuleiro, 'X')) {
                    limparTela();
                    imprimirTabuleiro(tabuleiro);
                    printf("Parabéns %s! Você venceu!\n", nome1);
                    atualizarRanking(nome1);

                    break;
                }
               
                jogadorAtual = 2;  // Próximo jogador
            } else {
                printf("Jogada inválida! Tente novamente.\n");
                getchar(); getchar();  // Pausa
                continue;
            }
        } else {
            // Vez do P2 ou Computador
            if (modo == 1) {
                jogadaComputador(tabuleiro);
            } else {


                printf("Vez do %s (O)\n", nome2);
                printf("Digite a linha (0-2) e coluna (0-2): ");
                scanf("%d %d", &linha, &coluna);
               
                
                if (linha >= 0 && linha < TAMANHO &&
                    coluna >= 0 && coluna < TAMANHO &&
                    tabuleiro[linha][coluna] == ' ') {
                    tabuleiro[linha][coluna] = 'O';
                } else {
                    printf("Jogada inválida! Tente novamente.\n");
                    getchar(); getchar();  // esse getchar é para pausa
                    continue;
                }
            }
           
            // Verifica vitória do player 2
            if (verificarVencedor(tabuleiro, 'O')) {


                limparTela();
                imprimirTabuleiro(tabuleiro);
                if (modo == 1) {
                    printf("O computador venceu !\n");
                } else {
                    printf("Parabéns %s! Você venceu !\n", nome2);
                    atualizarRanking(nome2);
                }
                break;
            }
           
            jogadorAtual = 1;  // Volta para o p1
        }
       
        // Verifica empate
        if (tabuleiroCompleto(tabuleiro)) {
            limparTela();
            imprimirTabuleiro(tabuleiro);
            printf("Empate!\n");
            break;
        }
    }
   
    // quando o jogo terminar essa parte vai ser a responsável por liberar a memória
    liberarTabuleiro(tabuleiro);
    printf("\nPressione ENTER para continuar...");
    getchar(); getchar();
}
// função do menu
int main() {
    
    srand(time(NULL));
   
    int opcao;
   
    //aqui usamos um do while para que o jogo fique rodando de maneira pérpetua caso o usuário deseje, ou, ao menos uma vez
    do {


        limparTela();
        printf("=== JOGO DA VELHA ===\n");
        printf("1. Jogar\n");
        printf("2. Ver Ranking\n");
        printf("3. Créditos\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
       
        
        switch (opcao) {

            case 1: jogar(); break;
            case 2:
                mostrarRanking();
                printf("\nPressione ENTER para continuar...");
                getchar(); getchar();
                break;
            case 3:
                mostrarCreditos();
                printf("\nPressione ENTER para continuar...");
                getchar(); getchar();
                break;
            case 4:
                printf("Obrigado por jogar!\n");
                break;
            default:
                printf("Opção inválida!\n");
                break;
        }
    } while (opcao != 4); // 4 = saída
   
    return 0;
}