/*
  Tests random distributions.

  Uniform distribution over a given range of integers
  http://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution

  random_device: non-deterministic random number generator
  http://en.cppreference.com/w/cpp/numeric/random/random_device

  mt19937: deterministic random number generator using Mersenne Twister method
  http://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine
*/

#include <iostream>
#include <random>
#include <map>
#include <string>

void test_mt19937();
void uniform_distribution_using_random_device();
void uniform_distribution_using_modulus_random_device();
void uniform_distribution_using_mt19937();
void uniform_distribution_using_mt19937_seeded_with_random_device();

int main() {
    test_mt19937();
    uniform_distribution_using_random_device();
    uniform_distribution_using_modulus_random_device();
    uniform_distribution_using_mt19937();
    uniform_distribution_using_mt19937_seeded_with_random_device();
    return 0;
}

void test_mt19937() {

    // The 10000th consecutive invocation of a default-contructed std::mt19937
    // is required to produce the value 4123659995.
    // 2^32 - 1 is 4,294,967,295
    //             4,123,659,995
    //               171,307,300
    // 4123659995 factorizes into 5^1 7^1 13^2 17^1 23^1 1783^1
    // 4123659996 factorizes into 2^2 3^3 137^1 278701^1
    //  171307300 factorizes into 2^2 5^2 17^1 100769^1

    std::mt19937 mt_default;

    for (int i = 0; i < 10000 - 1; i++) mt_default();
    auto tenthousandth = mt_default();

    unsigned int expected = 4123659995;
    std::cout << "10000th mt19937 is " << tenthousandth << "."
              << "  That " << (tenthousandth == expected ? "matches" : "does not match")
              << " the spec." << std::endl;

    std::cout << std::endl;
}

// Example taken from http://en.cppreference.com/w/cpp/numeric/random/random_device
// It comes with a note:
// demo only: the performance of many implementations of random_device degrades
// sharply once the entropy pool is exhausted. For practical use random_device
// is generally only used to seed a PRNG such as mt19937.
void uniform_distribution_using_random_device() {
    std::cout << "uniform_distribution_using_random_device:" << std::endl;

    std::random_device rand_gen;
    std::uniform_int_distribution<int> uniform_dist(0, 9);

    std::map<int, int> histogram;
    for (int i = 0; i < 100000; i++) {
        auto sample = uniform_dist(rand_gen);
        histogram[sample]++;
    }

    for (auto pair : histogram) {
        std::cout << pair.first << ": " << std::string(pair.second / 100, '*') << std::endl;
    }
    std::cout << std::endl;
}

void print_histogram(std::map<int, int> histogram) {
    for (auto pair : histogram) {
        std::cout << pair.first << ": "
                  << std::string(pair.second / 100, '*')
                  << std::endl;
    }
    std::cout << std::endl;
}

// Why do we need uniform_int_distribution when a random number generator,
// such as random_device or mt19937, generates random integers in a range?
// Can't we just do `randgen() % K` to get random numbers from 0 to K - 1?
// Problem is, we can, but they won't be uniformly distributed --
// unless K divides N, where N is the size of the range of randgen().
//
// Let's say you have a uniform generator for integers between 0 to 99.
// If you do i % 10, you have 10 bins, each of size 10.
// If you do i % 11, you have 10 bins each of size 9, and bin 0 of size 10.
// So the bin 0 has higher probability than the other bins.
void uniform_distribution_using_modulus_random_device() {
    std::cout << "uniform_distribution_using_modulus_random_device:" << std::endl;

    std::random_device rand_gen;
    std::uniform_int_distribution<int> uniform_dist(0, 99);

    std::map<int, int> histogram;
    for (int i = 0; i < 100000; i++) {
        auto sample = uniform_dist(rand_gen) % 11;
        histogram[sample]++;
    }

    print_histogram(histogram);
}

// uses the default-constructed mt19937; the same seqeuence of values for every run.
void uniform_distribution_using_mt19937() {
    std::cout << "uniform_distribution_using_mt19937:" << std::endl;

    std::mt19937 rand_gen;
    std::uniform_int_distribution<int> uniform_dist(0, 9);

    std::map<int, int> histogram;
    for (int i = 0; i < 100000; i++) {
        auto sample = uniform_dist(rand_gen);
        histogram[sample]++;
    }

    print_histogram(histogram);
}

// uses mt19937 seeded using random_device; high-quality, non-deterministic sequence of randoms.
void uniform_distribution_using_mt19937_seeded_with_random_device() {
    std::cout << "uniform_distribution_using_mt19937_seeded_with_random_device:" << std::endl;

    std::random_device seed_gen;
    std::mt19937 rand_gen {seed_gen()};
    std::uniform_int_distribution<int> uniform_dist(0, 9);

    std::map<int, int> histogram;
    for (int i = 0; i < 100000; i++) {
        auto sample = uniform_dist(rand_gen);
        histogram[sample]++;
    }

    print_histogram(histogram);
}
