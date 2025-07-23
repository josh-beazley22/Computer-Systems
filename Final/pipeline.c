#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pipeline.h"

const char* instruction_names[] = {
    "NOOP", "READ", "WRITE", "ADD", "SUB",
    "MUL", "DIV", "ADDF", "SUBF", "MULF", "DIVF"
};

void print_cpu(int cycle, CPU* cpu) {
    printf("Cycle: %3d --", cycle);
    for (int i = 0; i < PIPELINE_SIZE; i++) {
        printf(" %5s", instruction_names[cpu->cpu_stage[i]]);
    }
    printf(" : execution step: %d\n", cpu->execute_step);
}

bool cpu_all_noop(CPU* cpu) {
    for (int i = 0; i < PIPELINE_SIZE; i++) {
        if (cpu->cpu_stage[i] != NOOP) {
            return false;
        }
    }
    return true;
}

int main() {
    // 1. load instructions into Program
    Instruction program_instructions[] = {
        WRITE, ADDF, SUB, WRITE, DIVF
    };

    // Just allocate the Program struct, not the instructions array
    Program* prog = malloc(sizeof(Program));
    prog->instructions = program_instructions;  // points to stack array
    prog->size = sizeof(program_instructions) / sizeof(program_instructions[0]);
    prog->current = 0;

    // 2. Initialize CPU with NOOP
    CPU* cpu = malloc(sizeof(CPU));
    cpu->execute_step = 0;
    for (int i = 0; i < PIPELINE_SIZE; i++) {
        cpu->cpu_stage[i] = NOOP;
    }
    int cycle = 0;

    // 3. pop instructions into CPU memory from the Program pipeline
    while (prog->current < prog->size || !cpu_all_noop(cpu)) {
        // check if CPU advances or stalls
        int ex_stage_cycles = instruction_cycles[cpu->cpu_stage[EX_STAGE]];
        if (cpu->execute_step+1 >= ex_stage_cycles) {
            // entire pipeline moves forwards.
            for (int i = PIPELINE_SIZE-1; i > 0; i--) {
                cpu->cpu_stage[i] = cpu->cpu_stage[i-1];
            }
            if (prog->current < prog->size) {
                // add new instruction
                cpu->cpu_stage[IF_STAGE] = prog->instructions[prog->current];
                prog->current += 1;
                cpu->execute_step = 0;
            } else {
                cpu->cpu_stage[IF_STAGE] = NOOP;
                cpu->execute_step = 0;
            }
        } else {
            // Instruction in EX stage needs multiple clock cycles to complete.
            cpu->cpu_stage[WB_STAGE] = cpu->cpu_stage[ME_STAGE];
            cpu->cpu_stage[ME_STAGE] = NOOP;
            cpu->execute_step += 1;
        }
        cycle++;
        print_cpu(cycle, cpu);
    }
    printf("Total clock cycles: %d\n", cycle);
}
