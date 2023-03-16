#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <cs50.h>


void wait(int seconds);
void clear_terminal();
char square [3][3] = {"123","456","789"};
bool first_turn = true , end_game = false;
int mode = 0 , difficulty = 3;
char player_char , computer_char;
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
    for(long start = time(NULL) , end = start ; end - start != seconds ; end = time(NULL)){
        int x = 0;
    }
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
    system("clear");
    while(mode < 1 || mode > 2){
        mode = get_int("\ncomputer vs player (1)\nplayer vs player (2)\n\n:> ");
    }
    system("clear");
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
            system("clear");
            printf("\nplayer %i (%c) : \n",(turn % 2) + 1,player_char);
            player_turn();
            system("clear");
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
            difficulty = get_int("\nchoose a difficulty \n\n1 is the hardest  3 is the easiest \n\n:> ");
        }while(difficulty < 1 || difficulty > 3);
        system("clear");
        char first_play;
        if(check_winner() != computer_char){
            first_play = 'p';
        }
        else if(check_winner() == computer_char){
            first_play = 'c';
        }
        for(int turn = 0 ; turn < 9 ; turn++){
            if(check_winner() == player_char && mode == 1){
                draw_table();
                printf("\nthe player wins !\n");
                end_game = true;
            }
            if(check_winner() == computer_char){
                draw_table();
                printf("\nthe computer wins !\n");
                end_game = true;
            }
            if(end_game == true){
                break;
            }
            if(end_game == false){
                system("clear");
            }
            decide_turn(first_play,turn);
        }

        if(check_winner() == 'd'){
            printf("\nit is a draw!\n");
        }
    }
    char answer;
    clear();
    do{
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
    case 'n' : system("clear");
               return;
    case 'N' : system("clear");
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
                printf("%c",square[((y + 1) / 3) - 1][(x - 3) / 6]);
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
    if(mode == 1 && first_turn == false  ){
        printf("\nplayer turn (%c) : \n\n",player_char);
    }

    while(first_turn == true){
        player_char = get_char("\nX or O ? ");
        if(player_char == 'X' || player_char == 'x' || player_char == 'O' || player_char == 'o'){
            first_turn = false;
        }
        system("clear");
    }
    if(player_char == 'x' || player_char == 'o'){
        player_char -= 32;
    }
    int cell;
    do{
        draw_table();
        cell = get_int("\ncell : ");
        if(square[(cell/3)-((cell+3)%3 == 0)][((cell+3)%3)-1+3*((cell+3)%3 == 0)] == 'X' || square[(cell/3)-((cell+3)%3 == 0)][((cell+3)%3)-1+3*((cell+3)%3 == 0)] == 'O'){
            system("clear");
            printf("cell %i is occupied\n",cell);
        }
        if(cell > 9 || cell < 0 ){
            system("clear");
            printf("choose a cell from 1 to 9\n");
        }
    }while(cell > 9 || cell < 0 || square[(cell/3)-((cell+3)%3 == 0)][((cell+3)%3)-1+3*((cell+3)%3 == 0)] == 'X' || square[(cell/3)-((cell+3)%3 == 0)][((cell+3)%3)-1+3*((cell+3)%3 == 0)] == 'O');
    square[(cell/3)-((cell+3)%3 == 0)][((cell+3)%3)-1+3*((cell+3)%3 == 0)] = player_char;

    draw_table();
}



void aware_choice(){
    for(int y = 0 ; y < 3 ; y++){
        for(int x = 0 ; x < 3 ; x++){

        }
    }
}



void computer_turn(){
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
    int row = rand() % 3 , column = rand() % 3;
    if(square[row][column] == 'X' || square[row][column] == 'O'){
        return computer_turn();
    }
    square[row][column] = computer_char;
}
