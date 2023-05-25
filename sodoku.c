#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <windows.h>


#define true	1
#define false	0
typedef long init;

char cell[9][10] = {"         ","         ","         ","         ","         ","         ","         ","         ","         "};
char solved[9][10] = {"         ","         ","         ","         ","         ","         ","         ","         ","         "};

int wait(int milli_seconds , init * initial);
long ms_time();

void key_press(char key);
int xpos = 0;
int ypos = 0;
int square_x = 0;
int square_y = 0;
char last_pressed;
int check(char n);

void choose_difficulty();
#define easy 6
#define med 4
#define hard 2
int difficulty = 9;

void generate_new_level();
#define ABC	1
#define BCA	2
#define CAB	3
#define ACB	4
#define BAC	5
#define CBA	6
int blocked[9][9];
int combinations[2][3] = {{ABC,BCA,CAB},{ACB,BAC,CBA}}; 

int check_win();

void winning_screen();

void instructionsf();

void new_frame();
char frame[28][56];
char last_frame[28][56];

void highlight();
int row_to_hi;
int col_to_hi;
int sqr_to_hi[2];

void highlight_selected();
int cells_to_hi_coord[9][2];

void update(int get_new , char * format , ... );
int frameno = 0;
char * available();
char avlbl [18];

init instruction;


int main(){
    new_game :

    system("cls");

    printf("\e[?25l");

    srand(ms_time());

    choose_difficulty();

    system("cls");

    instructionsf();
    wait(20000,&instruction);

    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 2 ; j++){
            cells_to_hi_coord[i][j] = -1;
        }
        for(int j = 0 ; j < 9 ; j++){
            cell[i][j] = ' ';
        }
    }

    generate_new_level();

    while(check_win() == 0){

        update(1,NULL);

        if(_kbhit()){
            key_press(getch());
        }

    }

    winning_screen();

    system("cls");
    
    COORD ask = {0,0};
    COORD cur = {0,2};
    COORD cur_main = {0,2};

    char answer = 0;

    answer = 0;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x70);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),ask);

    printf("do you want to play a new game\n\n");

    ask.Y = 2;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x70);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cur);
    printf("> yes  \n no    \n\n");

    printf(" use w and s to choose ");

    for(char i = getch() ; true ; i = getch()){
        switch(i){
            case 's' :  if(cur.Y == 3){
                            cur.Y = 2;
                            break;
                        }
                        cur.Y ++;
                        break;
            case 'w' :  if(cur.Y == 2){
                            cur.Y = 3;
                            break;
                        }
                        cur.Y --;
                        break;
        }
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cur_main);
        printf(" yes   \n no    \n\n");
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cur);
        switch(cur.Y){
            case 2 : printf("> yes");
                     answer = 'y';
                     break;
            case 3 : printf("> no");
                     answer = 'n';
                     break;
        }

        if(i == 13){
            break;
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07); 

    if(answer == 'y'){
        goto new_game;
    }
    if(answer == 'n'){
        return 0;
    }
}


void choose_difficulty(){
    COORD cur_main = {0,2}; 
    COORD cur = {0,2};
    COORD ins = {0,6};

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x70);
    
    printf("choose a difficulty\n\n");
    printf("> easy    \n medium   \n hard     ");

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),ins);
    printf(" use w and s to choose ");

    for(char i = getch() ; true ; i = getch()){
        switch(i){
            case 's' :  if(cur.Y == 4){
                            cur.Y = 2;
                            break;
                        }
                        cur.Y ++;
                        break;
            case 'w' :  if(cur.Y == 2){
                            cur.Y = 4;
                            break;
                        }
                        cur.Y --;
                        break;
        }
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cur_main);
        printf(" easy \n medium \n hard ");
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cur);
        switch(cur.Y){
            case 2 : printf("> easy");
                     difficulty = easy;
                     break;
            case 3 : printf("> medium");
                     difficulty = med;
                     break;
            case 4 : printf("> hard");
                     difficulty = hard;
                     break;
        }

        if(i == 13){
            break;
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07); 
}


long ms_time(){
    SYSTEMTIME time;

    GetSystemTime(&time);

    return ((time.wYear - 1970) * 12 * 30 * 24 * 60 * 60 * 1000) + (time.wMonth * 30 * 24 * 60 * 60 * 1000) + (time.wDay * 24 * 60 * 60 * 1000) + (time.wHour * 60 * 60 * 1000) + (time.wMinute * 60 * 1000) + (time.wSecond * 1000) + time.wMilliseconds;
}


void swap_char(char * a , char * b){
    char tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}


void swap_int(int * a , int * b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}


void sort_string(char string[10]){
    for(int i = 0 ; string[i + 1] != 0 ; i++){
        if(string [i] == '0'){
            swap_char(&string [i] , &string [i+1]);
        }
    }
}

void sort(int * array , int arr_size){
    for(int i = 0 ; i < arr_size - 1 ; i++){
        for(int j = 0 ; j < arr_size - 1 ; j++){
            if(array[j] > array[j + 1]){
                swap_int(&array [i] , &array [i+1]);
            }
        }
    }
    for(int i = 0 ; i < arr_size - 1; i++){
        if(array [i] == 0){
            swap_int(&array [i] , &array [i+1]);
        }
    }
}


void generate_new_level(){
    char numbers[] = "123456789";
    int size = 9;
    char random [9];
    for(int  i = 0 ; i < 9 ; i++){
        int r = rand()%size;
        random [i] = numbers[r];
        numbers[r] = '0';
        sort_string(numbers);
        size--;
    }
    for (int i = 0 ; i < 9 ; i++){
        for (int j = 0 ; j < 9 ; j++){
            solved[i][j] = random [j];
        }
    }

    for(int i = 0 ; i < 3 ; i++){
        size = 3;
        int r = rand()%2;
        for(int  j = 0 ; j < 3 ; j++){
            int rn = rand()%size;
            switch(combinations[r][rn]){
                case ABC : combinations[r][rn] = 0;
                           sort(combinations[r],3);
                           size --;
                           break;
                case BCA : combinations[r][rn] = 0;
                           sort(combinations[r],3);
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[j + 3*i][k],&solved[j + 3*i][k+3]);
                           }
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[j + 3*i][k + 3],&solved[j + 3*i][k+6]);
                           }
                           size --;
                           break;
                case CAB : combinations[r][rn] = 0;
                           sort(combinations[r],3);
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[j + 3*i][k + 6],&solved[j + 3*i][k]);
                           }
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[j + 3*i][k + 3],&solved[j + 3*i][k + 6]);
                           }
                           size --;
                           break;
                case ACB : combinations[r][rn] = 0;
                           sort(combinations[r],3);
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[j + 3*i][k + 6],&solved[j + 3*i][k + 3]);
                           }
                           size --;
                           break;
                case BAC : combinations[r][rn] = 0;
                           sort(combinations[r],3);
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[j + 3*i][k],&solved[j + 3*i][k + 3]);
                           }
                           size --;
                           break;
                case CBA : combinations[r][rn] = 0;
                           sort(combinations[r],3);
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[j + 3*i][k],&solved[j + 3*i][k + 6]);
                           }
                           size --;
                           break;
                           
            }
        }
        for(int i = 0 ; i < 2 ; i++){
            for(int j = 1 ; j <= 3 ; j++){
                combinations[i][j - 1] = j + 3*i;
            }
        }
        size = 3;
        for(int  j = 0 ; j < 3 ; j++){
            int rn = rand()%size;
            switch(combinations[r][rn]){
                case ABC : combinations[r][rn] = 0;
                           sort(combinations[r],3);
                           size --;
                           break;
                case BCA : combinations[r][rn] = 0;
                           sort(combinations[r],3);
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[i + 3*j][3*k],&solved[i + 3*j][3*k+1]);
                           }
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[i + 3*j][3*k + 1],&solved[i + 3*j][3*k+2]);
                           }
                           size --;
                           break;
                case CAB : combinations[r][rn] = 0;
                           sort(combinations[r],3);
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[i + 3*j][3*k + 1],&solved[i + 3*j][3*k + 2]);
                           }
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[i + 3*j][3*k],&solved[i + 3*j][3*k + 1]);
                           }
                           size --;
                           break;
                case ACB : combinations[r][rn] = 0;
                           sort(combinations[r],3);
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[i + 3*j][3*k + 1],&solved[i + 3*j][3*k + 2]);
                           }
                           size --;
                           break;
                case BAC : combinations[r][rn] = 0;
                           sort(combinations[r],3);
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[i + 3*j][3*k],&solved[i + 3*j][3*k + 1]);
                           }
                           size --;
                           break;
                case CBA : combinations[r][rn] = 0;
                           sort(combinations[r],3);
                           for(int k = 0 ; k < 3 ; k++){
                               swap_char(&solved[i + 3*j][3*k],&solved[i + 3*j][3*k + 2]);
                           }
                           size --;
                           break;
                           
            }
        }
        for(int i = 0 ; i < 2 ; i++){
            for(int j = 1 ; j <= 3 ; j++){
                combinations[i][j - 1] = j + 3*i;
            }
        }
    }
    for(int i = 0 ; i < 9 ; i++){
        int cell_no[] = {1,2,3,4,5,6,7,8,9};
        size = 9;
        for(int j = 0 ; j < difficulty ; j++){
            int r = rand() % size;
            cell[i][cell_no[r]-1] = solved[i][cell_no[r]-1];
            cell_no[r] = 0;
            sort(cell_no,9);
            size --;
        }
    }
    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            if(cell[i][j] != ' '){
                blocked[i][j] = 1;
            }
            else{
                blocked[i][j] = 0;
            }
        }
    }
}


int check_win(){
    for(int i = 0 ; i < 9 ; i ++){
        for(int j = 0 ; j < 9 ; j++){
            if(cell[i][j] == ' '){
                return 0;
            }
        }
    }

    return 1;
}


void winning_screen(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x70);

    COORD win = {52,13};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),win);
    printf("           ");
    win.Y ++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),win);
    printf("  you won  ");
    win.Y ++;
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),win);
    printf("           ");

    for(int i = 0 ; i < 3000 && kbhit() == 0 ; i++ ){
        Sleep(1);
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);
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

    COORD instructions = {69,3};

    SetConsoleCursorPosition(out,instructions);

    printf("%c",201);
    for(int i = 0 ; i < 42 ; i++){
        printf("%c",205);
    }
    printf("%c",187);

    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("%c wasd to move between cells               %c",186,186);
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("%c                                          %c",186,186);
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("%c (capital) WASD to move between tables    %c",186,186);
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("%c                                          %c",186,186);
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("%c press ENTER to enter insert mode         %c",186,186);
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("%c > press ESCAPE to get out of insert mode %c",186,186);
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("%c > press ENTER again to confirm           %c",186,186);
    instructions.Y ++;

    SetConsoleCursorPosition(out,instructions);
    printf("%c",200);
    for(int i = 0 ; i < 42 ; i++){
        printf("%c",205);
    }
    printf("%c",188);
}

init blinker;

int blink = 0;
int interval;

char cursor = '|';

void new_frame(){
    for(int i = 0 ; i < 28 ; i++){
        for(int j = 0 ; j < 56 ; j++){
            last_frame[i][j] = frame[i][j];
        }
    }

    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 2 ; j++){
            cells_to_hi_coord[i][j] = -1;
        }
    }
    
    int index = 0;

    for(int j = 0 ; j < 28 ; j++){
        for(int i = 0 ; i < 55 ; i++){
            if((i+3)%6 == 0 && (j+1)%3 == 0 ){
                if((square_x + 3*square_y) == (3*(j/10) + i/18) && (xpos + 3*ypos) == ((i+18)%18)/6 + 3*(((j+9)%9)/3)){
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

                if(cell[(3*(j/10) + i/18)][((i+18)%18)/6 + 3*(((j+9)%9)/3)] == cell[square_x + 3*square_y][xpos + 3*ypos] && cell[(3*(j/10) + i/18)][((i+18)%18)/6 + 3*(((j+9)%9)/3)] >= '0' && cell[(3*(j/10) + i/18)][((i+18)%18)/6 + 3*(((j+9)%9)/3)] <= '9'){
                    cells_to_hi_coord[index][0] = i;
                    cells_to_hi_coord[index][1] = j;
                    index++;
                }

                frame[j][i] = cell[3*(j/10) + i/18][((i+18)%18)/6 + 3*(((j+9)%9)/3)];
                continue;
            }
            if(i == 54 && j == 0){
                frame[j][i] = 187;//'╗'
                continue;
            }
            if(i == 54 && j == 27){
                frame[j][i] = 188;//'╝'
                continue;
            }
            if(i == 0 && j == 0){
                frame[j][i] = 201;//'╔'
                continue;
            }
            if(i == 0 && j == 27){
                frame[j][i] = 200;//'╚'
                continue;
            }
            if((i+18)%18 == 0 && j == 0 && i > 0 && i <54 && (j+9)%9 == 0){
                frame[j][i] = 203;//'╦'
                continue;
            }
            if((i+18)%18 == 0 && j == 27 && i > 0 && i <54 && (j+9)%9 == 0){
                frame[j][i] = 202;//'╩'
                continue;
            }
            if((i+18)%18 == 0 && j > 0 && j < 27 && i == 54 && (j+9)%9 == 0){
                frame[j][i] = 185;//'╣'
                continue;
            }
            if((i+18)%18 == 0 && j > 0 && j < 27 && i == 0 && (j+9)%9 == 0){
                frame[j][i] = 204;//'╠'
                continue;
            }
            if((i+18)%18 == 0 && j > 0 && j < 27 && i > 0 && i <54 && (j+9)%9 == 0){
                frame[j][i] = 206;//'╬'
                continue;
            }
            if((i+18)%18 == 0 && j > 0 && j < 27){
                frame[j][i] = 186;//'║'
                continue;
            }
            if(i > 0 && i <54 && (j+9)%9 == 0){
                frame[j][i] = 205;//'═'
                continue;
            }
            if((i+6)%6 == 0 && j > 0 && j < 27 && i > 0 && i < 54 && (j+3)%3 == 0){
                frame[j][i] = 197;//'┼'
                continue;
            }
            if((i+6)%6 == 0 && j > 0 && j < 27){
                frame[j][i] = 179;//'│'
                continue;
            }
            if(i > 0 && i < 54 && (j+3)%3 == 0){
                frame[j][i] = 196;//'─'
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


void highlight_selected(){
    char cells_to_hi[9][4][8];

    for(int i = 0 ; i < 9 ; i++){
        for(int j = 0 ; j < 4 ; j++){
            cells_to_hi[i][j][7] = 0;
        }
    }

    FILE * debug = fopen("debug.txt","w");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x80);

    for(int k = 0 ; k < 9 ; k++){
        if(cells_to_hi_coord[k][0] == -1){
            break;
        }
        int start_x = cells_to_hi_coord[k][0] - 3;
        int start_y = cells_to_hi_coord[k][1] - 2;
        for(int i = start_y , first_index = 0 ; first_index < 4 ; i++ , first_index ++){
            for(int j = start_x , second_index = 0; second_index < 7 ; j++ , second_index ++){
                cells_to_hi[k][first_index][second_index] = frame[i][j];
            }
        }
        for(int i = 0 ; i < 4 ; i ++){
            fprintf(debug , "%s\n" , cells_to_hi[k][i]);
        }

        COORD hilight = {start_x,start_y + 1};
        for(int i = 0 ; i < 4 ; i++){
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),hilight);
            printf("%s",cells_to_hi[k][i]);
            hilight.Y ++ ;
        }
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);

    fclose(debug);
}


COORD msg  = {91,15};
int length = 0;

#define mesagetime 1200
init message;

void update( int get_new , char * format , ... ){
    frameno ++;

    COORD frames       = {0,1};
    COORD avl          = {81,18};
    COORD av           = {82,20};
    COORD instructions = {69,3};

    char * avlble = available(square_x + 3*square_y);

    int dif = 17;
    int draw_new = 0;

    if(wait(20000,&instruction)){
        for(int i = 0 ; i < 9 ; i++){
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),instructions);
            for(int j = 0 ; j < 48 ; j++){
                printf(" ");
            }
            instructions.Y++;  
        }
    }

    if(get_new == 1){
        new_frame();
    }

    for(int  i = 0 ; i < 28 ; i++){
        for(int j = 0 ; j < 56 ; j++){
            if(frame[i][j] != last_frame[i][j]){
                draw_new = 1;
                break;
            }
        }
        if(draw_new == 1){
            break;
        }
    }

    if(draw_new == 1){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),frames);
        printf("%s",frame[0]);
        highlight();
        highlight_selected();
    }

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
    va_start(args , format);

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
        case 'W' : if(blink == 1){
                        update(0,"you can't move between tables in insert mode");
                        break;
                    }
                   if(square_y == 0){
                       dis_y = 2;
                       break;
                   }
                   dis_y--;
                   break;
        case 'A' : if(blink == 1){
                        update(0,"you can't move between tables in insert mode");
                        break;
                   }
                   if(square_x == 0){
                       dis_x = 2;
                       break;
                   }
                   dis_x--;
                   break;
        case 'S' : if(blink == 1){
                        update(0,"you can't move between tables in insert mode");
                        break;
                   }
                   if(square_y == 2){
                       dis_y = -2;
                       break;
                   }
                   dis_y++;
                   break;
        case 'D' : if(blink == 1){
                        update(0,"you can't move between tables in insert mode");
                        break;
                   }
                   if(square_x == 2){
                       dis_x = -2;
                       break;
                   }
                   dis_x++;
                   break;
        case 13  :  if(blocked[square_x + 3*square_y][(xpos + xdis) + 3*(ypos + ydis)]){
                        update(0,"you can't change this cell");
                        break;
                    }
                    blink = 1;
                    get_input();
                    blink = 0;
                    return;
    } 
    cursor = '|';
    blinker = 0;
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    if(blink == 1 && blocked[square_x + 3*square_y][(xpos + xdis) + 3*(ypos + ydis)] == 1){
        update(0,"you can't change that cell");
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
            if(avl[j] == 0){
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
