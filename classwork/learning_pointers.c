#include <stdio.h>

void swap_fail(int a, int b) {
    // Doesn't swap the two values.
    int temp = a;
    a = b;
    b = temp;
}

void swap(int* a, int* b) {
    // Swaps values stored in two pointers.
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 42;
    printf("x=%d\n", x);

    int* ptr = &x;
    printf("mem address=%ld\n", (long) ptr);
    printf("actural value=%d\n", *ptr);

    *ptr = 100;
    printf("x=%d\n", x);

    int** ppx = &ptr;
    printf("mem addy1=%ld\n", (long) ppx);
    printf("mem addy2=%ld\n", (long) *ppx);
    printf("actual value=%d\n", **ppx);

    int a = 12;
    int b = 17;
    swap_fail(a, b);
    printf("x=%d, y=%d\n", a, b);
    swap(&a, &b);
    printf("x=%d, y=%d\n", a, b);
}