#include <iostream>

void test_native_arrays();
void test_native_2D_arrays_and_arrays_of_pointers();
void test_list_vector_iota_shuffle();

int main() {

    test_native_arrays();
    test_native_2D_arrays_and_arrays_of_pointers();
    test_list_vector_iota_shuffle();

    return 0;
}

void print_array(int array[], size_t size);

void test_native_arrays() {

    // int array1[] -- Error: array needs an explicit size or an initializer
    int array2[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int array3[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };  // C++11 uniform initialization
    int array4[10];       // Same as in the scalar case, uninitialized automatic variables
                          // have undefined i.e. garbage values.  Says K&R book.
    int array5[10] = { }; // K&R book also says, if too few initializers, missing elements
                          // will be zero for external, static, and automatic variables.

    // range for loop works for native array here, because the size is known.
    std::cout << "array2 = { ";
    for (auto x : array2) {
        std::cout << x << " ";
    }
    std::cout << "}" << std::endl;

    std::cout << "array3 = ";
    print_array(array3, 10);
    std::cout << std::endl;

    // array4 is uninitialized;  its values will be arbitrary, garbage, possibly
    // non-zero, and different for every run.
    std::cout << "array4 = ";
    print_array(array4, 10);
    std::cout << std::endl;

    // array5 is initialized, but with too few initializers.
    // according to K&R, missing values will be filled with zeros.
    std::cout << "array5 = ";
    print_array(array5, 10);
    std::cout << std::endl;

    // int vs. size_t
    // on 64-bit MacBook Pro, these print 1, 2, 4, 8, 8.
    // size_t is 8 bytes, same as long, i.e. longer than int which is 4 bytes.
    std::cout << "sizeof(char)   = " << sizeof(char) << std::endl;
    std::cout << "sizeof(short)  = " << sizeof(short) << std::endl;
    std::cout << "sizeof(int)    = " << sizeof(int) << std::endl;
    std::cout << "sizeof(long)   = " << sizeof(long) << std::endl;
    std::cout << "sizeof(size_t) = " << sizeof(size_t) << std::endl;

    std::cout << std::endl;
}

#include <cstring>

// These nuances are best explained in K&R book.
void test_native_2D_arrays_and_arrays_of_pointers() {

    int matrixa[2][10];  // 20 spaces allocated for 2D matrix of 2 rows, 10 columns

    // int matrixb[][10] -- Error: array needs an explicit size or initializer.
                         // But this is how a 2D array function parameter is declared.

    int matrixb[][10] { }; // now initialized (as size-0):
                           // array of size-10 arrays, or really a pointer to size-10 array

    int (*matrixc)[10];    // As you know any `array[]` is really `*array`, this looks
                           // bizarre but is equivalent to `matrix[][10]`.
                           // array of size-10 arrays is same as pointer to size-10 array.

    int *matrixd[10];    // Parantheses matter.  This is size-10 array of pointers to int.
    int* matrixe[10];    // size-10 array of pointers to int.
                         // K&R always puts `*` closer to the name.  There are solid
                         // syntactic reasonings for it, but like this case here,
                         // it may be clearer to put `*` closer to `int` here.

    // matrixa points to allocated memory of 20 (element is int).
    // matrixb is a size-0 array; we couldn't declare it without initializing.
    //   if matrixb were parameter to this function, it could have been left
    //   uninitialized, and then it would have been just a pointer, pointing to
    //   no allocated memory, pointing to garbage.
    // matrixc is an uninitialized pointer, pointing to garbage.
    // matrixd points to allocated memory of 10 (element is int*).
    // matrixe is same as matrixd, points to allocated memory of 10 (element is int*).

    // Use the above variables to get rid of unused variable warnings.
    matrixc = matrixa;
    matrixc = matrixb;
    matrixd[0] = matrixa[0];
    matrixe[0] = matrixc[0];

    // K&R illustrates the differece between array[][10] (*array)[10] and *array[]:
    // the latter can have "rows" of different lengths, whereas
    // the former fixes every row to be of length 10.
    // So to avoid wasted memory, array of strings should be
    //   char *array[],
    // instead of
    //   char array[][MAX_STRING_LENGTH]
    //   char (*array)[MAX_STRING_LENGTH]

    // C++11 does not allow converting string literal/constant to char *.
    // const char * is okay.
    const char *monthsa[] = { "January", "February", "March", "April" };
    const char monthsb[][9] = { "January", "February", "March", "April" };
    const char (*monthsc)[9] = monthsb;

    // Use the above variables to get rid of unused variable warnings.
    monthsa[0] = monthsc[1];
    std::cout << strcmp(monthsa[0], monthsb[0]) << std::endl;
    std::cout << std::endl;
}

// Error: range for loop doesn't compile for native array as function parameter.
// Function parameter with array type "T []" is treated as pointer type "T *".
// The size cannot be deduced for such array at compile time.
#if 0
void print_array(int array[]) {
    for (auto x : array) {
        std::cout << x << " ";
    }
}
#else
void print_array(int array[], size_t size) {
    std::cout << "{ ";
    for (size_t i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << "}";
}
#endif

// -----------------------------------------------------------------------------
// Example taken from http://en.cppreference.com/w/cpp/algorithm/iota
//
// Illustrates std::iota, which fills a range with value, ++value, ++value etc.
// Also illustrates
// - std::list
// - std::vector that is filled with iterators of the std::list
// - std::shuffle that permutes the elements a random-access range such as
//   the one for std::vector (but not std::list).
// Basically:
// Constructing std::vector<std::list<T>::iterator> is a way to shuffle list.
// And std::iota comes handy in initializing that vector.

template <class T, class V>
void my_iota1(T begin, T end, V value) {
    for (auto ii = begin; ii != end; ii++) {
        *ii = value;
        ++value;  // use of prefix ++ is deliberate; that's in the spec.
    }
}

template <class ForwardIterator, class V>
void my_iota2(ForwardIterator begin, ForwardIterator end, V value) {
    while (begin != end) {
        *begin++ = value;
        ++value;  // use of prefix ++ is deliberate; that's in the spec.
    }
}

#include <list>       // list
#include <numeric>    // iota
#include <vector>     // vector
#include <random>     // random_device, mt19937
#include <algorithm>  // shuffle
                      // In Apple LLVM Clang, algorithm is already included
                      // in vector, list, random.  But not so in GCC 7.

template <class T> void print_range(T begin, T end);

template <class ForwardIterator, class ResultType>
void map_and_print_range(
    ForwardIterator begin,
    ForwardIterator end,
    ResultType mapfunction(ForwardIterator));

void test_list_vector_iota_shuffle() {

    std::list<int> mylist(10);
    std::iota(mylist.begin(), mylist.end(), 100);

    std::cout << "mylist   = ";
    print_range(mylist.begin(), mylist.end());
    std::cout << std::endl;

    std::vector<std::list<int>::iterator> mylist_iterators(10);
    std::iota(mylist_iterators.begin(), mylist_iterators.end(), mylist.begin());

    // let's shuffle the iterators

    // shuffle: http://en.cppreference.com/w/cpp/algorithm/shuffle
    // redirects to random_shuffle: http://en.cppreference.com/w/cpp/algorithm/random_shuffle
    // But random_shuffle variants are obsolete.  Use shuffle.

    // One liner, with C++11 uniform initialization syntax.

    std::shuffle(mylist_iterators.begin(), mylist_iterators.end(), std::mt19937{std::random_device{}()});
    std::cout << "shuffle1 = { ";
    for (auto ii : mylist_iterators) {
        std::cout << *ii << " ";
    }
    std::cout << "}" << std::endl;

    // One liner, with constructor syntax.

    std::shuffle(mylist_iterators.begin(), mylist_iterators.end(), std::mt19937(std::random_device()()));
    std::cout << "shuffle2 = { ";
    for (auto ii : mylist_iterators) {
        std::cout << *ii << " ";
    }
    std::cout << "}" << std::endl;

    // Let's demystify what is happening at that third argument to shuffle.

    // random_device: non-deterministic but low-quality random number generator.
    // http://en.cppreference.com/w/cpp/numeric/random/random_device

    // mt19937: high-quality but deterministic pseudo random number generator.
    // It is a specific instance of std::mersenne_twister_engine<...> adhering
    // to 32-bit Mersenne Twister by Matsumoto and Nishimura, 1998.
    // http://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine

    // So, to have a high-quality, non-deterministic (i.e. randomly seeded)
    // random number generator, use mt19937 that is seeded with a random value
    // from random_device.

    // re-initialize the vector of iterators
    std::iota(mylist_iterators.begin(), mylist_iterators.end(), mylist.begin());

    std::random_device nondet_rand_gen;
    auto seed = nondet_rand_gen();
    std::mt19937 mt_rand_gen1(seed);
    std::shuffle(mylist_iterators.begin(), mylist_iterators.end(), mt_rand_gen1);
    std::cout << "shuffle3 = { ";
    for (auto ii : mylist_iterators) {
        std::cout << *ii << " ";
    }
    std::cout << "}" << std::endl;

    // re-initialize the vector of iterators
    std::iota(mylist_iterators.begin(), mylist_iterators.end(), mylist.begin());

    // another round, but with same seed for mt19937; gives identical shuffle.
    std::mt19937 mt_rand_gen2(seed);
    std::shuffle(mylist_iterators.begin(), mylist_iterators.end(), mt_rand_gen2);
    std::cout << "shuffle4 = { ";
    for (auto ii = mylist_iterators.begin(); ii != mylist_iterators.end(); ii++) {
        std::cout << *(*ii) << " ";
    }
    std::cout << "}" << std::endl;

    // Try the map_and_print_range template function.  But in its current,
    // simplest definition, compiler cannot automatically deduce the types
    // and do the matching to synthesize a correct instance of the template.
    // So, we have to explicitly specify the template types, thus negating the
    // idea of brevity of calling a function.
    std::iota(mylist_iterators.begin(), mylist_iterators.end(), mylist.begin());
    std::shuffle(mylist_iterators.begin(), mylist_iterators.end(), std::mt19937{std::random_device{}()});
    std::cout << "shuffle5 = ";
    // map_and_print_range(mylist_iterators.begin(), mylist_iterators.end(),
    //     [](std::vector<std::list<int>::iterator>::iterator ii) { return *(*ii); });
    map_and_print_range<std::vector<std::list<int>::iterator>::iterator, int>(
        mylist_iterators.begin(),
        mylist_iterators.end(),
        [](std::vector<std::list<int>::iterator>::iterator ii) { return *(*ii); });
    std::cout << std::endl;

    // `auto` works in the lambda declaration.
    std::cout << "shuffle5 = ";
    map_and_print_range<std::vector<std::list<int>::iterator>::iterator, int>(
        mylist_iterators.begin(), mylist_iterators.end(), [](auto ii) { return *(*ii); });
    std::cout << std::endl;
}

template <class T> void print_range(T begin, T end) {
    std::cout << "{ ";
    for (auto ii = begin; ii != end; ii++) {
        std::cout << *ii << " ";
    }
    std::cout << "}";
}

template <class ForwardIterator, class ResultType>
void map_and_print_range(
    ForwardIterator begin,
    ForwardIterator end,
    ResultType mapfunction(ForwardIterator)) {
    std::cout << "{ ";
    for (auto ii = begin; ii != end; ii++) {
        std::cout << mapfunction(ii) << " ";
    }
    std::cout << "}";
}
