/*
    decltype is an operator which returns the declared type of an expression passed to it.    
    cv-qualifiers and references are maintained if they are part of the expression. 
*/
#include <iostream>
using namespace std;

template < typename T >
auto sum(T a, T b) -> decltype(a+b) // here decltype returns the type of (a + b) expression with cv qualifiers 
{
    return (a+b);
}


int main()
{
    int a = 10;
    decltype(a) b = 20;     
    cout << b <<endl;                                
    
    cout<< sum(1,2) << endl;
    
}