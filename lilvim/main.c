#include <stdio.h>
#include <conio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct cursor{
    int x;
    int y;
} cursor;

typedef struct node{
    long line_no;
    long line_start;
    struct node * next;
}node;

#define up 72
#define down 80
#define left 75
#define right 77

COORD wsize();
COORD cur_pos();

void title();
void print_mode();

void create_line_map();
void update_line_map();
node line0 = {.line_no = 0 , .line_start = 0 , .next = NULL};
node * line_map = &line0;
void add_node(node *n);

void print_lines(int start , int end);

char * line(int line_number);
char * line_ptr;

void move_cursor(int x , int y);

void insert_mode();

void add(char c , long line_to_edit , long position);

void delete(long line_to_edit , long position);

void load_file(char * file_name);
long line_max;
long size;

void copy_file(char * source , char * destination);

void create_swap(char * file_name);
char * swap_name;

void app_interface();
char mode[] = "             ";
void switch_directions(int *update_preview);

cursor preview_cursor = {0,2};
int start_line;

void update_interface(COORD * size_ptr , int update_preview);

int line_indentation = 0;

void unhighlight();

void highlight();

void run_command(char * command);

    void quit();

    void save();

        char * opened_file;

    void manual();

    void run_goto(int *i , char *command);

    void go_to(int mode , void *input);

        int power(int number , int indice); 

        int string_to_int(char *);
    //defining modes for goto

    #define number 0

    void err_msg(char * message , ... );

    void msg  (int line , char * message  , ... );

void clr_command_line();

void clr_ln(int line_number);


void title(){
    clr_ln(0);
    int x = (wsize().X / 2);
    x -= (strlen(opened_file)/2);

    move_cursor(0,0);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0B);
    printf("type ':man' for the manual");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);

    move_cursor(x,0);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0B);
    printf("%s",opened_file);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);
}


void print_mode(){
    clr_ln(wsize().Y - 4);
    int x = (wsize().X / 2);
    x -= (strlen(mode)/2);

    move_cursor(x,wsize().Y - 4);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0B);
    printf("%s",mode);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);
}


void app_interface(){
    start_line = 0;
    COORD win_size = {0,0};
    strcpy(mode,"NORMAL MODE");
    print_mode();
    update_interface(&win_size , 1);

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    char command[31];
    command[30] = 0;

    int update_preview = 0;
    highlight();

    while(1){
        update_interface(&win_size , update_preview);
        update_preview = 0;
        for(int  i = 0 ; i < 31 ; i++){
            command[i] = '\000';
        }
        if(!_kbhit()){
            continue;
        }
        char key = getch();
        if(key != ':' && key != '$' && key != 'i' && key >= 0){
            continue;
        }

        if(key < 0){
            switch_directions(&update_preview);
            continue;
        }
        switch(key){
            case 'i' :
                strcpy(mode,"INSERT MODE");
                print_mode();
                if((preview_cursor.y - 2) + start_line > line_max){
                    preview_cursor.y = line_max - start_line + 2;
                }
                insert_mode();
                break;

            case ':' :
                strcpy(mode,"COMMAND MODE");
                print_mode();
                clr_command_line();
                unhighlight();
                move_cursor(0,wsize().Y - 3);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E);
                printf(":");
                fgets(command,30,stdin);
                strcpy(mode,"NORMAL MODE");
                print_mode();
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);
                highlight();
                break;

            case '$' :
                strcpy(mode,"TERMINAL MODE");
                print_mode();
                clr_command_line();
                unhighlight();
                move_cursor(0,wsize().Y - 3);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E);
                printf("%c ",175);//»
                command[0] = '$';
                fgets(command + 1,29,stdin);
                strcpy(mode,"NORMAL MODE");
                print_mode();
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);
                highlight();
                for(int i = 0 ; command[i] != 0 ; i++){
                    if(command[i] == 'c' && command[i + 1] == 'l'){
                        win_size.X ++;
                        break;
                    }
                }
                break;
        }
        for(int  i = 0 ; i < 30 ; i++){
            if(command[i] == '\n'){
                command[i] = 0;
                break;
            }
        }
        clr_command_line();
        run_command(command);
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    }
}


void clr_command_line(){
    clr_ln(wsize().Y - 3);
    clr_ln(wsize().Y - 2);
    clr_ln(wsize().Y - 1);
}


void update_interface(COORD * size_ptr , int update_preview){
    if(size_ptr -> X == 0){
        switch((int)(wsize().Y) >= 300){
            case 1 : 
                size_ptr -> X = wsize().X /300;
                size_ptr -> Y = wsize().Y /300;
                break;
            case 0 :
                size_ptr -> X = wsize().X;
                size_ptr -> Y = wsize().Y;
                break;
        }
    }else{
        if(wsize().X != size_ptr -> X || wsize().Y != size_ptr -> Y){
            update_line_map();
            size_ptr -> X = wsize().X;
            size_ptr -> Y = wsize().Y;
            system("cls");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E);
            title();
            print_mode();
            move_cursor(0,2);
            line_indentation = 0;
            print_lines(start_line,start_line + size_ptr -> Y - 7);
            line_indentation = 0;
            highlight();
        }
    }

    printf("\e[?25h");
    int line_to_clr;
    int start;

    if(update_preview){
        if(start_line + size_ptr -> Y - 6 > line_max){
            start = (start_line + size_ptr -> Y - 7) - line_max;
            line_to_clr = size_ptr -> Y - start;
            line_to_clr -= 5;
            clr_ln(line_to_clr);
        }
        move_cursor(0,2);
        print_lines(start_line,start_line + size_ptr -> Y - 7);
        highlight();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E);
    }
    move_cursor(preview_cursor.x,preview_cursor.y);
}


void unhighlight(){
    printf("\e[?25l");

    move_cursor(0,preview_cursor.y);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);

    char line_to_unhi[wsize().X - 1];
    line_to_unhi[wsize().X - 2] = 0;
    if(line_indentation < (int)strlen(line(start_line + preview_cursor.y - 2))){
        strncpy(line_to_unhi , &line_ptr[line_indentation] , wsize().X - 2);
    }else{
        line_to_unhi[0] = 0;
    }
    puts(line_to_unhi);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E);

    printf("\e[?25h");
}


void highlight(){
    printf("\e[?25l");

    move_cursor(0,preview_cursor.y);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E);

    char line_to_hi[wsize().X - 1];
    line_to_hi[wsize().X - 2] = 0;
    if(line_indentation < (int)strlen(line(start_line + preview_cursor.y - 2))){
        strncpy(line_to_hi , &line_ptr[line_indentation] , wsize().X - 2);
    }else{
        line_to_hi[0] = 0;
    }
    puts(line_to_hi);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);

    printf("\e[?25h");
}


int power(int num , int indice){
    int ret = 1;
    for(int i = 0 ; i < indice ; i++){
        ret *= num;
    }

    return ret;
}


int string_to_int(char * par){
    int res = 0;

    for (int i = 1 ; i <= (int) strlen(par) ; i++){
        res += (par[i - 1] - 48) * power( 10 , strlen(par) - i);
    }

    return res;
}


void run_command(char * command){
    COORD bla = {0,0};
    int bla_bla = 0;
    for(int i = 0 ; command[i] != 0 ; i++){
        switch(command[i]){
            case ' ' :
                break;
            case 'q' :
                quit();
                break;
            case 'w' : 
                save();
                msg(wsize().Y - 3 , "wrote the changes into %s" , opened_file);
                break;
            case '$' :
                bla_bla = 1;
                system(&command[i + 1]);
                system("cls");
                title();
                print_mode();
                update_interface(&bla , bla_bla);
                printf("\e[?25h");
                return;
            case 'm' :
                if(command[i + 1] == 'a' && command[i + 2] == 'n'){
                    i += 3;
                    manual();
                    return;
                }else{
                    err_msg("this command \"%c\" doesn't exist",command[i]);
                    return;
                }
            case 'g' :
                if(command[i + 1] == 'o' && command[i + 2] == 't' && command[i + 3] == 'o' && command[i + 4] == ' '){
                    i += 4;
                    run_goto(&i , command);
                    return;
                }else{
                    err_msg("this command \"%c\" doesn't exist",command[i]);
                    return;
                }
            default :
                err_msg("this command \"%c\" doesn't exist",command[i]);
                return;
        }
    }
}


void quit(){
    remove(swap_name);
    remove(swap_name);
    free(line_ptr);
    free(swap_name);
    free(opened_file);

    if(line_map -> next != NULL){
        while(line_map -> next != NULL){
            node *tmp = line_map -> next -> next;
            free(line_map -> next);
            line_map -> next = tmp;
        }
    }

    system("cls");
    exit(0);
}


void save(){
    copy_file(swap_name,opened_file);
}


void manual(){
    FILE *manuals = fopen("manual.txt","r");
    fseek(manuals , 0L , SEEK_END);
    size_t man_size = ftell(manuals);
    fseek(manuals , 0L , SEEK_SET);

    char man[man_size + 1];
    fread(man,1,man_size,manuals);

    for(size_t i = 0 ; i < man_size ; i++){
        if(man[i] < 32 || man[i] > 126 ){
            if(man[i] != '\n' && man[i] != '\t'){
                man[i] = 0;
                break;
            }
        }
    }

    fclose(manuals);

    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E);
    title();

    move_cursor(0,2);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E);
    printf("%s",man);

    for(char key = getch() ; key != 'q' && key != 'Q' ; key = getch()){}

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E);
    system("cls");

    title();
    COORD bla = {0,0};
    update_interface(&bla,1);
    return;
}


void run_goto(int *i , char *command){
    for(int j = *i ; command[j + 3] != 0; j++ ){
        if(command[j] == 's' && command[j + 1] == 't' && command[j + 2] == 'a' && command[j + 3] == 'r' && command[j + 4] == 't'){
            int zero = 0;
            go_to(number,&zero);
            return;
        }
    }

    for(int j = *i ; command[j + 1] != 0 ; j++ ){
        if(command[j] == 'e' && command[j + 1] == 'n' && command[j + 2] == 'd'){
            int end = line_max - (wsize().Y - 6);
            go_to(number,&end);
            return;
        }
    }

    for(int j = *i ; j < (int) strlen(command) ; j++ , *i = *i + 1){
        if( command[j] >= '0' && command[j] <= '9' ){
            break;
        }
    }


    int length = 0;
    for(int j = *i ; j < (int) strlen(command) ; j++ , length++){
        if( command[j] < '0' || command[j] > '9'){
            break;
        }
    }

    if(length == 0){
        err_msg("the goto command requires input");
        return;
    }

    char number_str[length + 1];
    number_str[length] = 0;

    for(int j = *i ; j - *i < length ; j++ ){
        number_str[j - *i] = command[j];
    }

    int number_val = string_to_int(number_str) - 1;

    if(number_val > line_max){
        err_msg("line %i doesn't exit",number_val + 1);
        return;
    }

    if(number_val == -1){
        err_msg("the goto command starts counting from 1");
        return;
    }

    go_to(number,&number_val);
}


void go_to(int mode , void *input){
    preview_cursor.x = 0;
    preview_cursor.y = 2;

    switch(mode){
        case number :
            goto num;
            break;
    }


    num :
    {
        COORD s = {0,0};
        start_line = *(int *)input;
        update_interface(&s,1);
        return;
    }

}


void err_msg (char * message , ... ) {
    move_cursor(0,wsize().Y - 3);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x04);
    printf("E:");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);

    va_list args_ptr;
    va_start(args_ptr,message);

    vprintf(message,args_ptr);

    va_end(args_ptr);
}


void msg  (int line , char * message  , ... ) {
    move_cursor(0,line);

    va_list args_ptr;
    va_start(args_ptr,message);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);
    vprintf(message,args_ptr);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);

    va_end(args_ptr);
}


void clr_ln(int line_number){
    printf("\e[?25l");
    move_cursor(0,line_number);
    for(int  i = 0 ; i < wsize().X ; i++){
        printf(" ");
    }
    move_cursor(0,line_number);
    printf("\e[?25h");
}


void add_node(node *n){
    for(node * iteration = line_map; iteration != NULL ; iteration = iteration -> next){
        if(iteration -> next == NULL){
            iteration -> next = n;
            break;
        }else if(n -> line_no == iteration -> next -> line_no){
            iteration -> next -> line_no = n -> line_start;
            break;
        }
    }
}


void create_line_map(){

    FILE * copier = fopen(swap_name,"r");

    char current_char [3];
    current_char [0] = ' ';
    current_char [1] = ' ';
    current_char [2] = 0;

    int offset = 0;
    int current_line = 0;

    fseek(copier , 0L , SEEK_SET);

    for(offset = 0 , current_line = 0 ; offset <= size ; offset++){
        fseek(copier,offset,SEEK_SET);
        fread(current_char,1,2,copier);
        if(current_char[0] == '\n'){
            offset ++;
            current_line ++;
            node * addition = malloc(sizeof(node));
            addition -> line_no = current_line;
            addition -> line_start = offset + 1;
            addition -> next = NULL;
            add_node(addition);
            fseek(copier,offset,SEEK_SET);
        }
    }

    fclose(copier);
}


void update_line_map(){
    
    FILE * copier = fopen(swap_name,"r");
    FILE * tell_size = fopen(swap_name,"r");

    fseek(tell_size,0,SEEK_END);
    size_t file_size = ftell(tell_size) - 2;

    fclose(tell_size);

    char current_char [3];
    current_char [0] = ' ';
    current_char [1] = ' ';
    current_char [2] = 0;

    size_t offset = 0;
    int current_line = 0;

    fseek(copier , 0L , SEEK_SET);

    node * iteration = line_map;


    for(offset = 0 , current_line = 0 ; offset <= file_size ; offset++){
        fseek(copier,offset,SEEK_SET);
        fread(current_char,1,2,copier);
        if(current_char[0] == '\n'){
            offset ++;
            current_line ++;
            fseek(copier,offset,SEEK_SET);
            if(iteration -> next == NULL){
                node *addition = malloc(sizeof(node));
                addition -> line_no = current_line;
                addition -> line_start = offset;
                addition -> next = NULL;
                add_node(addition);
                line_max++;
            }
            iteration -> next -> line_start = offset + 1;
            iteration = iteration -> next;
        }

        if(offset == file_size && iteration -> next != NULL){
            free(iteration -> next);
            iteration -> next = NULL;
            line_max--;
        }
    }

    fclose(copier);
}


void print_lines(int start , int end){
    printf("\e[?25l");

    int  rows = wsize().Y - 6;
    int  cols = wsize().X - 2;
    char text[rows][cols + 1];

    for(int i = 0 ; i < rows ; i++){
        for(int j = 0 ; j < cols ; j++){
            text[i][j] = ' ';
        }
        text[i][cols] = '\n';
    }

    text[rows - 1][cols] = '\000';

    for(int i = 0 ; i <= end - start ; i++){
        if(start + i > line_max){
            text[i][0] = '~';
            continue;
        }
        if(line_indentation < (int)strlen(line(i + start))){
            strncpy(text[i] , &line_ptr[line_indentation] , cols);
        }else{
            continue;
        }
        for(int j = 0 ; j < cols ; j++){
            if(text[i][j] == 0){
                text[i][j] = ' ';
            }
        }
    }

    text[rows - 1][cols] = '\000';

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE) , 0x07);

    puts(text[0]);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE) , 0x0E);

    printf("\e[?25h");
}


char * line(int line_number){
    FILE * copier = fopen(swap_name,"r");

    if(line_number > line_max){
        return "";
    }

    char current_char [3];
    current_char [0] = ' ';
    current_char [1] = ' ';
    current_char [2] = 0;

    fseek(copier , 0L , SEEK_SET);

    int line_start = -1;

    if(line_number == 0){
        line_start = 0;
    }else for(node * iteration = line_map -> next; iteration != NULL ; iteration = iteration -> next){
        if(iteration -> line_no == line_number){
            line_start = iteration -> line_start;
            break;
        }
    }

    if(line_start == -1){
        return "";
    }

    int line_length = 0;

    int  copied_char = 1;

    if(current_char[0] == '\n'){
        current_char[0] = 1;
    }
    if(current_char[1] == '\n'){
        current_char[0] = 1;
    }

    for(long offset = line_start ; copied_char != 0 ; offset++ , line_length += copied_char != 0){
        if(current_char[0] == '\n'){
            break;
        }
        if(current_char[0] == '\000'){
            break;
        }
        fseek(copier,offset,SEEK_SET);
        copied_char = fread(current_char,1,2,copier);
    }

    line_ptr = malloc(line_length + 1);
    line_ptr[line_length] = 0;

    fseek( copier , line_start , SEEK_SET );
    fread( line_ptr , 1 , line_length , copier);
    fclose(copier);


    for(size_t i = 0 ; i < strlen(line_ptr) ; i++){
        if(line_ptr[i] == '\n'){
            line_ptr[i] = '\000';
            break;
        }
    }

    for(size_t i = 0 ; i < strlen(line_ptr) ; i++){
        if(line_ptr[i] == '\t'){
            line_ptr[i] = ' ';
        }
    }

    return line_ptr;
}


COORD wsize(){
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
    COORD ret = {info.dwSize.X , info.dwSize.Y};
    return ret;
}


COORD cur_pos(){
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
    COORD ret = {info.dwCursorPosition.X , info.dwCursorPosition.Y};
    return ret;
}


void load_file(char * file_name){
    opened_file = malloc(strlen(file_name) + 1);
    strcpy(opened_file,file_name);
    opened_file[strlen(file_name)] = 0;

    FILE * file = fopen(file_name , "r");

    line_max = 0;

    int keep_reading = 1;

    while(keep_reading){
        char count = fgetc(file);

        keep_reading = count > 0;

        if(count == '\n'){
            line_max ++;
        }
    }

    if(file == NULL){
        printf("this file doesn't exist\n");
        quit();
    }

    fseek(file , 0L , SEEK_END);
    size = ftell(file) - 2;
    fclose(file);

    create_swap(file_name);
    title();
    create_line_map();
}


void create_swap(char * file_name){
    int file_name_len = strlen(file_name); 
    char swap_file_name[file_name_len + 5];
    for(int i = 0 ; i < file_name_len ; i++){
        swap_file_name[i] = file_name[i];
    }
    swap_file_name[file_name_len] = '.';
    swap_file_name[file_name_len + 1] = 's';
    swap_file_name[file_name_len + 2] = 'w';
    swap_file_name[file_name_len + 3] = 'p';
    swap_file_name[file_name_len + 4] = '\000';

    swap_name = malloc(file_name_len + 5);
    strcpy(swap_name,swap_file_name);
    
    copy_file(file_name,swap_name);
}


void copy_file(char * source , char * destination){
    FILE * src = fopen(source,"r");
    FILE * dest = fopen(destination,"w");
    if(source == NULL){
        return;
    }

    for(char get_char = fgetc(src) ; get_char != EOF ; get_char = fgetc(src)){
        fputc(get_char , dest);
    }

    fclose(src);
    fclose(dest);
}


void move_cursor(int x , int y){
    COORD position = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),position);
}


void insert_mode(){
    COORD win_size = {0,0};
    update_interface(&win_size , 0);

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

    int update_preview = 0;
    highlight();

    while(1){
        update_interface(&win_size , update_preview);
        update_preview = 0;

        if(!_kbhit()){
            continue;
        }

        char key = getch();

        if(key < 0){
            switch_directions(&update_preview);
        }else{
            switch(key){
                case 27 : 
                    update_line_map();
                    strcpy(mode,"NORMAL MODE");
                    print_mode();
                    return;
                
                case 8 :
                    if((preview_cursor.y - 2) + start_line == 0 && preview_cursor.x + line_indentation == 0){
                        printf("\a");
                        break;
                    }
                
                    size_t length_of_upper_line = strlen(line((preview_cursor.y - 3) + start_line))  + 1;
                    if((int)length_of_upper_line > wsize().X - 2){
                        length_of_upper_line = wsize().X - 2;
                    }
                    
                    delete((preview_cursor.y - 2) + start_line , preview_cursor.x + line_indentation - 1);
                    if(preview_cursor.x == 0){
                        preview_cursor.x = length_of_upper_line;
                        if(preview_cursor.y == 2){
                            start_line--;
                        }else{
                            preview_cursor.y--;
                        }
                        update_line_map();
                        update_preview = 1;
                    }
                    if(preview_cursor.x > 0){
                        preview_cursor.x --;
                    }else{
                        line_indentation --;
                        update_line_map();
                        update_preview = 1;
                    }
                    clr_ln(preview_cursor.y);
                    highlight();
                
                    break;

                case '\r' :
                    add('\n',(preview_cursor.y - 2) + start_line , preview_cursor.x + line_indentation);

                    update_line_map();
                    preview_cursor.x = 0;
                    if(preview_cursor.y == wsize().Y - 5){
                        start_line++;
                    }else{
                        preview_cursor.y ++;
                    }
                    line_indentation = 0;
                    update_interface(&win_size,1);

                    break;

                default :
                    add(key,(preview_cursor.y - 2) + start_line , preview_cursor.x + line_indentation);

                    if(preview_cursor.x == wsize().X - 2){
                        line_indentation++;
                        update_line_map();
                        update_interface(&win_size,1);
                        highlight();
                    }

                    if(preview_cursor.x < wsize().X - 2){
                        preview_cursor.x++;
                        highlight();
                    }
            }
        }

        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    }
}


void switch_directions(int *update_preview){
    switch(getch()){
        case up : 
            if(mode[0] == 'I'){
                update_line_map();
            }

            size_t line_len_up = strlen(line((preview_cursor.y + start_line) - 3));
            if(preview_cursor.y == 2){
                if(start_line == 0){
                    printf("\a");
                    break;
                }
                start_line -= start_line > 0;
                *update_preview = 1;
                line_indentation = 0;
                break;
            }
            if(preview_cursor.x > (int)line_len_up){
                preview_cursor.x = line_len_up;
            }

            if(line_indentation > 0){
                line_indentation = 0;
                *update_preview = 1;
            }

            unhighlight();
            preview_cursor.y--;
            highlight();
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E);
            break;
        case down : 
            if(mode[0] == 'I' && (preview_cursor.y - 2) + start_line == line_max){
                printf("\a");
                return;
            }

            if(mode[0] == 'I'){
                update_line_map();
            }

            size_t line_len_down = strlen(line((preview_cursor.y + start_line) - 1));
            if(preview_cursor.y == wsize().Y - 5){
                if(start_line - (wsize().X - 5) == line_max){
                    printf("\a");
                    break;
                }
                start_line += start_line < line_max;
                *update_preview = 1;
                line_indentation = 0;
                break;
            }
            if(preview_cursor.x > (int)line_len_down){
                preview_cursor.x = line_len_down;
            }

            if(line_indentation > 0){
                line_indentation = 0;
                *update_preview = 1;
            }

            unhighlight();
            preview_cursor.y++;
            highlight();
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E);
            break;
        case right : 
            if(preview_cursor.x == wsize().X - 2){
                if(preview_cursor.x + line_indentation < (int)strlen(line((preview_cursor.y + start_line) - 2))){
                    line_indentation ++;
                    *update_preview = 1;
                    break;
                }
                printf("\a");
                break;
            }

            if(preview_cursor.x == (int)strlen(line((preview_cursor.y - 2) + start_line))){
                printf("\a");
                break;
            }

            preview_cursor.x++;
            break;
        case left : 
            if(preview_cursor.x == 0){
                if(line_indentation > 0){
                    line_indentation --;
                    *update_preview = 1;
                    break;
                }
                printf("\a");
                break;
            }
            preview_cursor.x--;
            break;
    }
}

void add(char c , long line_to_edit , long position){

    for(node *i = line_map ; i != NULL ; i = i -> next){
        if(i -> line_no == line_to_edit){
            position += i -> line_start;
            break;
        }
    }

    FILE *get = fopen(swap_name,"r");
    FILE *put = fopen(swap_name,"r+");

    fseek(get,position,SEEK_SET);
    fseek(put,position,SEEK_SET);

    fputc(c,put);

    for(char i = fgetc(get) ; i != EOF ; i = fgetc(get)){
        fputc(i,put);
    }

    fclose(get);
    fclose(put);

}


void delete(long line_to_edit , long position){

    for(node *i = line_map ; i != NULL ; i = i -> next){
        if(i -> line_no == line_to_edit){
            position += i -> line_start;
            break;
        }
    }

    FILE *get = fopen(swap_name,"r");
    fseek(get , 0L , SEEK_END);

    size_t size = ftell(get);

    fseek(get , 0L , SEEK_SET);

    char text[size];
    text[size - 9] = 0;

    position -= line_to_edit * (preview_cursor.x + line_indentation == 0);

    if(position > 0){
        fread(text,1,position,get);
    }

    position += line_to_edit * (preview_cursor.x + line_indentation == 0);

    fseek(get , (long)position + 1 , SEEK_SET);
    fread(&text[position - line_to_edit],1,(size - position) - 1,get);    

    text[size - 11] = 0;

    fclose(get);

    FILE *put = fopen(swap_name,"w");

    fputs(text , put);

    fclose(put);

}