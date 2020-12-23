/*
[[noreturn]]    using this attribute, the void function doesn't return to main function.
        
                below func doesnot return to main function. so, cout in the end of main doesn't execute at all.

                How ever if any return statement is present in void function, compiler throws warning also.

                The following standard functions have this attribute: std::_Exit, std::abort, std::exit, std::quick_exit, 
                std::unexpected, std::terminate, std::rethrow_exception, std::throw_with_nested, std::nested_exception::rethrow_nested
*/
#include <iostream>
#include <exception>
#include  <stdexcept>
using namespace std;


[[noreturn]] void func()
{
    cout<<"in func() "<<endl;
    return ;
}

int main()
{
    
    func(); // this function never returns to main function
    cout<<"main ends here \n"; // this doesn't execute only
}