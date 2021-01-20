#include <iostream>
using namespace std;

string gr(int a, int b)
{
    if(int tmp = a; tmp>b)
    {   
        return "greater";
    } else
    {
        return "not greater";
    }
}

int main()
{

    cout<<gr(2,3)<<endl;

}