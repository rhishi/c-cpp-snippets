/*
  Trying out std::string.
  See: http://en.cppreference.com/w/cpp/string/basic_string.
*/
#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::endl;

void test_substr_erase_insert_replace();
void test_resize();

int main() {

    test_substr_erase_insert_replace();
    test_resize();

    return 0;
}

// substr extracts a range
// erase deletes a range or a position
// insert inserts a string or a char at a position
// replace deletes a range and inserts a string at the start of the range
void test_substr_erase_insert_replace() {

    std::string line1 = "Two roads diverged in a yellow wood";
    std::string line2 { "And sorry I could not travel both" };

    // line1.push_back(", "); // Error: push_back only takes char, not string.
    line1.append(", ");
    line2.push_back('.');

    std::string twolines = line1 + line2;
    std::string twolinescopy;

    // find returns the start index, or std::string::npos.
    cout << twolines.find("roads") << endl;
    cout << twolines.find("woods") << endl;
    cout << std::string::npos << endl;

    // -------------------------------------------------------------------------
    // http://en.cppreference.com/w/cpp/string/basic_string/substr

    // substr(index, count) -- defaults: index = 0, count = npos

    // First word in two ways: hardcoded count, or use of find(' ').
    cout << twolines.substr(0, 3) << endl;
    cout << twolines.substr(0, twolines.find(' ')) << endl;

    // The rest after the first word in two ways: drop the count letting it
    // be default value std::string::npos, or use any large count.
    cout << twolines.substr(twolines.find(' ')) << endl;
    cout << twolines.substr(twolines.find(' '), 1000) << endl;

    // Exception (std::out_of_range) if index is not in range.
    // cout << twolines.substr(1000, 10) << endl;  // std::out_of_range exception

    // Also problem passing std::string::npos returned by find.
    // cout << twolines.substr(twolines.find("traveled")) << endl;  // exception

    // There is no substr(iterator) or substr(iterator, iterator).

    cout << endl;

    // -------------------------------------------------------------------------
    // http://en.cppreference.com/w/cpp/string/basic_string/erase

    // erase(index, count) -- defaults: index = 0, count = npos
    // returns the string itself.
    cout << twolines.substr().erase(0, twolines.find(' ')) << endl;
    cout << twolines.substr().erase(twolines.find(' ')) << endl;

    // erase(iterator) -- erases only one char, not until the end.
    // returns the iterator to the next char.
    twolinescopy = twolines;
    twolinescopy.erase(twolinescopy.begin());
    cout << twolinescopy << endl;

    // TODO: GCC 7 gives segmentation fault on the following code.
    // delete all space characters: using erase(iterator) and std::find.
    twolinescopy = twolines;
    for (auto begin = twolinescopy.begin();
        begin != twolinescopy.end();
        begin = twolinescopy.erase(std::find(begin, twolinescopy.end(), ' ')))
        ;
    cout << twolinescopy << endl;

    cout << endl;

    // -------------------------------------------------------------------------
    // http://en.cppreference.com/w/cpp/string/basic_string/insert

    cout << endl;

    // -------------------------------------------------------------------------
    // http://en.cppreference.com/w/cpp/string/basic_string/replace

    cout << endl;
}

// Taken from http://en.cppreference.com/w/cpp/string/basic_string/resize
void test_resize()
{
    std::cout << "Basic functionality of resize:\n";

    const unsigned int desired_length { 8 };
    std::string long_string { "Where is the end?" };
    std::string short_string { "Ha" };

    // Shorten
    std::cout << "Before: \"" << long_string << "\"\n";
    long_string.resize(desired_length);
    std::cout << "After: \"" << long_string << "\"\n";

    // Lengthen
    std::cout << "Before: \"" << short_string << "\"\n";
    short_string.resize(desired_length);
    std::cout << "After: \"" << short_string << "\"\n";
    std::cout << "After: size is " << short_string.size() << "\n";
    short_string.resize(2);
    std::cout << "After: \"" << short_string << "\"\n";
    std::cout << "After: size is " << short_string.size() << "\n";
    short_string.resize(desired_length, 'n');
    std::cout << "After: \"" << short_string << "\"\n";
    std::cout << "After: size is " << short_string.size() << "\n";

    std::cout << std::endl;

    // TODO: Apple LLVM Clang 8 results in segmentation faults, not exceptions bad_alloc, length_error.
#if 0
    std::cout  << "\nErrors:\n";
    {
        std::string s;

        try {
            // size is OK, no length_error
            // (may throw bad_alloc)
            s.resize(s.max_size() - 1, 'x');
        } catch (const std::bad_alloc&) {
            std::cout << "1. bad alloc\n";
        }

        try {
            // size is OK, no length_error
            // (may throw bad_alloc)
            s.resize(s.max_size(), 'x');
        } catch (const std::bad_alloc& exc) {
            std::cout << "2. bad alloc\n";
        }

        try {
            // size is BAD, throw length_error
            s.resize(s.max_size() + 1, 'x');
        } catch (const std::length_error&) {
            std::cout << "3. length error\n";
        }
     }
#endif
}
