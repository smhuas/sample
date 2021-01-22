#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>
using namespace std;
/*
The class template std::variant represents a type-safe union. 
An instance of std::variant at any given time holds a value of one of its alternative types (it's also possible for it to be valueless).

Empty variants are also ill-formed (std::variant<std::monostate> can be used instead).
https://www.bfilipek.com/2018/06/variant.html
*/
// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... 
{ 
    using Ts::operator()...; 
};
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct SampleVisitor
{
    void operator()(int i) const { 
        std::cout << "int: " << i << "\n"; 
    }
    void operator()(float f) const { 
        std::cout << "float: " << f << "\n"; 
    }
    void operator()(const std::string& s) const { 
        std::cout << "string: " << s << "\n"; 
    }
};

int main()
{
    std::variant<int,double> v{12}; 
    std::get<int>(v); // == 12
    std::get<0>(v);   // == 12
    v = 12.2;
    std::get<double>(v); // == 12.2
    std::get<1>(v); // == 12.2

    cout<< std::get<1>(v)<<endl;
    cout<<"index is " <<v.index() <<endl;

    variant<int, float, string> intFloatString{"vijay"};
    std::visit(SampleVisitor{},intFloatString);


    std::visit(overloaded {
            [](auto arg) { std::cout << arg << ' '; },
            [](float arg) { std::cout << std::fixed << arg << ' '; },
            [](const std::string& arg) { std::cout << std::quoted(arg) << ' '; },
        }, intFloatString);
   
}