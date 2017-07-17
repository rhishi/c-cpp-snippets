/*
  Defines Swap function in various ways.
  - array and two indices
  - two pointers
  - two references
  - two references & using move semantics
  And two incorrect or useless ways.
  - two arguments passed by value
  - two rvalue references
*/

#include <iostream>
#include <vector>
#include <string>
#include <utility>   // move
#include <chrono>    // stead_clock, duration, duration_cast

using namespace std;
using namespace std::chrono;

void Swap(int array[], int i, int j);
template <typename T> void Swap(T array[], size_t i, size_t j);
void SwapIntegersByValue(int x, int y);
void Swap(int* x, int* y);
void Swap(int& x, int& y);
void SwapRValueReferences(int&& x, int&& y);
template <typename T> void Swap(T& x, T& y);
template <typename T> void SwapQuiet(T& x, T& y);
template <typename T> void SwapQuietUsingMove(T& x, T& y);

int main() {
    cout << "Hello, World!" << endl;

    // Create integer vector, and reverse it using Swap(int&, int&)
    vector<int> intvec;
    intvec.push_back(1);
    intvec.push_back(2);
    intvec.push_back(3);
    intvec.push_back(4);

    const int size = intvec.size();
    for (int i = 0; i < size / 2; i++) {
        Swap(intvec[i], intvec[size - 1 - i]);
    }
    for (int x : intvec) {
        cout << x << " ";
    }
    cout << endl;

    // Get the bare array inside the integer vector, and reverse it using Swap(int[], int, int)
    int* array = intvec.data();

    for (int i = 0; i < size / 2; i++) {
        Swap(array, i, size - 1 - i);
    }
    for (int x : intvec) {
        cout << x << " ";
    }
    cout << endl;

    // reverse the array using Swap(int*, int*)
    for (int i = 0; i < size / 2; i++) {
        Swap(array + i, array + size - 1 - i);
    }
    for (int x : intvec) {
        cout << x << " ";
    }
    cout << endl;

    // Create string vector, and reverse it using Swap(T&, T&)
    vector<string> strvec { "a", "b", "c", "d", "e" };

    const int strvecsize = strvec.size();
    for (int i = 0; i < strvecsize / 2; i++) {
        Swap(strvec[i], strvec[strvecsize - 1 - i]);
    }
    for (auto x : strvec) {
        cout << x << " ";
    }
    cout << endl;

    // Get the bare array inside the string vector, and reverse it using Swap(T[], size_t, size_t)
    string* strarray = strvec.data();

    for (int i = 0; i < size / 2; i++) {
        Swap(strarray, i, strvecsize - 1 - i);
    }
    for (auto x : strvec) {
        cout << x << " ";
    }
    cout << endl;
    cout << endl;

    // Swap two integer variables using Swap(int&, int&)
    int x = 5;
    int y = 6;
    Swap(x, y);
    cout << x << " " << y << endl;

    // When function takes in a reference, it expects an l-value, not an r-value.
    // Hence the following line errors out.  Quoting Apple LLVM Clang:
    //  1. candidate functions (int&, int&) and (T&, T&) with T = int are not
    //     viable, because they expect an l-value for 1st argument.
    //  2. candidate function (int*, int*) is not viable, because no known
    //     conversion from int to int* for 1st argument.
    //  3. candidate functions that require 3 arguments but 2 were provided.

    // Swap(x + y, y);  //-- Error: expects an l-value not an r-value for 1st argument

    // Of course one could define Swap(int, int) or Swap(int&&, int&&), but they
    // are of little use.
    SwapIntegersByValue(x + y, y);
    SwapIntegersByValue(x + y, x - y);
    SwapIntegersByValue(x, x - y);
    SwapIntegersByValue(x, y);
    // SwapRValueReferences(x + y, y);  //-- Error: no known conversion from int to int&& for 2nd argument
    SwapRValueReferences(x + y, x - y);
    // SwapRValueReferences(x, y);      //-- Error: no known conversion from int to in&& for 1st argument
    cout << x << " " << y << endl;
    cout << endl;

    // Test SwapQuiet and SwapQuietUsingMove on small vector of small strings
    vector<string> smallstrvec { "aa", "bb", "cc", "dd" };
    const int smallstrvecsize = smallstrvec.size();
    for (int i = 0; i < smallstrvecsize / 2; i++) {
        SwapQuiet(smallstrvec[i], smallstrvec[smallstrvecsize - 1 - i]);
    }
    for (auto x : smallstrvec) {
        cout << x << " ";
    }
    cout << endl;

    for (int i = 0; i < smallstrvecsize / 2; i++) {
        SwapQuietUsingMove(smallstrvec[i], smallstrvec[smallstrvecsize - 1 - i]);
    }
    for (auto x : smallstrvec) {
        cout << x << " ";
    }
    cout << endl;
    cout << endl;

    // Test the performance of SwapQuiet and SwapQuietUsingMove using very long strings
    auto starta = steady_clock::now();
    vector<string> bigstrvec(1000);
    for (string& x : bigstrvec) {
        x = string(10000000, 'a');
    }
    auto enda = steady_clock::now();
    cout << "Allocated " << bigstrvec.size() << " x " << bigstrvec[0].size()
         << " in " << duration_cast<duration<double>>(enda - starta).count()
         << " seconds" << endl;

    const int bigstrvecsize = bigstrvec.size();

    auto startsq = steady_clock::now();
    for (int i = 0; i < bigstrvecsize / 2; i++) {
        SwapQuiet(bigstrvec[i], bigstrvec[bigstrvecsize - 1 - i]);
    }
    auto endsq = steady_clock::now();
    cout << "Swapped Quietly"
         << " in " << duration_cast<duration<double>>(endsq - startsq).count()
         << " seconds" << endl;

    auto startsqum = steady_clock::now();
    for (int i = 0; i < bigstrvecsize / 2; i++) {
        SwapQuietUsingMove(bigstrvec[i], bigstrvec[bigstrvecsize - 1 - i]);
    }
    auto endsqum = steady_clock::now();
    cout << "Swapped Quietly Using Move"
         << " in " << duration_cast<duration<double>>(endsqum - startsqum).count()
         << " seconds" << endl;

    cout << endl;
}

void Swap(int array[], int i, int j) {
    cout << "In Swap(int[], int, int)" << endl;
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

template <typename T>
void Swap(T array[], size_t i, size_t j) {
    cout << "In Swap(T[], size_t, size_t)" << endl;
    T temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

void SwapIntegersByValue(int x, int y) {
    cout << "In SwapIntegersByValue(int, int)" << endl;
    int temp = x;
    x = y;
    y = temp;
}

void Swap(int* x, int* y) {
    cout << "In Swap(int*, int*)" << endl;
    int temp = *x;
    *x = *y;
    *y = temp;
}

void Swap(int& x, int& y) {
    cout << "In Swap(int&, int&)" << endl;
    int temp = x;
    x = y;
    y = temp;
}

void SwapRValueReferences(int&& x, int&& y) {
    cout << "In SwapRValueReferences(int&&, int&&)" << endl;
    int temp = x;
    x = y;
    y = temp;
}

template <typename T> void Swap(T& x, T& y) {
    cout << "In Swap(T&, T&)" << endl;
    T temp = x;
    x = y;
    y = temp;
}

template <typename T> void SwapQuiet(T& x, T& y) {
    T temp = x;
    x = y;
    y = temp;
}

template <typename T> void SwapQuietUsingMove(T& x, T& y) {
    T temp = move(x);
    x = move(y);
    y = move(temp);
}
