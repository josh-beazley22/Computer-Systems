#include <stdio.h>
#include "oddinacci.h"
#include "measure.h"

#define ODDINACCI_ITERS 40


int main(int argc, char** argv) {
    long val1 = 0;
    double slow = measure(oddinacci, ODDINACCI_ITERS, &val1);
    printf("odd slow=%ld\n", val1);

    long val2 = 0;
    double fast = measure(oddinacci_fast, ODDINACCI_ITERS, &val2);
    printf("odd fast=%ld\n", val2);

    printf("slow=%f, fast=%f\n", slow, fast);
    printf("factor=%f\n", slow / fast);
}
