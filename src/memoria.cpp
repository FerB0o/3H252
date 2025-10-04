#include <iostream>

using namespace std;

class Empleado{
private://Relacon tiene
    char nombre [10];
    int edad;
    char direccion[255];
    int telefono;
public: //Relacon usa
    int LeerEdad(){
        return edad;
    }
}

int main(){

//Enteros
cout<<"Tamano de int " << sizeof(int) << endl;
cout<<"Tamano de long " << sizeof(long) << endl;

//Punto flotante
cout<<"Tamano de float " << sizeof(float) << endl;
cout<<"Tamano de double " << sizeof(double) << endl;

//Bytes
cout<<"Tamano de char " << sizeof(char) << endl;
cout<<"Tamano de byte " << sizeof(byte) << endl;
cout<<"Tamano de bool " << sizeof(bool) << endl;

cout<<"Tamano de puntero " << sizeof(int *) << endl;

Empleado empleados[10];//
cout<<"Tamano de Empleado" << sizeof(Empleado) <<endl;
//cout<<"Empleados" << empleados <<endl;
cout<<"Empleados" << empleados [0].LeerEdad() <<endl;

int a= 64;
char b = (char)a;
cout<<b<<endl;




return 0;

}