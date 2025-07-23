#include <unistd.h>

char* message = "Bacon, Egg, & Cheese\n";

int main() {
    // Register is the fastest bit of memory.
    register int i = 10;
top:
    if (i == 0) {
        goto end;
    }

    write(1, message, 13);
    i--;
    goto top;

end:
    return 0;
}

// Steps to write in assembler
// 1. write in C
// 2. extract all loops
// 3. then convert to assembler
