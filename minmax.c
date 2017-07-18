/*
  How to write MIN, MAX macros.
  (But really, don't!  Use inline functions.
  If using C++, templatized inline functions.)

  Write MIN macro that returns the smaller of its 2 arguments.
  Similarly write MAX macro.

  This is a typical embedded software interview question, featuring in
  - Google's Embedded SWE Concepts interview guide given by Google recruiter in
    June 2017.
  - ProgrammerWorld - Best Questions for Embedded Programmers
    http://faq.programmerworld.net/programming/Best-Questions-for-Embedded-Programmers.htm
    as of June 2017.

  A great answer is found on StackOverflow.  It gives multiple candidates.
  This file is pretty much a summary of that discussion.
  https://stackoverflow.com/questions/3437404/min-and-max-in-c

  TODO: integer min/max using bit hacks
  TODO: two variants that avoid double evaluation
  TODO: variant that avoids mismatching types
*/
#include <stdio.h>

/* Rookie attempt, with no parentheses.  Several failure modes. */

#define MIN(x, y) x < y ? x : y
#define MAX(x, y) x > y ? x : y

void test1() {
    // should print:
    // test1: 3 5 	3 5 5 3 	3 13 	16

    printf("test1: ");

    printf("%d ", MIN(5, 3));  // correct. 3
    printf("%d ", MAX(5, 3));  // correct. 5
    printf("\t");
    printf("%d ", 2 * MIN(5, 3));  // error. wanted 6, got 3 (because 2 * 5 < 3 ? 5 : 3)
    printf("%d ", 2 * MAX(5, 3));  // error. wanted 10, got 5 (because 2 * 5 > 3 ? 5 : 3)
    printf("%d ", 2 * MIN(3, 5));  // error. wanted 6, got 5 (because 2 * 3 < 5 ? 3 : 5)
    printf("%d ", 2 * MAX(3, 5));  // error. wanted 10, got 3 (because 2 * 3 > 5 ? 3 : 5)
    printf("\t");

    int x = 8;
    int y = 5;
    printf("%d ", MIN(x - y, x + y));  // correct. 3
    printf("%d ", MAX(x - y, x + y));  // correct. 13
    printf("\t");

    printf("%d ", MIN(x > y ? 2 * x : y, 0));  // error. expected 0, got 16
                                               // What are the precedence rules in:
                                               // x > y ? 2 * x : y < 0 ? x > y ? 2 * x : y : 0
    printf("\n");
}

#undef MIN
#undef MAX

/* Parentheses to avoid most of precedence errors.  Still relying on precedence of `<' and `?:'. */

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

void test2() {
    // should print:
    // test2: 3 5 	6 10 6 10 	3 13 	0 	6 9 7 	7 9 7

    printf("test2: ");

    printf("%d ", MIN(5, 3));  // correct. 3
    printf("%d ", MAX(5, 3));  // correct. 5
    printf("\t");
    printf("%d ", 2 * MIN(5, 3));  // correct. 6
    printf("%d ", 2 * MAX(5, 3));  // correct. 10
    printf("%d ", 2 * MIN(3, 5));  // correct. 6
    printf("%d ", 2 * MAX(3, 5));  // correct. 10
    printf("\t");

    int x = 8;
    int y = 5;
    printf("%d ", MIN(x - y, x + y));  // correct. 3
    printf("%d ", MAX(x - y, x + y));  // correct. 13
    printf("\t");

    printf("%d ", MIN(x > y ? 2 * x : y, 0));  // correct. 0
    printf("\t");

    x = 8;
    y = 5;
    printf("%d ", MIN(x++, y++));  // error. expected 5, got 6.
    printf("%d %d ", x, y);        // error. expected 9 6, got 9 7.
    printf("\t");
    x = 8;
    y = 5;
    printf("%d ", MIN(++x, ++y));  // error. expected 6, got 7.
    printf("%d %d ", x, y);        // error. expected 9 6, got 9 7.

    printf("\n");
}

#undef MIN
#undef MAX

/* Full parentheses to avoid any precedence confusion.  Still double evaluation though. */

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#include <math.h>

void test3() {
    // should print:
    // With Apple LLVM Clang 8:
    // test3: +0.0 -0.0 	-0.0 +0.0 -0.0 +0.0 	+0.0 -0.0 +0.0 -0.0
    // With GCC 7:
    // test3: +0.0 -0.0 	-0.0 +0.0 -0.0 +0.0 	-0.0 -0.0 +0.0 +0.0

    // According to http://en.cppreference.com/w/c/numeric/math/fmin,
    // Functions in fmin, fmax family are not required to be sensitive to the
    // sign of zero, although some implementations additionally enforce that +0
    // is larger than -0.  E.g. min(+0, -0) = min(-0, +0) = -0.
    // Apple LLVM Clang 8 ignores the sign of zero, while GCC 7 enforces it.

    printf("test3: ");
    printf("%+1.1f %+1.1f ", +0.0, -0.0);
    printf("\t");

    printf("%+1.1f ", MIN(+0.0, -0.0));
    printf("%+1.1f ", MIN(-0.0, +0.0));
    printf("%+1.1f ", MAX(+0.0, -0.0));
    printf("%+1.1f ", MAX(-0.0, +0.0));
    printf("\t");

    printf("%+1.1f ", fminf(+0.0, -0.0));
    printf("%+1.1f ", fminf(-0.0, +0.0));
    printf("%+1.1f ", fmaxf(+0.0, -0.0));
    printf("%+1.1f ", fmaxf(-0.0, +0.0));

    printf("\n");
}

#undef MIN
#undef MAX

/* Using the macros defined in sys/param.h.  Seem similar to our definition above. */

#include <sys/param.h>

void test4() {
    // should print:
    // test4: 3 5 	6 9 7 	7 9 7 	-0.0 +0.0 -0.0 +0.0

    printf("test4: ");

    printf("%d ", MIN(5, 3));  // correct. 3
    printf("%d ", MAX(5, 3));  // correct. 5
    printf("\t");

    int x = 8;
    int y = 5;
    printf("%d ", MIN(x++, y++));  // error. expected 5, got 6.
    printf("%d %d ", x, y);        // error. expected 9 6, got 9 7.
    printf("\t");
    x = 8;
    y = 5;
    printf("%d ", MIN(++x, ++y));  // error. expected 6, got 7.
    printf("%d %d ", x, y);        // error. expected 9 6, got 9 7.
    printf("\t");

    printf("%+1.1f ", MIN(+0.0, -0.0));
    printf("%+1.1f ", MIN(-0.0, +0.0));
    printf("%+1.1f ", MAX(+0.0, -0.0));
    printf("%+1.1f ", MAX(-0.0, +0.0));

    printf("\n");
}

int main() {

    test1();
    test2();
    test3();
    test4();

    return 0;
}
