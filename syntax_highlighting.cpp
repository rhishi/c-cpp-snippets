/*
  In Atom and TextMate, as of June 2017, some words in C++ variable declarations
  get coloured red (teal in TextMate).  They look quite odd, as if there were
  syntax errors.  This file is a set of such declarations, along with adjacent
  declarations of normal colours.  Prime suspect is presence of parentheses
  in the declaration.  Thus exhibiting inspirations behind C++11's uniform
  initialization syntax.
*/
#include <vector>
#include <utility>
#include <iostream>
#include <random>

char add_chars(char x, char y);
const char add_chars_const(char x, char y);
char* cycle_chars(char x, char y);
const char* cycle_chars_const(char x, char y);

std::ostream& operator<< (std::ostream& os, const std::pair<int, int>& pair);

int main() {

    const char ch = 'a';
    const char *pch = &ch;

    char (*f)(char, char);
    const char (*g)(char, char);
    char* (*j)(char, char);
    const char* (*k)(char, char);

    const char *greeta[] = { "hi", "world" };
    const char greetb[][6] = { "hi", "world" };
    const char (*greetc)[6] = greetb;

    std::pair<int, int> pair(4, 5);
    std::vector<int> vector(10);

    int ten(10);
    int multiply(int, int);
    int eleven (10);
    int twelve {12};
    int thirteen = 13;

    // Create high-quality but deterministic random number generator, and seed
    // it with a random value generated from a non-deterministic but low-quality
    // random number generator.
    std::random_device nondet_rand_gen;
    std::mt19937 mersenne_twister_rand_gena(nondet_rand_gen());
    std::mt19937 mersenne_twister_rand_genb{nondet_rand_gen()};

    // To get rid of unused variables warnings

    std::cout << ch << ", " << *pch << ", " << (int)ch << std::endl;

    f = add_chars;
    g = add_chars_const;
    std::cout << f(ch, '\n') << std::endl;
    std::cout << g(ch, '\r') << std::endl;
    std::cout << std::endl;

    j = cycle_chars;
    k = cycle_chars_const;
    std::cout << (*j)(ch, 'z') << std::endl;
    std::cout << (*k)(ch, 'x') << std::endl;
    std::cout << std::endl;

    for (auto i: {0, 1}) {
        std::cout << greeta[i] << greetb[i] << greetc[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << pair << std::endl;
    std::cout << std::endl;

    std::cout << "ten = " << ten << std::endl;
    std::cout << "ten times ten = " << multiply(ten, ten) << std::endl;
    std::cout << ten << eleven << twelve << thirteen << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < 20; i++) {
        std::cout << mersenne_twister_rand_gena() % 100 << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 20; i++) {
        std::cout << mersenne_twister_rand_genb() % 100 << " ";
    }
    std::cout << std::endl;
}

int multiply(int x, int y) {
    return x * y;
}

std::ostream& operator<< (std::ostream& os, const std::pair<int, int>& pair) {
    os << "(" << pair.first << "," << pair.second << ")";
    return os;
}

char add_chars(char x, char y) {
    return x + y;
}

const char add_chars_const(char x, char y) {
    return x + y;
}

char* cycle_chars(char x, char y) {
    static char buffer[11];
    for (int i = 0; i < 10; i++) {
        buffer[i] = x + y * i;
    }
    buffer[10] = '\0';
    return buffer;
}

const char* cycle_chars_const(char x, char y) {
    static char buffer[11];
    for (int i = 0; i < 10; i++) {
        buffer[i] = x + y * i;
    }
    buffer[10] = '\0';
    return buffer;
}
