#include <iostream>
#include <vector>

int main() {

    // The curly braces syntax of uniform initialization creates a bit of
    // confusion for std::vector.
    // std::vector<int>    v{10} means size-1 vector with the only element set to 10.
    // std::vector<double> v{10} means size-1 vector with the only element set to 10.
    // std::vector<char*>  v{10} means size-10 vector with uninitialized elements.
    // std::vector<int>    v(10) means size-10 vector with uninitialized elements.

    // https://en.wikipedia.org/wiki/C%2B%2B11#Uniform_initialization
    // Uniform initialization does not replace constructor syntax, which is
    // still needed at times.  If a class has an initializer list constructor
    // -- TypeName(initializer_list<SomeType>) -- then it takes priority over
    // other forms of construction, provided that the initializer list conforms
    // to the sequence constructor's type.  The C++11 version of std::vector has
    // an initializer list constructor for its template type.

    std::vector<int> v1{10};
    std::cout << v1.size() << std::endl; // prints 1

    std::vector<double> v2{10};
    std::cout << v2.size() << std::endl; // prints 1

    std::vector<char*> v3{10};
    std::cout << v3.size() << std::endl; // prints 10

    std::vector<int> v4(10);
    std::cout << v4.size() << std::endl; // prints 10

    std::vector<char*> v5 {10};
    std::cout << v5.size() << std::endl; // prints 10

    // std::vector<char*> v6 = {10};     // Error: equal sign forces initializer list constructor
    // std::cout << v6.size() << std::endl;

    std::vector<char> v7 = {10};
    std::cout << v7.size() << std::endl; // prints 1
}
