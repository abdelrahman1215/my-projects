#include <stdio.h>

int main(){
    int start;

    printf("start? : ");

    scanf("%i",&start);

    int end;

    printf("end? : ");

    scanf("%i",&end);

    int temp;

    FILE* file = fopen("fibonacci series.txt","w");

    for(int i = 0;start <= end;i = temp){

        temp = start;

        fprintf(file,"%.i\n",start);

        start += i;

    }

}
