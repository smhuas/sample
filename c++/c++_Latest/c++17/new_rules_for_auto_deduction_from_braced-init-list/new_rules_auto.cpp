#include <iostream>
using namespace std;

/*
Changes to auto deduction when used with the uniform initialization syntax. Previously, auto x {3}; 
deduces a std::initializer_list<int>, which now deduces to int.
*/

int main()
{
    auto x1 {1, 2, 3}; // error: not a single element
    auto x2 = {1, 2, 3}; // x2 is std::initializer_list<int>
    auto x3 {3}; // x3 is int
    auto x4 {3.0}; // x4 is double
}