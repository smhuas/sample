#include <iostream>
using namespace std;

template<typename T>
constexpr auto isinteger()
{
    if constexpr(std::is_integral<T>::value)
    {
        return true;
    } else 
    {
        return false;
    }
}

int main()
{
    static_assert(isinteger<int>() == true);
    static_assert(isinteger<char>() == true);
    static_assert(isinteger<double>() == false);
}