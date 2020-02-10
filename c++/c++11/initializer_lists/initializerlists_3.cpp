/*
//          Uniform Initialization
//          Uniform Initialization search order
//       1) Initialization list constructor
//       2) Regular constructor
//       3) Aggregate Initilization list
*/
#include <iostream>
#include <vector>
using namespace std;


/*
// scenario 1
// This is a aggregate class. aggregate class means class with no member functions. This works fine always.
*/
namespace scenario_1{

class myclass {
    public:
    int a;
    string b;   
    
};

}

int main1()
{
    using namespace scenario_1;
    myclass m = {10,"vijay"}; // Aggregate Initialization
    return 0;
}


namespace scenario_2 {
/*
// scenario 2
// This is not a aggregate class. since this class has parameterized constructor member functions. This fails in c++03. This is fixed from c++11.
*/
class myclass {
    public:
    int a;
    string b;
    myclass(int a, string s){}
    
};
}

int main2()
{
    using namespace scenario_2;
    myclass m = {10,"vijay"}; 
    return 0;
}

namespace scenario_3 {

class myclass {
    public:
    int a;                                                                              // search order 2
    myclass(int a){cout<<"parameterized CTOR \n";}                                    // search order 2
    myclass(const initializer_list<int> &mm){cout<<"Initialization list CTOR \n";}    // search order 1
};
}

int main()
{
    using namespace scenario_3;
    myclass m = {10};

    return 0;
}


//          Uniform Initialization search order
//       1) Initialization list constructor
//       2) Regular constructor
//       3) Aggregate Initilization list
