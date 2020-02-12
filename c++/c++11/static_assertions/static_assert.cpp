/*
     static_assert() introduced from c++11 for compile time asserts
*/

#include <iostream>
#include <cassert> // required for runtime assert
using namespace std;




int main()
{
    const int size = 3; // assert fails if size is non-constant, since it cannot resolve size at compile time
    static_assert(size > 4,"size < 4"); 

    return 0;
}