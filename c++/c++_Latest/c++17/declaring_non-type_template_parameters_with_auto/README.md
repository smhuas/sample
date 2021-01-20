the type of the non-type template arguements like integer_seq<1,2,3,4>{} can be deduced automatically, no need to give the type explicitly.

ex:- std::integer_sequence<int,1,2,3,4>();  // before c++17
     my_integer_sequence<1,2,3,4>();           // c++17