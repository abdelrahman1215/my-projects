#include <stdio.h>

int main(){

    int user_input;

    printf("type in a number less than 256\n\n   :> ");

    scanf("%i",&user_input);

    printf("\n");

    for(int binary = 128 ; binary >= 1 ; binary /= 2){

        if(user_input > 255){

            printf("this number isn't less than 256\n");
            break;

        }

        if(user_input >= binary){

            printf("1");

            user_input %= binary;

            continue;
        }

        printf("0");

    }

    printf("\n");

}
