#include<iostream>
#include<type_traits>
#include <utility>
using namespace std;


template <auto... seq>
struct my_integer_sequence {};


            
template<auto... seq>
void print_sequence( my_integer_sequence<seq...> int_seq){
    ((cout<<seq<<" "), ...); cout<<endl;
}



int main()
{
   
    print_sequence( my_integer_sequence<1,2,3,4>{} );
    

    

}