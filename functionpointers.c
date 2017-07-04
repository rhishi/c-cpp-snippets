/*
  Understanding function type and function pointer type in C.

  Quick summary:
  - There is just the function pointer type.
  - Quoting K&R:
    You can define pointers to functions, which can be assigned, placed in
    arrays, passed to functions, returned by functions, and so on.
  - But you need not use strictly the pointer-style syntax i.e. `*` and `&`.
  - You can just use the function/variable names.
  - Quoting K&R:
    In the call to `qsort`, `strcmp` and `numcmp` are addresses of functions.
    Since they are known to be functions, the `&` operator is not necessary.
    In the same way that it is not needed before an array name.

  K&R drops `&` when function names are used as values of function pointer type,
  but does not drop `*` when defining function parameter that is of function
  pointer type.  Also does not drop `*` when invoking a function pointer.
  For example:
  void qsort(... int (*comp)(void *, void *)) {
      ...
      if ((*comp)(v[i], v[left]) < 0)
          ...
  }

  But with today's compilers, even with -std=c90 option, you can drop explicit
  `*` when declaring a function pointer parameter or typedef, and drop it when
  invoking a function pointer.

  Anyway, don't sweat it too much on whether `&`, `*` are needed or not.
  If in doubt, go with the K&R way of always using `*`, and skipping `&`.

  In any case, don't ignore warnings of incompatible pointer types when passing
  incorrect functions to function pointer parameters/variables.

  Compile and Run this file as one of following ways:

  gcc-7 -Wall -Wextra -std=c90 -ansi -pedantic functionpointers.c -o functionpointers.out && ./functionpointers.out
  clang -Wall functionpointers.c -o functionpointers.out && ./functionpointers.out

  You should see incompatible-pointer-types warnings, but also some valid output
  and some garbage output.
*/

#include <stdio.h>

void map1(int input[], int (*f)(int), int output[], size_t size) {
    size_t i;
    for (i = 0; i < size; i++) {
        output[i] = (*f)(input[i]);
    }
}

void map2(int input[], int (*f)(int), int output[], size_t size) {
    size_t i;
    for (i = 0; i < size; i++) {
        output[i] = f(input[i]);
    }
}

void map3(int input[], int f(int), int output[], size_t size) {
    size_t i;
    for (i = 0; i < size; i++) {
        output[i] = f(input[i]);
    }
}

typedef int unary_func(int);

void map4(int input[], unary_func f, int output[], size_t size) {
    size_t i;
    for (i = 0; i < size; i++) {
        output[i] = f(input[i]);
    }
}

void map5(int input[], unary_func* f, int output[], size_t size) {
    size_t i;
    for (i = 0; i < size; i++) {
        output[i] = f(input[i]);
    }
}

typedef int (*unary_function)(int);

void map6(int input[], unary_function f, int output[], size_t size) {
    size_t i;
    for (i = 0; i < size; i++) {
        output[i] = f(input[i]);
    }
}

void print_array(int array[], size_t size) {
    size_t i;
    for (i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int square_int(int x) {
    return x * x;
}

float square_float(float x) {
    return x * x;
}

int main() {
    int input[] = { 1, 2, 3, 4 };
    int output[4];
    const size_t size = 4;

    print_array(input, size);
    printf("\n");

    map1(input, square_int, output, size);
    print_array(output, size);
    map1(output, &square_int, output, size);
    print_array(output, size);
    printf("\n");

    map2(input, square_int, output, size);
    print_array(output, size);
    map2(output, &square_int, output, size);
    print_array(output, size);
    printf("\n");

    map3(input, square_int, output, size);
    print_array(output, size);
    map3(output, &square_int, output, size);
    print_array(output, size);
    printf("\n");

    map4(input, square_int, output, size);
    print_array(output, size);
    map4(output, &square_int, output, size);
    print_array(output, size);
    printf("\n");

    map5(input, square_int, output, size);
    print_array(output, size);
    map5(output, &square_int, output, size);
    print_array(output, size);
    printf("\n");

    map6(input, square_int, output, size);
    print_array(output, size);
    map6(output, &square_int, output, size);
    print_array(output, size);
    printf("\n");

    /* Try to pass square_float as the int (*)(int) function pointer.
       This will result in compiler warnings about incompatible pointer types
       [-Wincompatible-pointer-types].
    */

    map1(input, square_float, output, size);
    print_array(output, size);
    map1(input, &square_float, output, size);
    print_array(output, size);
    printf("\n");

    map2(input, square_float, output, size);
    print_array(output, size);
    map2(input, &square_float, output, size);
    print_array(output, size);
    printf("\n");

    map3(input, square_float, output, size);
    print_array(output, size);
    map3(input, &square_float, output, size);
    print_array(output, size);
    printf("\n");

    map4(input, square_float, output, size);
    print_array(output, size);
    map4(input, &square_float, output, size);
    print_array(output, size);
    printf("\n");

    map5(input, square_float, output, size);
    print_array(output, size);
    map5(input, &square_float, output, size);
    print_array(output, size);
    printf("\n");

    map6(input, square_float, output, size);
    print_array(output, size);
    map6(input, &square_float, output, size);
    print_array(output, size);
    printf("\n");

    /*
      The above map invocations with square_float generate garbage values.
      With both Clang and GCC, the same garbage value is repeated over all
      output.  And that one common value varies with every execution.

      Before we added map5, map6, with Clang, seemed like the one common garbage
      value was always divisible by 2^7 but not 2^8.
      Defining one more function "float identity_float(float x)" below square_float
      changed things slightly: still one common garbage value that changes with
      every execution, but its max-power-of-2 factor became 2^4 instead of 2^7.
      More strange: assigning that newly-defined identity_float function to a
      fresh unary_func pointer declared here changed things again a bit and the
      garbage value got its max-power-of-2 factor to be 2^5.

      Adding map5 and invoking it here, also caused the max-power-of-2 factor to
      switch from 2^7 to 2^5.  Adding map6 changed it to 2^6.
      With GCC 7, seemed like one common garbage value was always odd.

      Anyway, the moral of the story: they are garbage; don't ignore warnings.
    */

    /* Using sizeof on function type results in warning in GCC 7:
       invalid application of 'sizeof' to a function type [-Wpointer-arith]. */
    printf("%lu\n", sizeof(unary_func));      /* undefined; 1 in GCC 7 */
    printf("%lu\n", sizeof(unary_func *));    /* pointer size; 8 on 64-bit machine */
    printf("%lu\n", sizeof(unary_function));  /* pointer size; 8 on 64-bit machine */
    printf("%lu\n", sizeof(void *));          /* pointer size; 8 on 64-bit machine */

    return 0;
}
