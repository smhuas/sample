#include <iostream>
#include <map>
using namespace std;

/*
A proposal for de-structuring initialization, that would allow writing auto [ x, y, z ] = expr; 
where the type of expr was a tuple-like object, whose elements would be bound to the variables x, y, and z (which this construct declares). 
Tuple-like objects include std::tuple, std::pair, std::array, and aggregate structures.
*/


using Coordinate = std::pair<int, int>;
Coordinate origin() {
  return Coordinate{1, 2};
}

int main()
{
    const auto [ x, y ] = origin();
    cout<< x << y << endl;

    map<int,string> mp = {{1,"vijay"}, {2,"bhaskar"}, {3,"reddy"}};
    for(auto [x,y]:mp)
    {
        cout<<x << " --> "<<y<<endl;
    }
}