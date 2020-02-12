/*
//  override keyword available from c++11 onwards
*/
#include <iostream>
using namespace std; 

class base {
	public:
		virtual void A(int){
		}
		virtual void B() const{}
};

class derived : public base {
	public:
		virtual void A(float) override{} // compilation fails
		
		virtual void B()  override{} // compilation fails
		
		void myfunc() override {} // compilation fails
};

int main() 
{ 	
	base *b = new derived; 
	return 0; 
} 