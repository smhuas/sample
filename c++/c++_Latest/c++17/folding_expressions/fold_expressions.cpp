#include <iostream>
#include <vector>
using namespace std;

template<typename ...T>
auto add(T... args)
{
    return (... + args); // unary left fold
}

template<typename ...T>
auto mul(T... args)
{
    return (args * ...); // unary right fold
}

template<typename ...T>
auto appendstringinfront(T... args) -> string
{
    return ("hello" + ... + args); // binary left fold
}

template<typename ...T>
auto appendstringinend(T... args)
{
    return (args + ... + "hello"); // binary right fold
}

template<typename ...T>
auto AND(T... args) {return (... && args); }

template<typename ...T>
auto OR(T... args) {return (... || args); }

template < typename T,  typename ...Targs>
void push_back_vec(vector<T> &vec, Targs... args)
{
    (vec.push_back(args), ...); // unary fold with comma operator
}

int main()
{

cout<< add(1,2,3,4) << endl;
cout<< mul(1,2,3,4) << endl;

cout<< appendstringinfront<string>("world") << endl;
#if 1
cout<< appendstringinend<string>("world") << endl;
cout<< boolalpha << AND(true,true,true,true,false) <<endl;
cout<< boolalpha << AND(true,true,true,true,true) <<endl;
    
    if(int x = 89; AND( (x > 0), (x > 50) , (x > 100)  ))
    {
        cout<< "TRUE " << endl;
    } else {
        cout<< "FALSE " << endl;
    }

vector<int> v;
push_back_vec(v,1,2,3,4,5,6,7,8,9,10);
for(auto i:v){cout<<i<<" ";} cout<<endl;
#endif


}