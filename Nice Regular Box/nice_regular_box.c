#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_n_pluses(int n) {
    // Print to stdout n '+' characters
    for (int i = 0; i < n+4; i++) {
        printf("+");
    }
    printf("\n");
}

int main() {

    // Declare a buffer.
    int buffer_size = 200;
    long buffer[buffer_size];

    // returns value of buffer[1]
    *(buffer + 1);

    // Find how many lines of input to read.
    fgets(buffer, buffer_size, stdin);
    int n = atoi(&buffer[0]); 
    // [4, 5,  6, '\n', '\0'], '\0'

    // Find the length of the longest line of input.
    // Exit loop after reading n lines.
    int max = 0;
    for (int i = 0; i < n; i++) {
        // Exit loop when EOF is reached.
        // And store next line of input into the buffer.
        if (fgets(buffer, buffer_size, stdin) == NULL) {
            break;
        }
        int length = (int) strlen(buffer)-1;
        if (length > max) {
            max = length;
        }
    }
    // reset stdin back to beginning of file.
    fseek(stdin, 0, SEEK_SET);
    // skip the first line (which declares the number of lines to read).
    fgets(buffer, buffer_size, stdin);

    // Print formatted strings
    print_n_pluses(max);
    // Exit loop after reading n lines.
    for (int i = 0; i < n; i++) {
        // Exit loop when EOF is reached.
        // And store next line of input into the buffer.
        if (fgets(buffer, buffer_size, stdin) == NULL) {
            break;
        }
        // Build the fluff string. A string of only space characters.
        int n = (int) strlen(buffer) - 1;
        int len = max - n + 1;
        char fluff[len+1] = {'\s'};
        for (int i = 0; i < len; i++) {
            fluff[i] = ' ';
        }
        fluff[len] = '\0';
        buffer[n] = '\0';

        // Print to stdout
        printf("+ ");
        printf("%s", buffer);
        printf("%s", fluff);
        printf("+\n");
    }
    print_n_pluses(max);
}

