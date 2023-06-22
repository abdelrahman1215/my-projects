#include "main.c"

int main(int argc , char * argv[]){
    line_ptr =  NULL;

    if(argc > 2){
        printf("too many arguments\n");
        return 1;
    }

    if(argc == 1){
        printf("too few arguments\n");
        return 1;
    }

    system("cls");
    load_file(argv[1]);
    app_interface();

    return 0;
}