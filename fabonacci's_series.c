#include <stdio.h>

int main(){
    int start;

    printf("start? : ");

    scanf_s("%i",&start);

    int end;

    printf("end? : ");

    scanf_s("%i",&end);

    int temp;

    for(int i = 0;start <= end;i = temp){

        temp = start;

        printf("%i\n",start);

        start += i;

    }

    return 0;
}
