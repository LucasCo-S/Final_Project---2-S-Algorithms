#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>

//Definindo tamanho da matriz
#define LIN 10
#define COL 25
#define POS_X (COL/2)
#define POS_Y (LIN-2)

//Fun��es de Exibir Matriz
void fillM(char **m);
void printM(char **m, COORD pos);

//Fun��es de Mapeamento
void setPosition(int x, int y);
COORD getStartCursor(void);
void setChar(int x, int y, char key);
void cursorHidden();

//Sistema da Bomba
typedef struct{
	int x;
	int y;
}BCOORD;
DWORD WINAPI rposit(LPVOID lpParam);
volatile bool repeats = true; //Variavel de controle

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
    cursorHidden();

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
            if(key == 'a' || key == 'd'){
                setChar(coord_x, coord_y, key);
            }
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

void setChar(int x, int y, char key){
	HANDLE handCon = GetStdHandle(STD_OUTPUT_HANDLE);

    char c1[] = "p";
    char c2[] = "q";
    char *charac;
    if(key == 'd'){
        charac = c1;
    }else if(key == 'a'){
        charac = c2; 
    }else{
        return;
    }

    int strlenght = strlen(charac);
    CHAR_INFO cmdbuff[strlenght];
    int i;
    for(i=0;i<strlenght;i++){
        cmdbuff[i].Char.AsciiChar = charac[i];
        cmdbuff[i].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE;
    } 

    // Define o tamanho do buffer
    COORD bufferSize = {strlenght, 1}; //2 colunas, 1 linha

    // Ponto de início no buffer
    COORD bufferCoord = {0, 0};

    // Região no console onde será escrita
    SMALL_RECT writeRegion = {x, y, x + strlenght - 1, y};

    WriteConsoleOutput(handCon, cmdbuff, bufferSize, bufferCoord, &writeRegion);
}

void cursorHidden(){
    HANDLE handCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    // Obter informações do cursor
    GetConsoleCursorInfo(handCon, &cursorInfo);

    // Ocultar o cursor
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(handCon, &cursorInfo);
}


