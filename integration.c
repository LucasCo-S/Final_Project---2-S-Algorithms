#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<stdbool.h>

//Definindo tamanho da matriz
#define LIN 15
#define COL 50
#define POS_X (COL/2)
#define POS_Y (LIN-2)
#define NBOMB 3
#define RESET_COLOR "\x1B[0m"
#define WHITE_BK "\x1B[47m"

//Funções de Exibir Matriz
void fillM(char **m);
void printM(char **m, COORD pos);

//Sistema de Pontuação
void scoreHeadQuad();

//Funções de Mapeamento
void setPosition(int x, int y);
COORD getStartCursor(void);
void setChar(int x, int y, char c);
void cursorHidden();

//Sistema da Bomba
typedef struct {
    int x;
    int y;
} BCOORD;
DWORD WINAPI rPosit(LPVOID lpParam);

volatile bool repeats = true; //Variável de controle
volatile BCOORD verif[NBOMB]; //Variável de verificação

//Mutex para sincronizar o acesso ao console
HANDLE consoleMutex;

int main(){
	// Muda a codificação para UTF-8
    system("chcp 65001");
	
    //Alocação dinâmica da matriz
    int i;
    char **matriz = malloc(LIN * sizeof(char *));
    for(i = 0; i < LIN; i++){
        matriz[i] = malloc(COL * sizeof(char));
    }
    
    //Pegando coordenada de início da execução
    COORD costart = getStartCursor();

    //Posicionando área de atuação de geração de bombas
    BCOORD bombco;
    bombco.x = costart.X + COL - 2;
    bombco.y = costart.Y + 1;


    //Preenchendo e apresentando matriz
    fillM(matriz);
    printM(matriz, costart);

    //Inicializando posição do personagem
    int coord_x = POS_X + costart.X, coord_y = POS_Y + costart.Y;
    setPosition(coord_x, coord_y);
    printf("q");
    cursorHidden();

    //Criando mutex
    consoleMutex = CreateMutex(NULL, FALSE, NULL);

    //Criando thread
    HANDLE hrPosit;
    DWORD IdrPosit;

    hrPosit = CreateThread(NULL, 0, rPosit, &bombco, 0, &IdrPosit);

    while(repeats){
        //Verifica colisão mesmo se o jogador estiver parado
        int i;
    	for(i = 0; i < NBOMB; i++){
        	if(verif[i].x == coord_x && verif[i].y == coord_y){
            	repeats = false;
            	break;
        	}
    	}
    	if(!repeats) break; 

        if(_kbhit()){
            char key = _getch();

            //Ignora teclas não tratadas
            if(key != 'a' && key != 'd' && key != 'q') continue;

            //Limpando última posição
            setChar(coord_x, coord_y, ' ');

            //Condição para saída
            if (key == 'q') repeats = false;

            //Movimentação horizontal
            if(key == 'a' && coord_x > costart.X + 1) coord_x--;
            if(key == 'd' && coord_x < (costart.X + COL - 2)) coord_x++;

            //Atualiza posição do personagem
            if(key == 'a' || key == 'd') {
                setChar(coord_x, coord_y, key);
            }

			for(i = 0; i < NBOMB; i++){
            	if(verif[i].x == coord_x && verif[i].y == coord_y) {
                	repeats = false;
                	break;
            	}
        	}

            //Sleep(50);
        }
    }
    

    WaitForSingleObject(hrPosit, INFINITE);
    CloseHandle(hrPosit);
    CloseHandle(consoleMutex); // Libera o Mutex
    
    //aqui é pra ficar o contador de pontos
    
    
    
    printf("\n\nO jogo acabou, va olhar a luz do sol.\n");
    while(1){
    	if(_kbhit()){
    		char letra = _getch();
    		if(letra == 'o') break;
		}
	}
    
}

void fillM(char **m){
    int i, j;
    for(i = 0; i < LIN; i++){
        for(j = 0; j < COL; j++){
            if(i > 0 && i < (LIN - 1) && j > 0 && j < (COL - 1)){
                m[i][j] = ' ';
            } else {
                m[i][j] = '¦';
            }
        }
    }
}

void printM(char **m, COORD pos){
    int i, j;
    for(i = 0; i < LIN; i++){
        setPosition(pos.X, pos.Y + i);
        for(j = 0; j < COL; j++){
            if(i == 0 || i == LIN - 1 || j == 0 || j == COL - 1){
                printf(WHITE_BK "¦" RESET_COLOR);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

COORD getStartCursor(){
    CONSOLE_SCREEN_BUFFER_INFO cursor;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
    return cursor.dwCursorPosition;
}

void setPosition(int x, int y){
    WaitForSingleObject(consoleMutex, INFINITE); // Bloqueia o Mutex
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    ReleaseMutex(consoleMutex); // Libera o Mutex
}

void setChar(int x, int y, char c){
    WaitForSingleObject(consoleMutex, INFINITE); // Bloqueia o Mutex
    setPosition(x, y);
    
    if(c == 'a') c = 'q';
    if(c == 'd') c = 'p';
    
	printf("%c", c);
    
    ReleaseMutex(consoleMutex); // Libera o Mutex
}

DWORD WINAPI rPosit(LPVOID lpParam) {
    BCOORD coorde = *(BCOORD*)lpParam;
    srand(time(NULL));
    
    while(repeats){
        int min = coorde.y, max = coorde.x - 2;
        int random[NBOMB];
		random[0] = min + rand() % ((max-1) - min + 1);
        random[1] = min + rand() % ((max-2) - min + 1);
        random[2] = min + rand() % ((max-3) - min + 1);
        
		int i;
        for(i = 0; i < LIN - 2; i++) {
            if(i > 0) setChar(random[0], coorde.y + i - 1, ' ');
			if(i > 0) setChar(random[1], coorde.y + i - 1, ' ');
			if(i > 0) setChar(random[2], coorde.y + i - 1, ' ');
			
            setChar(random[0], coorde.y + i, 'g');
			setChar(random[1], coorde.y + i, 'g');
			setChar(random[2], coorde.y + i, 'g');
			
            verif[0].x = random[0];
            verif[0].y = coorde.y + i;
            verif[1].x = random[1];
            verif[1].y = coorde.y + i;
            verif[2].x = random[2];
            verif[2].y = coorde.y + i;

            Sleep(40);

            if(i == LIN - 3) setChar(random[0], coorde.y + i, ' ');
            if(i == LIN - 3) setChar(random[1], coorde.y + i, ' ');
            if(i == LIN - 3) setChar(random[2], coorde.y + i, ' ');
        }
    }
    return 0;
}

void cursorHidden(){
    HANDLE handCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(handCon, &cursorInfo);

    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(handCon, &cursorInfo);
}

