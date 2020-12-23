     converting constructors    
     https://en.cppreference.com/w/cpp/language/converting_constructor
     usually when a value is assigned to a class object, we can do using below methods
     1) A a(1);
     2) A a = 1; // implicit conversion
     3) A a {1};
     4) A a = {1}; // implicit conversion

     Use explicit keyword in the constructor declaration to avoid these kinds of implicit conversion.