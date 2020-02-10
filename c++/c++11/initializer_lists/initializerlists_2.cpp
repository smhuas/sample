/*
//          initialization list constructor from c++11
//          std::intializer_list<> is introduced from c++11
*/
#include <iostream>
#include <vector>
using namespace std;

class myvector {
    public:
    vector<int> vec;

    myvector(const initializer_list<int> &vv )
    {
        for(initializer_list<int>::iterator iter = vv.begin();iter != vv.end(); ++iter)
        {
            vec.push_back(*iter);
        }
    }
};

int main()
{

    myvector v = {0,1,2,3,4};
    myvector c {0,1,2,3,4}; // both are same 
    return 0;
}