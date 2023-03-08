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
    if (number == 0 || number == 1){
        return 1;
    }
    for(int next = number - 1 ; next >= 1 ; next--){
        number *= next;
    }
    return number;
}



float power (float number , float indice){
    if(indice == 0){
        return 1;
    }
    float tmp = number;
    for(int times = 1 ; times < indice ; times++){
        number *= tmp;
    }
    return number;
}



float sine(float angle){
    if(angle > 360){
        int x = angle/360;
        float y = angle/360;
        angle = (y - x) * 360;
    }
    angle = 3.14159265 * (angle / 180.0);
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
    float cos = root(1.0 - power(sine(angle),2));
    return cos;
}



float tangent(float angle){
    float tan = sine(angle)/cosine(angle);
    return tan;
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