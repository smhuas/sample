/*
    Initializer lists
*/

#include <iostream>
#include <vector>
using namespace std;


int main()
{
    int arr[4] = {1,2,3,4};     // 

    vector<int> v = {1,2,3,4};  // c++03 this fails. Initialization lists for vector got introduced from c++11
                                // c++11 make sure Initializationlist constructor is called .
                                // c++11 enables this feature for all the stl's
    return 0;
}