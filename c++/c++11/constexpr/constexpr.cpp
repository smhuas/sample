/*
constexpr:-     values are those that the compiler can evaluate at compile-time
                Only non-complex computations can be carried out in a constant expression
                Use the constexpr specifier to indicate the variable, function, etc. is a constant expression.
                http://www.vishalchovatiya.com/when-to-use-const-vs-constexpr-in-cpp/
*/

#include <iostream>
using namespace std;

    constexpr int add(int a, int b)
    {
        return a+b;
    }

    const int sub(int a, int b)
    {
        return a-b;
    }

int main()
{
    constexpr int a = 10;
    constexpr int b = a; 

    const int c = 10;
    constexpr int d = c;

    const int e = sub(10,5);

    // constexpr int f = sub(10,5);  // this is not possible
    
    cout<< b <<endl;
    
    cout<< add(2,3) <<endl;

    
}