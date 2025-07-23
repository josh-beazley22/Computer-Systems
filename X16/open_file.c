#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void print_binary(uint16_t word) {
    for (int i = 15; i >= 0; i--) {
        putchar((word >> i) & 1 ? '1' : '0');
    }
    putchar('\n');
}

int main() {
    FILE *file = fopen("a.obj", "rb");
    if (!file) {
        perror("Failed to open a.obj");
        return 1;
    }

    uint16_t word;
    while (fread(&word, sizeof(word), 1, file) == 1) {
        print_binary(word);
    }

    fclose(file);
    return 0;
}
