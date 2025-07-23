#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bits.h"

// Assert that the argument is a bit of either 0 or 1
#define assert_bit(a) \
    do { \
        if ((a) != 0 && (a) != 1) { \
            assert(false); \
        } \
    } while (0)


// Get the nth bit. 0 indexed
uint16_t getbit(uint16_t number, int n) {
    uint16_t mask = 1 << n;
    return (number & mask) >> n;
}

// Get bits that are the given number of bits wide
uint16_t getbits(uint16_t number, int n, int wide) {
    uint16_t mask = ~(-1 << wide) << n;
    return (number & mask) >> n;
}

// Set the nth bit to the given bit value and return the result
uint16_t setbit(uint16_t number, int n) {
    uint16_t mask = (1 << n);
    return number | mask;
}

// Clear the nth bit
uint16_t clearbit(uint16_t number, int n) {
    uint16_t mask = ~(1 << n);
    return number & mask;
}

// Sign extend a number of the given bits to 16 bits
uint16_t sign_extend(uint16_t x, int bit_count) {
    if (getbit(x, bit_count-1) == 0) {
        return x;
    }
    uint16_t mask = -1 << bit_count;
    return x | mask;
}

bool is_positive(uint16_t number) {
    return getbit(number, 15) == 0;
}

bool is_negative(uint16_t number) {
    return getbit(number, 15) == 1;
}
