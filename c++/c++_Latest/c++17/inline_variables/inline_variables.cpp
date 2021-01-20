#include <iostream>
using namespace std;

/*
   https://www.codingame.com/playgrounds/2205/7-features-of-c17-that-will-simplify-your-code/inline-variables
   struct hello{
    static              int val1 = 20; // error
    static constexpr    int  val2 = 20; // OK
    static inline       int  val2 = 20; // OK
}; 
*/

struct hello{
    static inline int val = 20;
};

int main()
{

}