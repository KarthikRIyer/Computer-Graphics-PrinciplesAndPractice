#include<iostream>
#include<random>
#include<math.h>

std::random_device rd;
std::mt19937 eng(rd());


double f1(double x){
	return 4;
}

double f2(double x){
	return x*x;
}

double f3(double x){
	return sin(M_PI*x);
}

double integrate(double (*f)(double), double a, double b){
	double result = 0.0f;
	std::uniform_real_distribution<double> distr(a,b);
	for(int i = 0;i<10000;i++){
	result = result*((i)/(i+1.0f)) + (((*f)(distr(eng)))*(b-a)/(i+1));
	}
	return result;
}

int main(){
	double result = 0.0f;
	double r1 = 0.0f;
	double (*f_ptr)(double) = &f1;
	std::cout<<"Integral = "<<integrate(f_ptr,0.0f,2.0f*M_PI)<<std::endl;
}