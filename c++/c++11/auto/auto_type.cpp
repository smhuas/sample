/*
//          auto type   
*/

#include <iostream>
#include <vector>
using namespace std;


int main()
{
    auto a = 10;
    auto b = 12.34;
    auto str = string("vijay");

    cout<<"a type is "<<typeid(a).name()<<endl;
    cout<<"a type is "<<typeid(b).name()<<endl;
    cout<<"a type is "<<typeid(str).name()<<endl;

    vector<int> v = {1,2,3,4,5,6,7,8,9,10};

    for(vector<int>::iterator iter = v.begin();iter != v.end();++iter)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;


    for(auto iter=v.begin(): v)
    {
        cout<<*iter<<" "
    }
    cout<<endl;

    return 0;
}