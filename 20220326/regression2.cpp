#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <new>
#include <cstring>
#include <malloc.h>

using namespace std;

float datax[20];
float datay[20];

float n =0;

float EE(float x0, float x1, float y0, float y1) {
	return sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
}

float dis(float x, float y, float a, float b)
{
    return abs(a * x - y + b) / sqrt(a * a + b *b); 
}

float f(float a, float b)
{
    float sum = 0.0;
    for (int i = 0; i < n; i++)
    {
        sum += dis(datax[i], datay[i], a, b) / n;
    }
    return sum;
}

float dfabda(float a, float b, float da) 
{
    return (f(a + da, b) - f(a, b)) / da;
}

float dfabdb(float a, float b, float db)  
{
    return (f(a, b+db) - f(a, b)) / db;
}
 
int main(){
    
    
    	for(int i=0; i<20; i++){
		   n += i;
		  cout << datax[i] - 0.5 << datay[i] + 0.5;
		  cout << datax[i] << " " << datay[i] << " " << endl;	
	}
	   float a0 = 0, b0 = 0;
	int iteration = 0;
	float eta = 0.0001;
	float psi = 0.005;
	float da = 0.01;
	float db = 0.01;
	float a1 = 2, b1 = 0;
	while (EE(a0, b0, a1, b1) > eta && iteration < 1000000) {
		a0 = a1;
		b0 = b1;
		a1 -=   psi * dfabda(a0, b0, da);
		b1 -=   psi * dfabdb(a0, b0, db);
		iteration++;
	}
	cout << " y  = " << a1 << "x + " << b1 << endl;
	cout << iteration << "-th  E = " << EE(a0, b0, a1, b1) << endl;
	return 0;
}
