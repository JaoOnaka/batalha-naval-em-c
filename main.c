#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

/*Informações sobre os barcos

Navios( 2x ): #  #  #

Porta aviões ( 1x ): #  #  #  #

Hidro aviões ( 2x ): .  #  .
                     #  .  #

*/

//DECLARAÇÃO DE VARIAVEIS GLOBAIS
struct usuario
{
    char nomeJogador[20];
    int vezesJogadas;
};
int jogadas=0;

//DECLARAÇÃO DE FUNÇÕES
void limpaPrompt1();
void intro();
void menuPrincipal();
void jogo();
int tamanhoTabuleiro();
void criarTabuleiro(int t, char jogador2[20]);
void colocaBarcosJogador(int t, int tab[][t]);
void colocaBarcosCPU(int t, int tab[][t]);
void opcoesBarcos(int n, int pa, int ha);
int validaBarco(int n, int pa, int ha);
void pegaPos(int posXY[2], int t, int escolha);
int sorteio(int jog,int cpu);
void quemComeca(int t);
int rodadas(int turno, int t,int tabJog[][t], int tabCPU[][t]);
void tiroJogador(int tiroXY[2], int t);
void tiroCPU(int tiroXY[2], int t);
void organizaRanking();
void mostraRanking();
int sair();

//---------------------------------------------

//ARGUMENTOS DAS FUN��ES
void gotoxy(int x, int y){
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x-1,y-1});
}

///Limpa Prompt
void limpaPrompt1(){
    gotoxy(4,20);
    for(int x=0;x<15;x++){
        printf("\n                                                                                  ");
    }
    gotoxy(4,20);
}

///Intro
void intro(){
    printf("\n||===========================================================||");
    printf("\n||                      Batalha Naval                        ||");
    printf("\n||        Trabalho de Algoritmos e Logica de Programacao     ||");
    printf("\n||===========================================================||");
    getch();
}

///Menu Principal
void menuPrincipal(){
    int opc;
    int ok=0;
    int certeza;
    system("cls");
    printf("\n||===========================================================||");
    printf("\n||                                                           ||");
    printf("\n||                      MENU PRINCIPAL                       ||");
    printf("\n||                                                           ||");
    printf("\n||===========================================================||");
    printf("\n||                                                           ||");
    printf("\n||                    [1]Iniciar o jogo                      ||");
    printf("\n||                    [2]Ranking                             ||");
    printf("\n||                    [0]Sair                                ||");
    printf("\n||                                                           ||");
    printf("\n||===========================================================||");
    while(ok==0){
        printf("\nOpcao: ");
            scanf("%d%*c",&opc);
        if(opc>=0 && opc<=2){
            ok=1;
        }
    }
    if(opc==0){
        certeza = sair();
        if(certeza==1){
            system("cls");
            printf("Obrigado por jogar!");
            system("break");
        } else {
            menuPrincipal();
        }
    } else if(opc==1){
        jogo();
    } else if(opc==2){
        organizaRanking();
        mostraRanking();
    }
}

///Jogo
void jogo(){
    struct usuario jogador2;
    int tamanho;
    int jogador=1,cpu=0,turno;
    int x, y;
    int ganhador;
    int opc;

    //Arquivo
    FILE* ranking = NULL;
    ranking = fopen("ranking.txt","a");
    if(ranking==NULL){
        printf("Erro na abertura do arquivo.");
        system("pause");
        exit(1);
    }
    printf("Primeiramente, digite o nome do capitao: ");
    scanf("%[^\n]s", jogador2.nomeJogador); // %[^\n]s --> Significa que na primeira vez que o "\n" (quebra de linha) aparecer, a leitura � interrompida, evitando espa�os indesejados
    system("cls");
    tamanho = tamanhoTabuleiro(jogador2.nomeJogador);
    int tabJog[tamanho][tamanho]; //declarando o tabuleiro do jogador
    int tabCPU[tamanho][tamanho]; //declarando o tabuleiro do jogador
    criarTabuleiro(tamanho, jogador2.nomeJogador);
    for(x=0;x<tamanho;x++){
        for(y=0;y<tamanho;y++){
            tabJog[x][y]=0; //Tabuleiro do Jogador (0 = vazio)
        }
    }
    for(x=0;x<tamanho;x++){
        for(y=0;y<tamanho;y++){
            tabCPU[x][y]=0; //Tabuleiro da CPU (0 = vazio)
        }
    }
    colocaBarcosJogador(tamanho, tabJog);
    colocaBarcosCPU(tamanho, tabCPU);

    turno = sorteio(jogador,cpu);
    quemComeca(turno);
    ganhador = rodadas(turno, tamanho, tabJog, tabCPU);

    jogador2.vezesJogadas = jogadas;
    if(ganhador==0){
        printf("CPU ganhou!");
    } else {
        printf("%s ganhou! Com %d jogadas", jogador2.nomeJogador, jogador2.vezesJogadas);
        fprintf(ranking, "%s %d\n", jogador2.nomeJogador, jogador2.vezesJogadas);
    }
    fclose(ranking);


    printf("\nDeseja jogar novamente?");
    printf("\n[1] Sim");
    printf("\n[2] Nao");
    printf("\nOpcao:");
    scanf("%d%*c",&opc);
    if(opc==1){
        menuPrincipal();
    } else if(opc==2){
        system("break");
    }

}

///Tamanho do tabuleiro
int tamanhoTabuleiro(jogador2){
    int tam, tamanho;
    int ok=0;
    printf(" Ola, %s, e um prazer te-lo(a) em nossa tripulacao!", jogador2);
    printf("\n Para comecar, deseja jogar com qual tamanho de tabuleiro?");
    printf("\n [1] 10x10");
    printf("\n [2] 15x15\n");
    while(ok==0){
        printf(" Opcao: ");
            scanf("%d%*c", &tam);
        if(tam>=1 && tam<=2){
            ok=1;
        }
    }
    if(tam==1){
        tamanho=10;
    } else {
        tamanho=15;
    }
    return tamanho;
}

///Criar tabuleiro
void criarTabuleiro(int t, char jogador2[20]){
    int x, y;
    system("cls");

    //USU�RIO
    gotoxy(4, 2);
    printf("Nome: %s", jogador2);
    for(x=0;x<t;x++){
        gotoxy(x*3+10,3);
        printf("%d",x+1);
    }
    printf("\n     ------------------------------------------------------------------------------------------------------------");
    for(x=0;x<t;x++){
        for(y=0;y<t;y++){
            gotoxy(x*3+10,y+5);
            printf(".");
        }
    }
    for(y=0;y<t;y++){
        gotoxy(4,y+5);
        printf("%3d",y+1);
    }
    for(y=0;y<t;y++){
        gotoxy(7,y+5);
        printf(" |");
    }
    //CPU
    gotoxy(61, 2);
    printf("Nome: CPU");

    for(x=0;x<t;x++){
        gotoxy(x*3+65,3);
        printf("%d",x+1);
    }

    for(x=0;x<t;x++){
        for(y=0;y<t;y++){
            gotoxy(x*3+65,y+5);
            printf(".");
        }
    }
    for(y=0;y<t;y++){
        gotoxy(59,y+5);
        printf("%3d",y+1);
    }
    for(y=0;y<t;y++){
        gotoxy(62,y+5);
        printf(" |");
    }
}

///Coloca Barcos Jogador
void colocaBarcosJogador(int t, int tab[][t]){
    int barcos=5;
    int qtNavio=2;
    int qtPortaAv=1;
    int qtHidroAv=2;
    int escolha;
    int posXY[2]; //Coordenadas desejadas
    while(barcos>0){
        opcoesBarcos(qtNavio, qtPortaAv, qtHidroAv);
        escolha = validaBarco(qtNavio, qtPortaAv, qtHidroAv);
        pegaPos(posXY, t, escolha); //posXY[0] = X     //     posXY[1] = Y
        if(escolha==1){
            if(tab[posXY[0]-1][posXY[1]-1]==0 && tab[posXY[0]-1][posXY[1]]==0 && tab[posXY[0]-1][posXY[1]+1]==0){
                tab[posXY[0]-1][posXY[1]-1]=1;
                tab[posXY[0]-1][posXY[1]]=1;
                tab[posXY[0]-1][posXY[1]+1]=1;

                gotoxy((posXY[1]-1)*3+10,(posXY[0])+4);
                printf("#");

                gotoxy((posXY[1]-1)*3+13,(posXY[0])+4);
                printf("#");

                gotoxy((posXY[1]-1)*3+16,(posXY[0])+4);
                printf("#");
                qtNavio--;
                barcos--;
            }
        } else if(escolha==2){
            if(tab[posXY[0]-1][posXY[1]-1]==0 && tab[posXY[0]-1][posXY[1]+1]==0 && tab[posXY[0]-1][posXY[1]+1]==0 && tab[posXY[0]-1][posXY[1]+2]==0){
                tab[posXY[0]-1][posXY[1]-1]=1;
                tab[posXY[0]-1][posXY[1]]=1;
                tab[posXY[0]-1][posXY[1]+1]=1;
                tab[posXY[0]-1][posXY[1]+2]=1;
                gotoxy((posXY[1]-1)*3+10,(posXY[0])+4);
                printf("#");
                gotoxy((posXY[1]-1)*3+13,(posXY[0])+4);
                printf("#");
                gotoxy((posXY[1]-1)*3+16,(posXY[0])+4);
                printf("#");
                gotoxy((posXY[1]-1)*3+19, (posXY[0])+4);
                printf("#");
                qtPortaAv--;
                barcos--;
            }
        } else if(escolha==3){
            if(tab[posXY[0]][posXY[1]-1]==0 && tab[posXY[0]-1][posXY[1]]==0 && tab[posXY[0]][posXY[1]+1]==0){
                tab[posXY[0]][posXY[1]-1]=1;
                tab[posXY[0]-1][posXY[1]]=1;
                tab[posXY[0]][posXY[1]+1]=1;
                gotoxy((posXY[1]-1)*3+10,(posXY[0])+5);
                printf("#");
                gotoxy((posXY[1]-1)*3+13,(posXY[0])+4);
                printf("#");
                gotoxy((posXY[1]-1)*3+16,(posXY[0])+5);
                printf("#");
                qtHidroAv--;
                barcos--;
            }
        }
        limpaPrompt1();
        printf("\n\n\n\n\n\n\n");
    }
}

///Coloca Barcos CPU
void colocaBarcosCPU(int t, int tab[][t]){
    int barcos=5;
    int qtNavio=2;
    int qtPortaAv=1;
    int qtHidroAv=2;
    int escolha;
    int posXY[2]; //Coordenadas desejadas
    while(barcos>0){
        opcoesBarcos(qtNavio, qtPortaAv, qtHidroAv);
        escolha = validaBarcoCPU(qtNavio, qtPortaAv, qtHidroAv);
        pegaPosCPU(posXY, t, escolha); //posXY[0] = X     //     posXY[1] = Y
        if(escolha==1){
            if(tab[posXY[0]-1][posXY[1]-1]==0 && tab[posXY[0]-1][posXY[1]]==0 && tab[posXY[0]-1][posXY[1]+1]==0){
                tab[posXY[0]-1][posXY[1]-1]=1;
                tab[posXY[0]-1][posXY[1]]=1;
                tab[posXY[0]-1][posXY[1]+1]=1;

                qtNavio--;
                barcos--;
            }
        } else if(escolha==2){
            if(tab[posXY[0]-1][posXY[1]-1]==0 && tab[posXY[0]-1][posXY[1]+1]==0 && tab[posXY[0]-1][posXY[1]+1]==0 && tab[posXY[0]-1][posXY[1]+2]==0){
                tab[posXY[0]-1][posXY[1]-1]=1;
                tab[posXY[0]-1][posXY[1]]=1;
                tab[posXY[0]-1][posXY[1]+1]=1;
                tab[posXY[0]-1][posXY[1]+2]=1;

                qtPortaAv--;
                barcos--;
            }
        } else if(escolha==3){
            if(tab[posXY[0]][posXY[1]-1]==0 && tab[posXY[0]-1][posXY[1]]==0 && tab[posXY[0]][posXY[1]+1]==0){
                tab[posXY[0]][posXY[1]-1]=1;
                tab[posXY[0]-1][posXY[1]]=1;
                tab[posXY[0]][posXY[1]+1]=1;

                qtHidroAv--;
                barcos--;
            }
        }
        limpaPrompt1();
    }
}

///Op��es de Barcos
void opcoesBarcos(int n, int pa, int ha){
    gotoxy(4,21);
    printf("- = < Opcoes de Barcos > = -");
    if(n>0){
        printf("\n     [1] Navio (x%d)", n);
    }
    if(pa>0){
        printf("\n     [2] Porta-Aviao (x%d)", pa);
    }
    if(ha>0){
        printf("\n     [3] Hidro-Aviao (x%d)", ha);
    }
}

///Valida��o de Barco Jogador
int validaBarco(int n, int pa, int ha){
    int escolha;
    int ok=0;
    while(ok==0){
        printf("\n   Opcao: ");
        scanf("%d%*c", &escolha);
        if(escolha>0 && escolha<=3){
            if(escolha==1 && n>0){
                ok=1;
                return 1;
            } else if(escolha==2 && pa>0){
                ok=1;
                return 2;
            } else if(escolha==3 && ha>0){
                ok=1;
                return 3;
            } else {
                printf("\n   Tente novamente, opcao invalida.");
            }
        } else {
            printf("\n   Tente novamente, opcao invalida.");
        }
    }
}

///Valida��o de Barco CPU
int validaBarcoCPU(int n, int pa, int ha){
    int escolha;
    int ok=0;
    while(ok==0){
        escolha = rand() % 4;
        if(escolha>0 && escolha<=3){
            if(escolha==1 && n>0){
                ok=1;
                return 1;
            } else if(escolha==2 && pa>0){
                ok=1;
                return 2;
            } else if(escolha==3 && ha>0){
                ok=1;
                return 3;
            }
        }
    }
}

///Coordenadas Jogador
void pegaPos(int posXY[2],int t, int escolha){
    int ok=0;
    while(ok==0){
        printf("   Linha: ");
        scanf("%d", &posXY[0]);
        if(escolha==1){
            if(posXY[0]<=t && posXY[0]>0){
                ok=1;
            } else {
                printf("Coordenada invalida, tente novamente.");
            }
        } else if(escolha==2){
            if(posXY[0]<=t && posXY[0]>0){
                ok=1;
            } else {
                printf("Coordenada invalida, tente novamente.");
            }
        } else if(escolha==3){
            if(posXY[0]<=t-1 && posXY[0]>0){
                ok=1;
            } else {
                printf("Coordenada invalida, tente novamente.");
            }
        }
    }
    ok=0;
    while(ok==0){
        printf("   Coluna: ");
        scanf("%d", &posXY[1]);
        if(escolha==1){
            if(posXY[1]<=t-2 && posXY[1]>0){
                ok=1;
            } else {
                printf("Coordenada invalida, tente novamente.");
            }
        } else if(escolha==2){
            if(posXY[1]<=t-3 && posXY[1]>0){
                ok=1;
            } else {
                printf("Coordenada invalida, tente novamente.");
            }
        } else if(escolha==3){
            if(posXY[1]<=t-2 && posXY[1]>0){
                ok=1;
            } else {
                printf("Coordenada invalida, tente novamente.");
            }
        }
    }
}

///Coordenadas CPU
void pegaPosCPU(int posXY[2],int t, int escolha){
    int ok=0;
    while(ok==0){
        posXY[0] = rand() % t+1;
        if(escolha==1){
            if(posXY[0]<=t && posXY[0]>0){
                ok=1;
            }
        } else if(escolha==2){
            if(posXY[0]<=t && posXY[0]>0){
                ok=1;
            }
        } else if(escolha==3){
            if(posXY[0]<=t-1 && posXY[0]>0){
                ok=1;
            }
        }
    }
    ok=0;
    while(ok==0){
        posXY[1] = rand() % t+1;
        if(escolha==1){
            if(posXY[1]<=t-2 && posXY[1]>0){
                ok=1;
            }
        } else if(escolha==2){
            if(posXY[1]<=t-3 && posXY[1]>0){
                ok=1;
            }
        } else if(escolha==3){
            if(posXY[1]<=t-2 && posXY[1]>0){
                ok=1;
            }
        }
    }
}

///Sorteio de primeira jogada
int sorteio(int jog,int cpu){
    int sort;
    sort = rand() % 2;
    if(sort==jog){
        return 1;
    } else {
        return 0;
    }
}

///Quem come�a
void quemComeca(int t){
    gotoxy(4,19);
    if(t==1){
        printf("\n\n   Voce comeca jogando. Boa sorte!");
    } else {
        printf("\n\n   O computador comeca jogando. Boa sorte!");
    }
}

///Rodadas
int rodadas(int turno, int t,int tabJog[][t], int tabCPU[][t]){
    int fim=0;
    jogadas = 0;
    int tiroXY[2]; //tiroXY[0] = X     // tiroXY[1] = Y
    int tiroValido;
    int acertou;
    int x, y;
    int ok;
    int ganhador;

    do{
        while(turno==1){
            tiroValido=0;
            while(tiroValido==0){
                tiroJogador(tiroXY, t);
                if(tabCPU[tiroXY[0]-1][tiroXY[1]-1]!=2 && tabCPU[tiroXY[0]-1][tiroXY[1]-1]!=3){
                    tiroValido=1;
                }
            }

            printf("\n\n\n\n\n\n\n");

            if(tabCPU[tiroXY[0]-1][tiroXY[1]-1]==1){
                acertou=1;
            } else {
                acertou = 0;
            }
            if(acertou==1){
                tabCPU[tiroXY[0]-1][tiroXY[1]-1]=2;
                gotoxy(tiroXY[1]*3+62,tiroXY[0]+4);
                printf("X");
            } else {
                limpaPrompt1();
                if(tabCPU[tiroXY[0]-1][tiroXY[1]-1]!=2 && tabCPU[tiroXY[0]-1][tiroXY[1]-1]!=3){
                    tabCPU[tiroXY[0]-1][tiroXY[1]-1]=3;
                    printf("\nErrou!");
                    gotoxy(tiroXY[1]*3+62,tiroXY[0]+4);
                    printf("O");
                    turno=0;
                }
            }
            //Verifica se acabou
            ok=0;
            for(x=0;x<t;x++){
                for(y=0;y<t;y++){
                    if(tabCPU[x][y]==1){
                        ok++;
                    }
                }
            }
            if(ok==0){
                turno=2;
                ganhador=1;
                fim=1;
            }
            jogadas++;//for verifica matriz tabuleiro inteira
        }



        while(turno==0){
            tiroValido=0;
            while(tiroValido==0){
                tiroCPU(tiroXY, t);
                if(tabJog[tiroXY[0]-1][tiroXY[1]-1]!=2 && tabCPU[tiroXY[0]-1][tiroXY[1]-1]!=3){
                    tiroValido=1;
                }
            }

            if(tabJog[tiroXY[0]-1][tiroXY[1]-1]==1){
                acertou=1;
            } else {
                acertou = 0;
            }
            if(acertou==1){
                tabJog[tiroXY[0]-1][tiroXY[1]-1]=2;
                gotoxy(tiroXY[1]*3+7,tiroXY[0]+4);
                printf("X");
            } else {
                limpaPrompt1();
                if(tabJog[tiroXY[0]-1][tiroXY[1]-1]!=2 && tabJog[tiroXY[0]-1][tiroXY[1]-1]!=3){
                    tabJog[tiroXY[0]-1][tiroXY[1]-1]=3;
                    gotoxy(tiroXY[1]*3+7,tiroXY[0]+4);
                    printf("O");
                    turno=1;
                }
            }
            //Verifica se acabou
            ok=0;
            for(x=0;x<t;x++){
                for(y=0;y<t;y++){
                    if(tabJog[x][y]==1){
                        ok++;
                    }
                }
            }
            if(ok==0){
                turno=2;
                ganhador=0;
                fim=1;
            }
            gotoxy(4,21);
        }



    }while(fim==0);

    limpaPrompt1();

    return ganhador;
}

///Tiro Jogador
void tiroJogador(int tiroXY[2], int t){
    int ok=0;

    gotoxy(4,21);
    printf("\nDigite em que coordenadas deseja atirar:");
    while(ok==0){
        printf("\nLinha: ");
        scanf("%d%*c", &tiroXY[0]);
        if(tiroXY[0]>0 && tiroXY[0]<=t){
            ok=1;
        } else {
            printf("Coordenada invalida. Tente novamente.\n");
        }
    }
    ok=0;
    while(ok==0){
        printf("Coluna: ");
        scanf("%d%*c", &tiroXY[1]);
        if(tiroXY[1]>0 && tiroXY[1]<=t){
            ok=1;
        } else {
            printf("Coordenada invalida. Tente novamente.\n");
        }
    }
    limpaPrompt1();

}

///Tiro CPU
void tiroCPU(int tiroXY[2], int t){
    int ok=0;

    gotoxy(4,21);
    while(ok==0){
        tiroXY[0] = rand() % t+1;
        if(tiroXY[0]>0 && tiroXY[0]<=t){
            ok=1;
        }
    }
    ok=0;
    while(ok==0){
        tiroXY[1] = rand() % t+1;
        if(tiroXY[1]>0 && tiroXY[1]<=t){
            ok=1;
        }
    }
    limpaPrompt1();

}

///Organiza Ranking
void organizaRanking(){
    int x, y; //Anda pelos arrays
    int n=0, aux;
    char url[] = "ranking.txt", nome[50][20], auxNome[20];
    int vJogadas[50];

    FILE* ranking;
    ranking = fopen(url, "r");

    if(ranking==NULL){
        printf("Erro na abertura do arquivo.");
        system("pause");
        exit(1);
    }

    while((fscanf(ranking, "%s %d", nome[n], &vJogadas[n])) != EOF ){
        n++;
    }

    fclose(ranking);

    for(x=0;x<n;x++){
        for(y=x+1;y<n;y++){
            if(vJogadas[x]>vJogadas[y]){
                aux = vJogadas[x];
                vJogadas[x] = vJogadas[y];
                vJogadas[y] = aux;

                strcpy(auxNome, nome[x]);
                strcpy(nome[x], nome[y]);
                strcpy(nome[y], auxNome);
            }
        }
    }

    ranking = fopen(url, "w");

    if(ranking==NULL){
        printf("Erro na abertura do arquivo.");
        system("pause");
        exit(1);
    }
    for(x=0;x<n;x++){
        fprintf(ranking, "%s %d\n", nome[x], vJogadas[x]);
    }
    fclose(ranking);
}

///Mostra Ranking
void mostraRanking(){
    int x; //Passa pelo array
    int m, n; //Contador
    int vJogadas[50];
    char url[] = "ranking.txt", nome[50][20];
    char opc;

    FILE* ranking;
    ranking = fopen(url, "r");

    while((fscanf(ranking, "%s %d", nome[n], &vJogadas[n]))!=EOF){
        n++;
    }

    m = n/2;
    fclose(ranking);
    system("cls");

    printf("\n||===========================================================||");
    printf("\n||    Posicao     |          Nome          |     Jogadas     ||");
    printf("\n||----------------+------------------------+-----------------||");

    for(x=m;x<n;x++){
        printf("\n||      %2d�       |       %10s       |    %5d        ||", x+1-m, nome[x], vJogadas[x]);
    }

    printf("\n||===========================================================||");

    printf("\nPressione qualquer tecla para retornar.");
    getch();

    menuPrincipal();
}

///Sair
sair(){
    char certeza1;
    char certeza2;
    int ok=0;
    while(ok==0){
    printf("Deseja realmente sair? [s/n]: ");
        scanf("%c%*c",&certeza1);
    certeza2 = tolower(certeza1);
        if(certeza2=='s' || certeza2=='n'){
            ok=1;
        }
    }
    if(certeza2=='s'){
        return 1;
    } else {
        return 0;
    }
}
//---------------------------------------------


int main()
{

    srand(time(NULL));

    intro();
    menuPrincipal();

    return 0;
}
