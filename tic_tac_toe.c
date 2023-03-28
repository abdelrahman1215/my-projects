#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdarg.h>

void wait(int seconds);
char get_char(char * string);
int get_int(char * string);
char square [3][3] = {"123","456","789"};
bool first_turn = true , end_game = false;
int mode = 0 , difficulty = 3;
char computer_char, player_char;
void draw_table();
void player_turn();
void computer_turn();
char check_winner();
void new_game();
void clear();
void decide_turn(char player , int turn);
void aware_choice();


int main(){
    srand(time(NULL));
    new_game();
}


void wait(int seconds){
    for(long start = time(NULL) , end = time(NULL) ; end - start != seconds ; end = time(NULL)){
    }
}
char get_char(char * string){
    printf("%s",string);
    char input;
    do{
        input = getchar();
    }while(input == '\n');
    return input;
}
int get_int(char * string){
    int input;
    printf("%s",string);
    scanf("%i",&input);
    return input;
}



void decide_turn(char player , int turn){
    turn ++;
    if(player == 'p' && (int)(turn / 2) * 2 != turn){
        player_turn();
    }
    if(player == 'p' && (int)(turn / 2) * 2 == turn){
        computer_turn();
    }
    if(player == 'c' && (int)(turn / 2) * 2 != turn){
        computer_turn();
    }
    if(player == 'c' && (int)(turn / 2) * 2 == turn){
        player_turn();
    }
}



void new_game(){
    while(mode < 1 || mode > 2){
        system("cls");
        mode = get_int("\nplayer vs computer (1)\n\nplayer vs player   (2)\n\n:> ");
    }
    system("cls");
    if(mode == 2){
        for(int turn = 4 ; turn <= 13 ; turn++){
            if(end_game == true){
                break;
            }
            switch(player_char){
                case 'O' : player_char = 'X';
                           break;
                case 'X' : player_char = 'O';
                           break;
            }
            system("cls");
            printf("\nplayer %i (%c) : \n",(turn % 2) + 1,player_char);
            player_turn();
            system("cls");
            if(check_winner() == player_char){
                draw_table();
                printf("\nplayer %i wins !\n",(turn % 2) + 1);
                end_game = true;
            }
            if(check_winner() == 'd'){
                printf("\ndraw!\n");
                end_game = true;
            }
        }
    }
    else{
        do{
            system("cls");
            difficulty = get_int("\nchoose a difficulty \n\n1 is the easiest  4 is the hardest \n\n:> ");
        }while(difficulty < 1 || difficulty > 4);
        difficulty = 5 - difficulty;
        system("cls");
        char first_play;
        if(check_winner() != computer_char){
            first_play = 'p';
        }
        else if(check_winner() == computer_char){
            first_play = 'c';
        }
        for(int turn = 0 ; turn < 9 ; turn++){
            if(check_winner() == player_char && mode == 1){
                system("cls");
                draw_table();
                printf("\nthe player wins !\n");
                end_game = true;
            }
            if(check_winner() == computer_char){
                system("cls");
                draw_table();
                printf("\nthe computer wins !\n");
                end_game = true;
            }
            if(end_game == true){
                break;
            }
            if(end_game == false){
                system("cls");
            }
            decide_turn(first_play,turn);
        }
        if(check_winner() == player_char && mode == 1){
            system("cls");
            draw_table();
            printf("\nthe player wins !\n");
            end_game = true;
        }
        if(check_winner() == computer_char){
            system("cls");
            draw_table();
            printf("\nthe computer wins !\n");
            end_game = true;
        }
        if(check_winner() == 'd'){
            system("cls");
            draw_table();
            printf("\nit is a draw!\n");
        }
    }
    char answer;
    clear();
    do{
        wait(1);
        answer = get_char("\ndo you want to play a new game ( y / n ) : ");
        if(answer == 'y' || answer == 'Y' || answer == 'n' || answer == 'N'){
            break;
        }
    }while(answer != 0);
    switch(answer){
    case 'y' : end_game = false;
               new_game();
               break;
    case 'Y' : end_game = false;
               new_game();
               break;
    case 'n' : system("cls");
               return;
    case 'N' : system("cls");
               return;
    }
}


void draw_table(){
    printf("\n");
    for(int y = 1 ; y < 10 ; y++){
        for(int x = 1 ; x < 18 ; x++){
            if(y % 3 == 0 && x % 6 != 0 && y != 9){
                printf("_");
            }
            if(x % 6 == 0){
                printf("|");
                continue;
            }
            if((y + 1) % 3 == 0 && (x + 3) % 6 == 0){
                printf("%c",square[(int)(((y + 1) / 3)) - 1][(int)((x - 3) / 6)]);
            }
            else if(y % 3 != 0 && x % 6 != 0){
                printf(" ");
            }
            else if(y == 9 && x % 6 != 0){
                printf(" ");
            }
        }
        printf("\n");
    }
}

void clear(){
    int num = 0;
    for(int y = 0 ; y < 3 ; y++){
        for(int x = 0 ; x < 3 ; x++){
            square[y][x] = num+49;
            num++;
        }
    }
    first_turn = true;
    mode = 0;
}


char check_winner(){
    char winner = 'd';
    for(int y = 0 ; y < 3 ; y++){
        if(square[y][0] == square[y][1] && square[y][1] == square[y][2]){
            winner = square[y][0];
            break;
        }
        if(square[0][y] == square[1][y] && square[1][y] == square[2][y]){
            winner = square[0][y];
            break;
        }
        if(square[0][0] == square[1][1] && square[1][1] == square[2][2]){
            winner = square[0][0];
            break;
        }
        if(square[0][2] == square[1][1] && square[1][1] == square[2][0]){
            winner = square[0][2];
            break;
        }
        for(int x = 0 ; x < 3 ; x++){
            if(square[y][x] > 48 && square[y][x] < 58){
                winner = '0';
            }
        }
    }
    return winner;
}



void player_turn(){
    char computer_turns[3][3] = {"   ","   ","   "};
    int played_cell;
    if(mode == 1 && first_turn == false  ){
        for(int i = 0 ; i < 3 ; i++){
            for(int j = 0 ; j < 3 ; j++){
                if(square[i][j] == computer_char && computer_turns[i][j] != computer_char){
                    computer_turns[i][j] = computer_char;
                    played_cell = (i*3)+j+1;
                }
            }
        }
        printf("computer(%c) took cell no. %i",computer_char,played_cell);
    }

    while(first_turn == true){
        player_char = get_char("\nX or O ? ");
        if(player_char == 'X' || player_char == 'x' || player_char == 'O' || player_char == 'o'){
            first_turn = false;
        }
        system("cls");
    }
    if(player_char == 'x' || player_char == 'o'){
        player_char -= 32;
    }
    int cell;
    do{
        draw_table();
        cell = get_int("\ncell : ");
        if(square[((cell - 1)/3)][((cell+2)%3)] == 'X' || square[(int)((cell - 1)/3)][((cell+2)%3)] == 'O'){
            system("cls");
            printf("cell %i is occupied\n",cell);
        }
        if(cell > 9 || cell < 0 ){
            system("cls");
            printf("choose a cell from 1 to 9\n");
        }
    }while(cell > 9 || cell < 0 || square[((cell - 1)/3)][((cell+2)%3)] == 'X' || square[(int)((cell - 1)/3)][((cell+2)%3)] == 'O');
    square[(int)((cell-1)/3)][(int)((cell+2)%3)] = player_char;

    draw_table();
}



void aware_choice(){
    int row_fl[] = {0,0,0} , row_blk[] = {0,0,0} , fr_row[] = {-1,-1,-1};
    int col_fl[] = {0,0,0} , col_blk[] = {0,0,0} , fr_col[] = {-1,-1,-1};
    int diag_fl[] = {0,0} , diag_blk[] = {0,0} , fr_diag[] = {-1,-1};
    int rowtfl = -1 , rowtblk = -1 , mptrow = -1 , row_arrsize = -1;
    int coltfl = -1 , coltblk = -1 , mptcol = -1 , col_arrsize = -1;
    int diagtfl = -1 , diagtblk = -1 , mptdiag = -1 , diag_arrsize = -1;
    if(first_turn == true){
        square[rand()%3][rand()%3] = computer_char;
        return;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // scaning section.
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
///////////////////////////////////////////////////////////////////////////////////////////////////////////
            //filling scan
            row_fl[i] += (square[i][j] == computer_char);
            col_fl[i] += (square[j][i] == computer_char);
            if(square[j][j] == computer_char && i == 0){
               diag_fl[0] ++;
            }
            if(square[j][2 - j] == computer_char && i == 0){
               diag_fl[1] ++;
            }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
            //block scan
            row_blk[i] += (square[i][j] == player_char);
            col_blk[i] += (square[j][i] == player_char);
            if(square[j][j] == player_char && i == 0){
               diag_blk[0] ++;
            }
            if(square[j][2 - j] == player_char && i == 0){
               diag_blk[1] ++;
            }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
            //empty scan
            if(square[j][0] <= '9' && square[j][1] <= '9' && square[j][2] <= '9' && i == 0){
                fr_row[row_arrsize] = j;
                row_arrsize++;
            }
            if(square[0][j] <= '9' && square[1][j] <= '9' && square[2][j] <= '9' && i == 0){
                fr_col[col_arrsize] = j;
                col_arrsize++;
            }
            if(square[0][0] <= '9' && square[1][1] <= '9' && square[2][2] <= '9' && i == 0 && j == 0){
                fr_diag[diag_arrsize] = 0;
                diag_arrsize++;
            }
            if(square[0][2] <= '9' && square[1][1] <= '9' && square[2][0] <= '9' && i == 0 && j == 0){
                fr_diag[diag_arrsize] = 1;
                diag_arrsize++;
            }
        }
    }
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
///////////////////////////////////////////////////////////////////////////////////////////////////////////
            //this section checks if there is a player_char in the row,col or diagonal.#filling
            if(square[i][j] == player_char){
                row_fl[i] = 0;
            }
            if(square[j][i] == player_char){
                col_fl[i] = 0;
            }
            if(square[j][j] == player_char && i == 1){
                diag_fl[0] = 0;
            }
            if(square[j][2 - j] == player_char && i == 1){
                diag_fl[1] = 0;
            }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
            //this section checks if there is a computer_char in the row,col or diagonal.#blocking
            if(square[i][j] == computer_char){
                row_blk[i] = 0;
            }
            if(square[j][i] == computer_char){
                col_blk[i] = 0;
            }
            if(square[j][j] == computer_char && i == 1){
                diag_blk[0] = 0;
            }
            if(square[j][2 - j] == computer_char && i == 1){
                diag_blk[1] = 0;
            }
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    //decision making section.
    for(int index = 0 ; index < 3 ; index++){
        //filling decision
        if(row_fl[index] >= 1){
            rowtfl = index;
            if(row_fl[index] >= 2){
                coltfl = -1,diagtfl = -1;
                goto fill_excution;
            }
        }
        if(col_fl[index] >= 1){
            coltfl = index;
            if(col_fl[index] >= 2){
                rowtfl = -1,diagtfl = -1;
                goto fill_excution;
            }
        }
        if(diag_fl[index - (index == 2)] >= 1){
            diagtfl = index - (index == 2);
            if(diag_fl[index] >= 2){
                rowtfl = -1,coltfl = -1;
                goto fill_excution;
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
        //blocking decision
        if(row_blk[index] == 2){
            rowtblk = index;
            break;
        }
        if(col_blk[index] == 2){
            coltblk = index;
            break;
        }
        if(diag_blk[index - (index == 2)] == 2){
            diagtblk = index - (index == 2);
            break;
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
        //empty decision
        if(fr_row[index] >= 0 && row_arrsize > 0){
            mptrow = fr_row[rand()%row_arrsize];
        }
        if(fr_col[index] >= 0 && col_arrsize > 0){
            mptcol = fr_col[rand()%col_arrsize];
        }
        if(fr_row[index - (index == 2)] >= 0 &&diag_arrsize > 0){
            mptdiag = fr_diag[rand()%diag_arrsize];
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    //excution section.
    //blocking excution
    for(int i = 0 ; i < 3 ; i++){
        if(rowtblk >= 0 && square[rowtblk][i] <= '9'){
            square[rowtblk][i] = computer_char;
            return;
        }
        if(coltblk >= 0 && square[i][coltblk] <= '9'){
            square[i][coltblk] = computer_char;
            return;
        }
        if(diagtblk == 0 && square[i][i] <= '9'){
            square[i][i] = computer_char;
            return;
        }
        if(diagtblk == 1 && square[i][2 - i] <= '9'){
            square[i][2 - i] = computer_char;
            return;
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    //filling excution
    fill_excution :
    for(int i = 0 ; i < 3 ; i++){
        if(rowtfl >= 0 && square[rowtfl][i] <= '9'){
            square[rowtfl][i] = computer_char;
            return;
        }
        if(coltfl >= 0 && square[i][coltfl] <= '9'){
            square[i][coltfl] = computer_char;
            return;
        }
        if(diagtfl == 0 && square[i][i] <= '9'){
            square[i][i] = computer_char;
            return;
        }
        if(diagtfl == 1 && square[i][2 - i] <= '9'){
            square[i][2 - i] = computer_char;
            return;
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    //empty excution
    //in case there is more than one empty way.
    int r = -1;
    int rn = rand()%3;
    if(mptrow >= 0 && mptcol >= 0 && mptdiag >= 0){
        r = rand() % 3;
        goto excute;
    }
    if(mptrow >= 0 && mptcol >= 0){
        r = rand() % 2;
    }
    if(mptrow >= 0 && mptdiag >= 0){
        r = rand() % 2;
        r += (r == 1);
    }
    if(mptdiag >= 0 && mptcol >= 0){
        r = (rand() % 2) + 1;
    }
    excute :
    {
        if(r >= 0){
            switch(r){
                case 0 : square[mptrow][rand()%3] = computer_char;
                         return;
                case 1 : square[rand()%3][mptcol] = computer_char;
                         return;
                case 2 : if(mptdiag == 0){
                            square[rn][rn] = computer_char;
                            return;
                         }
                         if(mptdiag == 1){
                            square[rn][2 - rn] = computer_char;
                            return;
                         }
            }
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    //incase there is one empty way.
    if(mptrow >= 0){
        square[mptrow][rand() % 3] = computer_char;
        return;
    }
    if(mptcol >= 0){
        square[rand() % 3][mptcol] = computer_char;
        return;
    }
    if(mptdiag == 0){
        square[rn][rn] = computer_char;
        return;
    }
    if(mptdiag == 1){
        square[rn][2 - rn] = computer_char;
        return;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    //in case there is no all the above didn't work it will fill the first empty cell it encounters.
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            if(square[i][j] <= '9'){
                square[i][j] = computer_char;
                return;
            }
        }
    }
}


void computer_turn(){
    int empty_cells[]={0,0,0,0,0,0,0,0,0};
    if(first_turn == true){
        int x = rand() % 2;
        switch(x){
            case 0  :     computer_char = 'X';
                          player_char = 'O';
            case 1  :     computer_char = 'O';
                          player_char = 'X';
        }
        first_turn = false;
    }
    else{
       switch(player_char){
            case 'X' : computer_char = 'O';
                       break;
            case 'O' : computer_char = 'X';
                       break;
       }
    }
    if((rand() % difficulty) == 0){
        aware_choice();
        return;
    }
    int arr_size = 0;
    for(int cell = 0 ; cell < 9 ; cell++){
        if(square[cell/3][(cell+3)%3] <= '9'){
            empty_cells[arr_size] = cell;
            arr_size++;
        }
    }
    int cell = empty_cells[rand()% (arr_size + (arr_size == 0))];
    square[cell/3][(cell+3)%3] = computer_char;
}
