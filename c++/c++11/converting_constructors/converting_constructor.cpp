/*
     converting constructors    
     https://en.cppreference.com/w/cpp/language/converting_constructor
     usually when a value is assigned to a class object, we can do using below methods
     1) A a(1);
     2) A a = 1; // implicit conversion
     3) A a {1};
     4) A a = {1}; // implicit conversion

     Use explicit keyword in the constructor declaration to avoid these kinds of implicit conversion.
*/

#include <iostream>
using namespace std;

class A {
public:
    A(){}
    A(int a) {cout <<"single parameter "<< endl;}
    A(const A& obj) {cout << "copy ctor "<< endl;}
    A operator=(A& a){cout<< "assignment operator "<<endl;}
    A operator=(int a){cout<< "assignment operator "<<endl;}
    A(int a, int b) {}
    A(int a, int b, int c) {}
    //A (initializer_list<int> a){cout << "initialization list "<< endl;}
};

class B {
public:
    explicit B(){}
    explicit B(int a) {}
    explicit B(int a, int b) {}
    explicit B(int a, int b, int c) {}
};


int main()
{
    A aa (1); 
    A ab = 1;
    A ac {1};
    A ad = {1};

    //B d = 1; // nok
    //B e = {1}; // nok
    B f (1) ;
    
}