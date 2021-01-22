#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

struct Bar {
    typedef double internalType;  
};

template <typename T> 
typename T::internalType foo(const T& t) { 
    cout << "foo<T>\n"; 
    return 0; 
}

int foo(int i) { cout << "foo(int)\n"; return 0; }

int main()
{
    foo(Bar());
    foo(0);
}