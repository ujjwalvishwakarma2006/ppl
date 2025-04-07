#include <iostream>
using namespace std;

// A simple function
int add(int a, int b){
	return a + b;
}

int sub(int a, int b){
	return a - b;
}

int multiply(int a, int b){
	return a*b;
}

int main() {
	int (*fptr)(int, int){};
	int a = 10;
	int b = 5;
	
	fptr = &add;
	cout << fptr(a, b) << endl;
	fptr = &sub;
	cout << fptr(a, b) << endl;
	fptr = &multiply;
	cout << fptr(a, b) << endl; 
    return 0;
}
