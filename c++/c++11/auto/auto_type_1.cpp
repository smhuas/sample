/*
auto :- Extremely useful for readability, especially for complicated types:
*/
#include <iostream>
#include <vector>
using namespace std;

template < typename X, typename Y >
auto add(X x, Y y) -> decltype(x+y)
{
    return x+y;
}

int main()
{
    vector <int> v = {10,7,5,13,2};
    auto cit =  v.begin();    
    for(auto i:v){cout<<i<<" ";} cout<<endl;

    cout<<"add of 1,2 is " << add(1,2)<< endl;
    cout<<"add of 1.1,2.2 is " << add(1.1,2.2)<< endl;

}