#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<string.h>

#define LIN 10
#define COL 15
#define POS_X (COL/2)
#define POS_Y (LIN-2)

int main(){
    //Funções de mapeamento
    void setPosition(int x, int y);
    void initHeadQuad(char map[LIN][COL]);
    void printHeadQuad(char map[LIN][COL], COORD pos);
    COORD getStartCursor(void);

    //Pegando coordenada de início da execussão 
    COORD costart = getStartCursor();

    //Gerando e apresentando matriz
    char uma_pinha[LIN][COL];
    initHeadQuad(uma_pinha);
    printHeadQuad(uma_pinha, costart);

    //Inicializando posição do personagem
    int coord_x = POS_X + costart.X, coord_y = POS_Y + costart.Y;
    setPosition(coord_x, coord_y);
    printf("q");

    while(1){
        if(_kbhit()){
            char key = _getch();

            //Limpando ultima posição
            setPosition(coord_x, coord_y);
            printf(" ");
            
            //Condição para saida
            if(key == 'q')break;

            //Movimentação
            if (key == 'a' && coord_x > costart.X + 1) coord_x--; 
            if (key == 'w' && coord_y > costart.Y + 1) coord_y--; 
            if (key == 'd' && coord_x < (costart.X + COL - 2)) coord_x++;
            if (key == 's' && coord_y < (costart.Y + LIN - 2)) coord_y++;
            
            //Atualizando posição do personagem
            setPosition(coord_x, coord_y);
            printf("p");
        }
    }

    //Alterando o cursor para evitar problemas
    setPosition(0, (costart.Y + (LIN+1)));
}

void setPosition(int x, int y){
    COORD pos;
    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void initHeadQuad(char map[LIN][COL]){
    int i, j;
    for(i=0;i<LIN;i++){
        for(j=0;j<COL;j++){
            if(i>0 && i<(LIN - 1) && j>0 && j<(COL-1)){
                map[i][j] = ' ';
            }else{
                map[i][j] = '*';
            }
        }
    }
}

void printHeadQuad(char map[LIN][COL], COORD pos){
    int i, j;
    for(i=0;i<LIN;i++){
        setPosition(pos.X, pos.Y + i);
        for(j=0;j<COL;j++){
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

COORD getStartCursor(){
    CONSOLE_SCREEN_BUFFER_INFO cursor;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
    return cursor.dwCursorPosition;
}