#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<time.h>

#define LIN 10
#define COL 20

int main(){
    void ranposit(char **m);
    void fillM(char **m);
    void printM(char **m);

    int i;    
    char **matriz = malloc(LIN * sizeof(char *));
    for (i = 0; i < LIN; i++) {
        matriz[i] = malloc(COL * sizeof(char));
    }

    fillM(matriz);
    ranposit(matriz);
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

void printM(char **m){
    int i, j;
    for(i=0;i<LIN;i++){
        for(j=0;j<COL;j++){
            printf("%c", m[i][j]);
        }
        printf("\n");
    }
    
}

void ranposit(char **m){
    srand(time(NULL));
    while(1){
        int min = 1, max = COL - 2;
        int radom = min + rand()%(max - min + 1);

        if(_kbhit()){
            char key = _getch();
            if(key == 'q') break;
        }
        m[min][radom] = 'g';
        printM(m);
        
        Sleep(1200);
        system("cls");
        m[min][radom] = ' ';
    } 
}
