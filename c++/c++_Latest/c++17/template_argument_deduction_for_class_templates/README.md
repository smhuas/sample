if compile the same code in c++11/14, it wont compile, 
compiler expects MyContainer<float> or MyContainer<int>. we need to mention template argument like this <float>.

However in c++17 compiler deduces the template argument for you. Hence no need to give <float> or <int> anymore.