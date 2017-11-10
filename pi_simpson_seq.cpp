#include<string>
#include<iostream>
#include <stdlib.h>
using namespace std;

double f(double x){
    double t = 4.0/(1+x*x);
    return t;
}
double PI(int m){
    double sum = f(0.0)+ f(1.0);
    std::cout<<"f)0) = "<< f(0.0) <<std::endl;
    for (int i=0; i<m; i++){
        sum+= 2*f(1.0*i/m);
    }
    for (int i=0; i<m+1; i++){
        sum+= 4*f(1.0*(2*i-1)/(2*m));
    }
    double pi = 1.0/(6*m)*sum;
    return pi;
}

int main(int argc, char*argv[]){
    int m = strtol(argv[1], NULL, 10);
    std::cout<<"m = "<< m <<std::endl;
    std::cout<<"PI = "<< PI(m) <<std::endl;
    return 0;
}
