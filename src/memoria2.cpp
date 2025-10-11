#include <iostream>  

using namespace std;

int main()
{
    cout<<"Tamano de int " << sizeof(int) << endl;
    cout<<"Tamano de long " << sizeof(long) << endl;

//Punto flotante
    cout<<"Tamano de float " << sizeof(float) << endl;
    cout<<"Tamano de double " << sizeof(double) << endl;

//Bytes
    cout<<"Tamano de char " << sizeof(char) << endl;
    cout<<"Tamano de byte " << sizeof(byte) << endl;
    cout<<"Tamano de bool " << sizeof(bool) << endl;

    cout<<"Tamano de puntero " << sizeof(int*) << endl;

//punteros en C y c++
int* dirA = (int*)malloc(sizeof(int));//C
int* dirB = new int;

cout << "dirA:"<< *dirA <<endl;
cout << *dirB <<endl;

    return 0;
}


