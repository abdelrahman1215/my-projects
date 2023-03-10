#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <cs50.h>


char positions [3][3] = {"123","456","789"};
bool first_turn = true;
bool end_game = false;
char player_char , computer_char;
void draw_table();
void player_turn();
void computer_turn();
char check_winner();
void new_game();
void clear();
void decide_turn(char player , int turn);


int main(){
    srand(time(NULL));
    new_game();
}


void decide_turn(char player , int turn){
    turn ++;
    if(player == 'p' && (turn / 2) * 2 != turn){
        player_turn();
    }
    if(player == 'p' && (turn / 2) * 2 == turn){
        computer_turn();
    }
    if(player == 'c' && (turn / 2) * 2 != turn){
        computer_turn();
    }
    if(player == 'c' && (turn / 2) * 2 == turn){
        player_turn();
    }
}


void new_game(){
    draw_table();
    char first_play;
    if(check_winner() != computer_char){
        first_play = 'p';
    }
    else if(check_winner() == computer_char){
        first_play = 'c';
    }
    for(int turn = 0 ; turn < 9 ; turn++){
        decide_turn(first_play,turn);
        if(end_game == true){
            break;
        }
    }
    if(check_winner() == 'd'){
        printf("it is a draw!\n");
    }
    char answer;
    clear();
    do{
        answer = get_char("do you want to play a new game ( y / n ) : ");
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
        case 'n' : return;
        case 'N' : return;
    }
}


void draw_table(){
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
                printf("%c",positions[((y + 1) / 3) - 1][(x - 3) / 6]);
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
            positions[y][x] = num+49;
            num++;
        }
    }
    first_turn = true;
}


char check_winner(){
    char winner = 'd';
    for(int y = 0 ; y < 3 ; y++){
        if(positions[y][0] == positions[y][1] && positions[y][1] == positions[y][2]){
            winner = positions[y][0];
            break;
        }
        if(positions[0][y] == positions[1][y] && positions[1][y] == positions[2][y]){
            winner = positions[0][y];
            break;
        }
        if(positions[0][0] == positions[1][1] && positions[1][1] == positions[2][2]){
            winner = positions[0][0];
            break;
        }
        if(positions[0][2] == positions[1][1] && positions[1][1] == positions[2][0]){
            winner = positions[0][2];
            break;
        }
        for(int x = 0 ; x < 3 ; x++){
            if(positions[y][x] > 48 && positions[y][x] < 58){
                winner = '0';
            }
        }
    }
    return winner;
}



void player_turn(){
    while(first_turn == true){
        player_char = get_char("X or O ? ");
        if(player_char == 'X' || player_char == 'x' || player_char == 'O' || player_char == 'o'){
            first_turn = false;
        }
    }
    if(player_char == 'x' || player_char == 'o'){
        player_char -= 32;
    }
    int cell = get_int("cell : ");
    if(cell > 9 || cell < 0){
        printf("you can't place your turn in cell : %i\n",cell);
        draw_table();
        return player_turn();
    }
    switch(cell){
        case 1 : if(positions[cell/3][0] == 'X' || positions[cell/3][0] == 'O'){
                    printf("you can't place your turn in cell : %i\n",cell);
                    draw_table();
                    return player_turn();
                 }
                 positions[cell/3][0] = player_char;
                 break;
        case 2 : if(positions[cell/3][1] == 'X' || positions[cell/3][1] == 'O'){
                    printf("you can't place your turn in cell : %i\n",cell);
                    draw_table();
                    return player_turn();
                 }
                 positions[cell/3][1] = player_char;
                 break;
        case 3 : if(positions[(cell/3)-1][2] == 'X' || positions[(cell/3)-1][2] == 'O'){
                    printf("you can't place your turn in cell : %i\n",cell);
                    draw_table();
                    return player_turn();
                 }
                 positions[(cell/3)-1][2] = player_char;
                 break;
        case 4 : if(positions[cell/3][0] == 'X' || positions[cell/3][0] == 'O'){
                    printf("you can't place your turn in cell : %i\n",cell);
                    draw_table();
                    return player_turn();
                 }
                 positions[cell/3][0] = player_char;
                 break;
        case 5 : if(positions[cell/3][1] == 'X' || positions[cell/3][1] == 'O'){
                    printf("you can't place your turn in cell : %i\n",cell);
                    draw_table();
                    return player_turn();
                 }
                 positions[cell/3][1] = player_char;
                 break;
        case 6 : if(positions[(cell/3)-1][2] == 'X' || positions[(cell/3)-1][2] == 'O'){
                    printf("you can't place your turn in cell : %i\n",cell);
                    draw_table();
                    return player_turn();
                 }
                 positions[(cell/3)-1][2] = player_char;
                 break;
        case 7 : if(positions[cell/3][0] == 'X' || positions[cell/3][0] == 'O'){
                    printf("you can't place your turn in cell : %i\n",cell);
                    draw_table();
                    return player_turn();
                 }
                 positions[2][0] = player_char;
                 break;
        case 8 : if(positions[cell/3][1] == 'X' || positions[cell/3][1] == 'O'){
                    printf("you can't place your turn in cell : %i\n",cell);
                    draw_table();
                    return player_turn();
                 }
                 positions[2][1] = player_char;
                 break;
        case 9 : if(positions[(cell/3)-1][2] == 'X' || positions[(cell/3)-1][2] == 'O'){
                    printf("you can't place your turn in cell : %i\n",cell);
                    draw_table();
                    return player_turn();
                 }
                 positions[(cell/3)-1][2] = player_char;
                 break;
    }
    draw_table();
    if(check_winner() == player_char){
        printf("the player wins\n");
        end_game = true;
    }
}




void computer_turn(){
    if(first_turn == true){
        int x = (rand() % 3);
        switch(x){
            case 1  :     computer_char = 'X';
                          player_char = 'O';
            case 2  :     computer_char = 'O';
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

    int row_ = rand() % 3 , column_ = rand() % 3;
    if(positions[row_ ][column_] == 'X' || positions[row_ ][column_] == 'O'){
        return computer_turn();
    }
    printf("computer turn : \n");
    positions[row_][column_] = computer_char;
    draw_table();
    if(check_winner() == computer_char){
        printf("the computer wins\n");
        end_game = true;
    }
}
