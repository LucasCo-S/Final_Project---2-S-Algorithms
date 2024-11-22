#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>

//Definindo número de tamanho da matriz
#define LIN 10
#define COL 25
#define POS_X (COL/2)
#define POS_Y (LIN-2)

//Funções de Exibir Matriz
void fillM(char **m);
void printM(char **m, COORD pos);

//Funções de Mapeamento
void setPosition(int x, int y);
COORD getStartCursor(void);

//Sistema da Bomba
typedef struct{
	int x;
	int y;
}BCOORD;
DWORD WINAPI rposit(LPVOID lpParam);
volatile repeats = true; //Variavel de controle

int main(){
    //Alocação dinâmica da matriz
    int i;    
    char **matriz = malloc(LIN * sizeof(char *));
    for (i = 0; i < LIN; i++) {
        matriz[i] = malloc(COL * sizeof(char));
    }
    
    //Pegando coordenada de início da execussão 
    COORD costart = getStartCursor();

    //Posicionando area de atua��o de gera��o de bombas
    BCOORD bombco;
	bombco.x = costart.X + COL-2;
	bombco.y = costart.Y + 1;

    //Preenchendo e apresentando matriz
    fillM(matriz);
    printM(matriz, costart);

    //Inicializando posição do personagem
    int coord_x = POS_X + costart.X, coord_y = POS_Y + costart.Y;
    setPosition(coord_x, coord_y);
    printf("q");
    
    while(repeats){
        if(_kbhit()){
            char key = _getch();

            //Limpando ultima posição
            setPosition(coord_x, coord_y);
            printf(" ");
            
            //Condição para saida
            if(key == 'q') repeats = false;

            //Movimentação
            if (key == 'a' && coord_x > costart.X + 1) coord_x--; 
            if (key == 'd' && coord_x < (costart.X + COL - 2)) coord_x++;
            
            //Atualizando posição do personagem
            setPosition(coord_x, coord_y);
            printf("p");
        }
	}
}

void fillM(char **m){
    int i, j;
    for(i=0;i<LIN;i++){
        for(j=0;j<COL;j++){
            if(i>0 && i<(LIN - 1) && j>0 && j<(COL - 1)){
                m[i][j] = ' ';
            }else{
                m[i][j] = '*';
            }
        }
    }
}

void printM(char **m, COORD pos){
    int i, j;
    for(i=0;i<LIN;i++){
    	setPosition(pos.X, pos.Y + i);
        for(j=0;j<COL;j++){
            printf("%c", m[i][j]);
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
    COORD pos;
    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

DWORD WINAPI rposit(LPVOID lpParam){
	while(repeats){
		
	}
}
