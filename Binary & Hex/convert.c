#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_binary(char* str_num) {
    // Declare a buffer to pop binary ints into.
    int buffer[200];

    // Convert string to integer
    int input = atoi(str_num);
    printf("%d=", input);

    // Edge case where input is zero.
    if (input == 0) {
        printf("0\n");
        return;
    }

    int i = 0;
    while (input != 0) {
        // Store least significant bit.
        buffer[i] = input % 2;
        // Integer division
        input /= 2;
        i++;
    }
    // Print the array backwards to the console.
    for (int j = i-1; j > 0; j--) {
        printf("%d", buffer[j]);
        // print a space every 4 characters.
        if (j % 4 == 0) {
            printf(" ");
        }
    }
    printf("%d\n", buffer[0]);
}
void print_hex(char* str_num) {
    // Declare a buffer to pop hexidecimal ints into.
    int buffer[200];

    // Convert string to integer
    int input = atoi(str_num);
    printf("%d=0x", input);

    // Edge case where input is zero.
    if (input == 0) {
        printf("0\n");
        return;
    }

    int i = 0;
    while (input != 0) {
        // Store least significant bit.
        buffer[i] = input % 16;
        // Integer division
        input /= 16;
        i++;
    }
    // Print the array backwards to the console.
    for (int j = i-1; j >= 0; j--) {
        // Convert ints >10 into hexidecimal characters.
        if (buffer[j] > 9) {
            char c = 'a' + (buffer[j] - 10);
            printf("%c", c);
        } else {
            printf("%d", buffer[j]);
        }
        if (j == 0) {
            break;
        }
        // print a space every 2 characters.
        if (j % 2 == 0) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    // Too few arguments.
    if (argc <= 2) {
        printf("Usage: ./convert [-x|-b] num1 [num2 ...]\n");
        return 0;
    }
    // Check if second arg is a flag
    if (argv[2] && argv[2][0] == '-') {
        printf("Usage: ./convert [-x|-b] num1 [num2 ...]\n");
        return 0;
    }

    opterr = 0;  // disable getopt's error message to stderr
    int type = getopt(argc, argv, "xb");
    if (type == 'x') {
        for (int i = 2; i < argc; i++) {
            print_hex(argv[i]);
        }
    } else if (type == 'b') {
        for (int i = 2; i < argc; i++) {
            print_binary(argv[i]);
        }
    } else {
        printf("Usage: ./convert [-x|-b] num1 [num2 ...]\n");
    }
}
