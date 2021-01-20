#include <iostream>
#include <type_traits>
using namespace std;

template <typename T = float>
struct MyContainer {
  T val;
  MyContainer() : val{} {cout<<"type of val is "<<typeid(val).name() << endl;}
  MyContainer(T val) : val{val} {cout<<"type of val is "<<typeid(val).name() << endl;}
  
};

int main()
{
/* compiler expects MyContainer<float> or MyContainer<int>. we need to mention template argument like this <float>. */
MyContainer<int> c1 {1}; // OK MyContainer<int> 
MyContainer<float> c2; // OK MyContainer<float>

/* However in c++17 compiler deduces the template argument for you. Hence no need to give <float> or <int> anymore. */
MyContainer c3 {1}; // OK MyContainer<int>
MyContainer c4; // OK MyContainer<float>


}