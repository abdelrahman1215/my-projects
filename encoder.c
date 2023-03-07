#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>

int check(); // to check that the string is not made up of only zeros

int check_encryption; // its value is changed by get_encryption() then used by decrypt() to check if the file is empty

void new_encryption(); // makes a new code

void get_encryption(); // takes the code from the file and put it in an array

void encrypt();

void decrypt();

char code[26]; // used by get_encryption() to store the code






int main(){

    srand(time(NULL));

    printf("(|1 - encrypt|)\n\n(|2 - decrypt|)\n\n");

    int answer = get_int(":> ");

    if(answer == 1){

        string input = get_string("\ntype in  the text you want to encrypt\n\n:> ");

        encrypt(input);

        printf("\n");
    } //if
    if(answer == 2){

        string input = get_string("\ntype in  the text you want to decrypt\n\n:> ");

        decrypt(input);

        printf("\n");

    } //if

    else{

        return 1;

    }//else


}






void new_encryption(){

    char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

    FILE* new_alpha = fopen("new_alphabet","w");

    int not_similar = 0;

    while(check(alphabet) != 0){

        int x = rand()%26;

        if(alphabet[x] == '0' || not_similar == x){

            continue;

        } //if

        else{

            fprintf(new_alpha,"%c",alphabet[x]);

            alphabet[x] = '0';

            not_similar++;

        } //if

    } //while

    fclose(new_alpha);
}






void get_encryption(char get_code[26]){

    char Code[26];

    FILE* get_encrypt = fopen("new_alphabet","r");

    fread(Code,1,26,get_encrypt);

    if( *Code == -103 ){

        printf("\nthe file is empty");

        check_encryption = 1;

        return;

    } //if

    fclose(get_encrypt);

    for(int i = 0;i < 26;i++){

        get_code[i] = Code[i];

    } //for

    check_encryption = 0;

}






int check(char * string){

    int checker = 0;

    for(int i = 0;i < strlen(string);i++){

        if(string[i] != '0'){

           checker++;

        } //if

    } //for

    return checker;

}






void encrypt(char* sentence){

    char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

    new_encryption();

    get_encryption(code);

    printf("\nencrypted\n\n:> ");

    for(int i = 0;i < strlen(sentence);i++){

        if(isalpha(sentence[i]) != 0){

            for(int ind = 0;ind < 26;ind++){

                if(sentence[i] == alphabet[ind]){

                    printf("%c",code[ind]);

                } //if(sentence[i] == alphabet[ind])

                if(sentence[i] == alphabet[ind] - 32){

                    printf("%c",code[ind] - 32);

                } //if(sentence[i] == alphabet[ind] - 32)

            } //small for

        } //if(isalpha(sentence[i]) != 0)

        if(isalpha(sentence[i]) == 0){

                    printf("%c",sentence[i]);

        } //if(isalpha(sentence[i]) == 0)

    } //big for

    printf("\n");

}






void decrypt(char* sentence){

    char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

    get_encryption(code);

    if(check_encryption == 1){

        return ;

    } //if(check_encryption == 1)

    printf("\ndecrypted\n\n:> ");

    for(int i = 0;i < strlen(sentence);i++){

        if(isalpha(sentence[i]) != 0){

            for(int ind = 0;ind < 26;ind++){

                if(sentence[i] == code[ind]){

                    printf("%c",alphabet[ind]);

                } //if(sentence[i] == alphabet[ind])

                if(sentence[i] == code[ind] - 32){

                    printf("%c",alphabet[ind] - 32);

                } //if(sentence[i] == code[ind] - 32)

            } //small for

        } //if(isalpha(sentence[i]) != 0)

        if(isalpha(sentence[i]) == 0){

                    printf("%c",sentence[i]);

        } //if(isalpha(sentence[i]) == 0)

    } //big for

    printf("\n");

    FILE* erase = fopen("new_alphabet","w");

    fprintf(erase,"");

    fclose(erase);

}
