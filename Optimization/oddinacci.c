#include <stdlib.h>
#include "oddinacci.h"

long oddinacci(long n) {
    if (n == 0 || n == 1) {
        return n;
    } else if (n % 2 == 0) {
        return oddinacci(n - 1) + oddinacci(n - 2);
    } else {
        return oddinacci(n - 1) + oddinacci(n - 2) + oddinacci(n - 3);
    }
}

long oddinacci_fast(long n) {
    int oldest = 0;
    int storage[] = {0, 1, 1};
    if (n <= 2) {
        return storage[n];
    }
    for (int i=3; i <= n; i++) {
        // Even
        if (i%2 == 0) {
            storage[oldest] = storage[(oldest+1)%3] + storage[(oldest+2)%3];
        } else {
            storage[oldest] = storage[0] + storage[1] + storage[2];
        }
        oldest = (oldest+1)%3;
    }
    return storage[(oldest+2)%3];
}
