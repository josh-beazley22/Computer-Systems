#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ultron.h"


void ultron_init(Ultron* ultron) {
    // clear memory array to 0
    for (int i=0; i < ULTRON_MEMORY_SIZE; i++) {
        ultron->memory[i] = 0;
    }

    // Initialize program counter and accumulator to 0
    ultron->pc = 0;
    ultron->acc = 0;
}

void ultron_dump(Ultron* ultron) {
    // print contents of ultron
    printf("PC: %u\n", ultron->pc);
    printf("AC: %d\n", ultron->acc);
    for (int i=0; i < ULTRON_MEMORY_SIZE; i++) {
        // right adjusted output
        printf("%2d%5d\n", i, ultron->memory[i]);
    }
}

int ultron_memory(Ultron* ultron, int location) {
    return ultron->memory[location];
}

int ultron_accumulator(Ultron* ultron) {
    return ultron->acc;
}

int ultron_pc(Ultron* ultron) {
    return ultron->pc;
}

void ultron_load(Ultron* ultron, int size, int* instructions) {
    if (size > ULTRON_MEMORY_SIZE) {
        printf("Size of instructions is greater than ultron memory.");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        ultron->memory[i] = *(instructions+i);
    }
}

int ultron_run(Ultron* ultron) {
    // run until the program is exited
    while (0 == 0) {
        if (ultron->pc >= ULTRON_MEMORY_SIZE || ultron->pc < 0) {
            // Program counter outside the range of allowable memory
            return -3;
        }

        int instruction = ultron->memory[ultron->pc];
        int operation = instruction / 100;  // get first two digits
        int operand = instruction % 100;  // get last two digits
        if (operand >= ULTRON_MEMORY_SIZE || operand < 0) {
            // Operand outside the range of allowable memory
            return -2;
        }
        if (operation == 10) {
            // read a word from input into memory
            printf("? ");
            scanf("%d", &ultron->memory[operand]);
        } else if (operation == 11) {
            // write a word from memory to the screen
            printf("= %d\n", ultron->memory[operand]);
        } else if (operation == 20) {
            // load a word from memory into accumulator
            ultron->acc = ultron_memory(ultron, operand);
        } else if (operation == 21) {
            // store a word from accumulator into memory
            ultron->memory[operand] = ultron_accumulator(ultron);
        } else if (operation == 30) {
            // add to accumulator the value from memory
            ultron->acc += ultron_memory(ultron, operand);
        } else if (operation == 31) {
            // subtract from accum the value in memory
            ultron->acc -= ultron_memory(ultron, operand);
        } else if (operation == 40) {
            // branch if accumulator is negative to location
            if (ultron->acc < 0) {
                ultron->pc = operand-1;
            }
        } else if (operation == 43) {
            // stop executing
            return 0;
        } else {
            // operation is invalid
            return -1;
        }
        ultron->pc++;
    }
}
