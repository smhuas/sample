/*
default keyword in class
*/
#include <iostream>
using namespace std;

class A {
public:
    A(){}
    A(int a){cout<< "param ctor "<<endl;}
    
};

int main()
{
    A a;
    A b(2);
   
}