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
#define MAX_BOMB 5
#define RESET_COLOR "\x1B[0m"
#define WHITE_BK "\x1B[47m"

//Fun��es de Exibir Matriz
void printM(COORD pos);

//Sistema de Pontua��o
DWORD WINAPI scoreHeadQuad(LPVOID lpParam);
int scorePoints = 0;

//Fun��es de Mapeamento
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

volatile bool repeats = true; //Vari�vel de controle
volatile BCOORD verif[MAX_BOMB]; //Vari�vel de verifica��o
volatile int numB = 1;

//Mutex para sincronizar o acesso ao console
HANDLE consoleMutex;

int main(){
	// Muda a codifica��o para UTF-8
    SetConsoleOutputCP(CP_UTF8);
    
    //Definindo numeros aleatorios
    srand(time(NULL));

    //Pegando coordenada de in�cio da execu��o
    COORD costart = getStartCursor();

    //Definição do quadro de pontos
    int spaceBetween = 5;
    COORD sc; 
    sc.X = costart.X + COL + spaceBetween;
    sc.Y = costart.Y;

    //Posicionando �rea de atua��o de gera��o de bombas
    BCOORD bombco;
    bombco.x = costart.X + COL - 2;
    bombco.y = costart.Y + 1;

    //apresentando display de mapa
    printM(costart);

    //Inicializando posi��o do personagem
    int coord_x = POS_X + costart.X, coord_y = POS_Y + costart.Y;
    setPosition(coord_x, coord_y);
    printf("\U0001F525");
    cursorHidden();

    //Criando mutex
    consoleMutex = CreateMutex(NULL, FALSE, NULL);

    //Criando thread para geração de Bombas
    HANDLE hrPosit;
    DWORD IdrPosit;

    hrPosit = CreateThread(NULL, 0, rPosit, &bombco, 0, &IdrPosit);


    while(repeats){
        //Verifica colis�o mesmo se o jogador estiver parado
        int i;
    	for(i = 0; i < numB; i++){
        	if(verif[i].x == coord_x && verif[i].y == coord_y || verif[i].x == coord_x+1 && verif[i].y == coord_y){
            	repeats = false;
            	break;
        	}
    	}
    	if(!repeats) break; 

        if(_kbhit()){
            char key = _getch();

            //Ignora teclas n�o tratadas
            if(key != 'a' && key != 'd' && key != 'q') continue;

            //Limpando �ltima posi��o
            setChar(coord_x, coord_y, ' ');

            //Condi��o para sa�da
            if (key == 'q') repeats = false;

            //Movimenta��o horizontal
            if(key == 'a' && coord_x > costart.X + 1) coord_x--;
            if(key == 'd' && coord_x < (costart.X + COL - 2)) coord_x++;

            //Atualiza posi��o do personagem
            if(key == 'a' || key == 'd'){
                setChar(coord_x, coord_y, key);
            }

			for(i = 0; i < numB; i++){
            	if(verif[i].x == coord_x && verif[i].y == coord_y) {
                	repeats = false;
                	break;
            	}
        	}

            Sleep(50);
        }
    }
    

    WaitForSingleObject(hrPosit, INFINITE);
    CloseHandle(hrPosit);
    CloseHandle(consoleMutex); // Libera o Mutex
    
    printf("\n\nO jogo acabou, va olhar a luz do sol.\n");
    printf("Pontos: | %d |\n", scorePoints);

    while(1){
    	if(_kbhit()){
    		char letra = _getch();
    		if(letra == 'o') break;
		}
	}
    
}

void printM(COORD pos){
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
    
    if(c == 'a' || c == 'd'){
        printf("\U0001F525");
    }else{
        printf(" ");
    }
    if(c == 'o'){
        printf("\x1B[36mo\x1B[0m");
    }else if(c == ' '){
        printf("%c", c); 
    }
    
    ReleaseMutex(consoleMutex); // Libera o Mutex
}

DWORD WINAPI rPosit(LPVOID lpParam) {
    BCOORD coorde = *(BCOORD*)lpParam;
    
    while(repeats){
        int min = coorde.y, max = coorde.x;
        int random[numB];
		int i, j;
        if(scorePoints%20 == 0 && numB < MAX_BOMB) numB++;
        
        for(i = 0; i < numB; i++){
            random[i] = min + rand() % ((max-2) - min + 1);
        }

        for(i = 0; i < LIN - 2; i++){ 
            for(j = 0; j < numB; j++){
                if(i > 0) setChar(random[j], coorde.y + i - 1, ' ');
            }

            for(j = 0; j < numB; j++){
                setChar(random[j], coorde.y + i, 'o');
                verif[j].x = random[j];
                verif[j].y = coorde.y + i;
            }

            Sleep(40);

            if(i == LIN - 3){
                for (j = 0; j < numB; j++){
                    setChar(random[j], coorde.y + i, ' '); 
                }
            }
        }
        scorePoints += (5 * numB);
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
