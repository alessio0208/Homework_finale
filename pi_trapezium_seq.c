#include <stdio.h>
#include <stdlib.h>

double f(double x){
    double t = 4.0/(1+x*x);

    return t;
}


double PI(int n){
    double sum = 0;
    for (int i=0; i<n; i++){
        sum+= f(1.0*i/n) + f(1.0*(i+1)/n);
    }
    double pi = 1.0/(2*n)*sum;
    return pi;
}

int main(int argc, char*argv[]){

    int n=10000;
    printf("Test n= %d \n",n);
    printf("PI = %f \n",PI(n));
    return 0;
}
