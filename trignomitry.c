#include <stdio.h>

float factorial(float number);
float power (float number , float indice);
float root (float number);
float sine(float angle);
float tangent(float angle);
float cosine(float angle);

int main(){
    float angle;
    printf("type in the angle : ");
    scanf("%f",&angle);
    printf("sin : %.5f\ncos : %.5f\ntan : %.5f\n",sine(angle),cosine(angle),tangent(angle));
}



float factorial(float number){
    if (number == 0){
        return 1;
    }
    for(int next = number - 1 ; next >= 1 ; next--){
        number *= next;
    }
    return number;
}



float power (float number , float indice){
    float result = 1;
    for(int times = 0 ; times < indice ; times++){
        result *= number;
    }
    return result;
}


#define pi 3.14159265


float sine(float angle){
    angle = pi * ((int)(angle + 360) % 360) / 180;
    float sin = 0.0;
    for(int i = 1 , x = 1; i <= 24 ; i++ , x += 2){
        float power_series = power(angle,x)/factorial(x);
        if(i % 2 == 1 || i == 1){
            sin += power_series;
        }
        if(i % 2 == 0){
            sin -= power_series;
        }
    }
    return sin;
}



float cosine(float angle){
    return  root(1.0 - power(sine(angle),2));
}



float tangent(float angle){
    return sine(angle)/cosine(angle);
}



float root (float number){
    if(number == 1 || number == 0){
        return number;
    }
    float iteration = (number + 1)/2;
    for(int i = 0 ; i <= 10 ; i++){
        iteration = (iteration + (number/iteration))/2;
    }
    return iteration;
}
