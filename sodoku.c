#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>


#define true	1
#define false	0
typedef long init;

char cell[9][9] = {"         ","         ","         ","         ","         ","         ","         ","         ","         "};

int wait(int milli_seconds , init * initial);
long ms_time();

void key_press(char key);
int xpos = 0;
int ypos = 0;
int square_x = 0;
int square_y = 0;
char last_pressed;
int check(char n);

void instructionsf();

void new_frame();
char frame[28][56];

void highlight();
int row_to_hi;
int col_to_hi;
int sqr_to_hi[2];

void update(int get_new , ...);
int frameno = 0;
long start = 0;
long startms = 0;
char * available();
char avlbl [18];


int main(){
    //SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x08 );
    start = time(NULL);

    startms = ms_time(); 

    system("cls");

    printf("\e[?25l");

    instructionsf();

    while(true){

        update(1,NULL);

        if(_kbhit()){
            key_press(getch());
        }

    }
    return 0;
    
}


long ms_time(){
    SYSTEMTIME time;

    GetSystemTime(&time);

    return(time.wHour * 60 * 60 * 1000) + (time.wMinute * 60 * 1000) + (time.wSecond * 1000) + time.wMilliseconds;
}


int wait(int milli_seconds , init * initial){

    if(*initial == 0){
        *initial = ms_time();  
    }

    if(ms_time() - *initial >= milli_seconds){
        *initial = 0;
        return 1;
    }
    return 0;
}


void instructionsf(){
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD instructions = {70,3};

    SetConsoleCursorPosition(out,instructions);
    
    for(int i = 0 ; i < 42 ; i++){
        printf("_");
    }

    instructions.X --;
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("| wasd to move between cells               |");
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("|                                          |");
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("| (capital) WASD to move between tables    |");
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("|                                          |");
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("| press ENTER to enter insert mode         |");
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("| > press ESCAPE to get out of insert mode |");
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("| > press ENTER again to confirm           |");
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("|");
    for(int i = 0 ; i < 42 ; i++){
        printf("_");
    }
    printf("|");
}

init blinker;

int blink = 0;
int interval;

char cursor = '|';

void new_frame(){
    for(int j = 0 ; j < 28 ; j++){
        for(int i = 0 ; i < 55 ; i++){
            if((i+3)%6 == 0 && (j+1)%3 == 0 ){//19
                if((square_x + 3*square_y) == (3*(j/10) + i/18) &&(xpos + 3*ypos) == ((i+18)%18)/6 + 3*(((j+9)%9)/3)){
                    row_to_hi = j;
                    col_to_hi = i;
                    sqr_to_hi [0] = 18 * (i/18);
                    sqr_to_hi [1] = 9 * (j/9);

                    if(blink == 0){
                        cursor = '|';
                    }

                    if(blink == 1 && blinker == 0){
                        switch(cursor){
                            case ' ' : interval = 499;
                                       break;
                            case '|' : interval = 501;
                                       break;
                        }
                        wait(interval,&blinker);
                    }

                    if(blink == 1 && wait(interval,&blinker)){
                        switch(interval){
                            case 501 : cursor = ' ';
                                       break;
                            case 499 : cursor = '|';
                                       break;
                        }
                    }

                    frame[j][i - 1] = cursor;
                    frame[j][i + 1] = cursor;
                    frame[j][i] = cell[3*(j/10) + i/18][((i+18)%18)/6 + 3*(((j+9)%9)/3)];
                    i++;

                    continue;
                }

                frame[j][i] = cell[3*(j/10) + i/18][((i+18)%18)/6 + 3*(((j+9)%9)/3)];
                continue;
            }
            if(i > 0 && i < 54 && (i+18)%18 == 0 && j > 0){
                frame[j][i] = 'H';
                continue;
            }
            if((i+6)%6 == 0 && j > 0){
                frame[j][i] = '|';
                continue;
            }
            if(j > 0 && j < 27 && i > 0 && i <54 && (j+9)%9 == 0){
                frame[j][i] = '=';
                continue;
            }
            if(i > 0 && i < 54 && (j+3)%3 == 0){
                frame[j][i] = '_';
                continue;
            }
            frame[j][i] = ' ';
        }
        frame[j][55] = '\n';
    }
    frame[27][55] = 0;
}


void highlight(){
    char row[4][56];
    char col[27][8];
    char sqr[10][20];

    
    for(int i = 0 , in = row_to_hi - 2; i < 4 ; i++ , in++){
        for(int j = 0 ; j < 56 ; j++){
            row[i][j] = frame[in][j];
        }
    }

    row[3][55] = 0;

    for(int i = 0 , in = col_to_hi - 3; i < 7 ; i++ , in++){
        for(int j = 1 ; j < 28 ; j++){
            col[j-1][i] = frame[j][in];
            col[j-1][7] = 0;
        }
    }

    for(int i = sqr_to_hi[0] , ind = 0 ; ind < 20 ; i++ , ind++ ){
        for(int  j = sqr_to_hi[1] , jnd = 0 ; jnd < 10 ; j++ , jnd++){
            sqr[jnd][ind] = frame[j][i];
            sqr[jnd][19] = 0;
        }
    }

    COORD row_coord = {0,row_to_hi - 1};
    COORD col_coord = {col_to_hi - 3,2};
    COORD sqr_coord = {sqr_to_hi[0],sqr_to_hi[1] + 1};

    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0xf0 );

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),row_coord);
    printf("%s",row[0]);

    for(int i = 0 ; i < 27 ; i++){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),col_coord);
        printf("%s",col[i]);
        col_coord.Y ++;
    }

    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x30 );
    for(int i = 0 ; i < 10 ; i++){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),sqr_coord);
        printf("%s",sqr[i]);
        sqr_coord.Y ++;
    }

    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x07 );
}

COORD msg  = {91,15};
int length = 0;

#define mesagetime 1200
init message;

void update( int get_new , ... ){
    frameno ++;

    COORD fcount = {0,0};
    COORD frames = {0,1};
    COORD avl    = {81,18};
    COORD av     = {82,20};

    char * avlble = available(square_x + 3*square_y);

    int dif = 17;

    if(get_new == 1){
        new_frame();
    }
    
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),fcount);
    printf("%i , %i , %i , %i       ", row_to_hi , col_to_hi , sqr_to_hi[0] , sqr_to_hi[1]);

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),frames);
    printf("%s",frame[0]);
    highlight();

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),avl);
    printf("avalaible numbers :");
    
    for(int i = 0 ; avlble[i] != 0 ; i++){
        dif--;
    }

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),av);
    for(int i = 0 ; i < dif/2 ; i++){
        printf(" ");
    }

    printf("%s",avlble);

    for(int i = 0 ; i < dif/2 ; i++){
        printf(" ");
    }

    va_list args;
    va_start(args , get_new);

    char* format = va_arg(args,char*);
    msg.X = 91 - length/2;//do not change to -=.
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),msg);
    int erase = 0;
    for(int i = 0 ; message != 0 || i < length ; i++){
        if(message != 0 && wait(mesagetime , &message) == 0){
            break;
        }
        printf(" ");
        erase = 1;
    }
    if(erase){
        length = 0;
    }
    if(format != NULL){
        length += (length + 2) % 2 == 1;
        msg.X = 91 - length/2;//do not change to -=.
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), msg);
        for(int i = 0 ; i < length ; i++){
            printf(" ");
        }
        length = 0;
        for(int i = 0 ; format != NULL && format[i] != 0 ; i++){
            length ++;
        }
        msg.X = 91 - length/2;//do not change to -=.
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), msg);
        vprintf(format,args);
        wait(mesagetime , &message);
    }
    va_end(args);
    
    Sleep(20);
}

void get_input();
long last_move = 0;

void key_press(char key){
    int ydis = 0;
    int xdis = 0;
    int dis_y = 0;
    int dis_x = 0;
    switch(key){
        case 'w' : if(ypos == 0){
                       ydis = 2;
                       break;
                   }
                   ydis--;
                   break;
        case 'a' : if(xpos == 0){
                       xdis = 2;
                       break;
                   }
                   xdis--;
                   break;
        case 's' : if(ypos == 2){
                       ydis = -2;
                       break;
                   }
                   ydis++;
                   break;
        case 'd' : if(xpos == 2){
                       xdis = -2;
                       break;
                   }
                   xdis++;
                   break;
        case 'W' : if(square_y == 0){
                       dis_y = 2;
                       break;
                   }
                   dis_y--;
                   break;
        case 'A' : if(square_x == 0){
                       dis_x = 2;
                       break;
                   }
                   dis_x--;
                   break;
        case 'S' : if(square_y == 2){
                       dis_y = -2;
                       break;
                   }
                   dis_y++;
                   break;
        case 'D' : if(square_x == 2){
                       dis_x = -2;
                       break;
                   }
                   dis_x++;
                   break;
        case 13  :  blink = 1;
                    get_input();
                    blink = 0;
                    return;
    } 
    if(ms_time() - last_move >= 100 || last_move == 0 || last_pressed != key){
        ypos += ydis;
        xpos += xdis;
        square_x += dis_x;
        square_y += dis_y;
        last_move = ms_time();
        last_pressed = key;
    }
    cursor = '|';
    blinker = 0;
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}


void get_input(){
    char last_val = cell[square_x + 3*square_y][xpos + 3*ypos];
    update(1,NULL);
    char input;
    do{
        update(1,NULL);
        while(!kbhit()){
            update(1,NULL);
        }
        input = getch();
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        if(input == 27){
            cell[square_x + 3*square_y][xpos + 3*ypos] = last_val;
            blink = 0;
            return;
        }
        if(input == 13){
            blink = 0;
            return;
        }
        if(input == 'w' || input == 'a'|| input == 's' || input == 'd' || input == 'W' || input == 'A' || input == 'S' || input == 'D'){
            cell[square_x + 3*square_y][xpos + 3*ypos] = last_val;
            cursor = '|';
            blinker = 0;
            key_press(input);
            last_val = cell[square_x + 3*square_y][xpos + 3*ypos];
            continue;
        }
        if(input > '9' || input < '1'){
            update(0,"enter a number from 1 to 9");
            continue;
        }
        switch(check(input)){
            case 1 : update(0,"%c is in this square already",input);
                     continue;
            case 2 : update(0,"%c is in this row already",input);
                     continue;
            case 3 : update(0,"%c is in this column already",input);
                     continue;
        }
        cell[square_x + 3*square_y][xpos + 3*ypos] = input;
    }while (input != 27);
    return;
}


char * available(){
    char avl[9];
    for(int i = 1 ; i <= 18 ; i ++){
        avlbl[i-1] = 0;
        avl[(i/2)-1] = (i/2) + 48;
    }

    for(int i = 0 ; i <= 9 ; i++){
        for(char ch = '1' ; ch <= '9' ; ch++){
            if(check(ch)){
                for(int in = 0 ; in < 9 ; in++){
                    if(avl[in] == ch){
                        avl[in] = 0;
                    }
                }
            }
        }
    }

    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            if(avl[j] < avl[j+1]){
                char tmp = avl[j];
                avl[j] = avl[j+1];
                avl[j+1] = tmp;
            }
        }
    }

    for(int i = 0 ; i < 18 ; i+=2){
        avlbl[i] = avl[i/2];
        if(i > 0 && avl[i/2] > 0){
            avlbl[i-1] = ',';
        }
    }

    return avlbl;
}

int check(char ch){
    
    int square = square_x + 3*square_y;
    int pos = xpos + 3*ypos;

    for(int i = 0 ; i < 9 ; i++){
        if(cell[square][i] == ch){
            return 1;
        }
    }

    square = (square / 3) * 3;
    pos = (pos / 3) * 3;

    for(int i = square ; i < square + 3 ; i++){
        for(int j = pos ; j < pos + 3 ; j++){
            if(cell[i][j] == ch){
                return 2;
            }
        }
    }

    square = (square_x + 3*square_y) % 3;
    pos = (xpos + 3*ypos) % 3;

    for(int i = square ; i < 9 ; i += 3){
        for(int j = pos ; j < 9 ; j += 3){
            if(cell[i][j] == ch){
                return 3;
            }
        }
    }
    return 0;
}