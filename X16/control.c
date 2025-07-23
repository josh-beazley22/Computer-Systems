#include <stdio.h>
#include <stdlib.h>
#include "bits.h"
#include "control.h"
#include "instruction.h"
#include "x16.h"
#include "trap.h"
#include "decode.h"


// Update condition code based on result
void update_cond(x16_t* machine, reg_t reg) {
    uint16_t result = x16_reg(machine, reg);
    if (result == 0) {
        x16_set(machine, R_COND, FL_ZRO);
    } else if (is_negative(result)) {
        x16_set(machine, R_COND, FL_NEG);
    } else {
        x16_set(machine, R_COND, FL_POS);
    }
}



// Execute a single instruction in the given X16 machine. Update
// memory and registers as required. PC is advanced as appropriate.
// Return 0 on success, or -1 if an error or HALT is encountered.
int execute_instruction(x16_t* machine) {
    // Fetch the instruction and advance the program counter
    uint16_t pc = x16_pc(machine);
    uint16_t instruction = x16_memread(machine, pc);
    x16_set(machine, R_PC, pc + 1);

    if (LOG) {
        fprintf(LOGFP, "0x%x: %s\n", pc, decode(instruction));
    }

    // Variables we might need in various instructions
    reg_t dst, src1, src2, base;
    uint16_t result, indirect, offset, imm, cond;
    uint16_t jsrflag, flag, op2, val1, val2;

    // Decode the instruction
    uint16_t opcode = getopcode(instruction);
    switch (opcode) {
        case OP_ADD:
            // segment the instruction into reg_t tags
            dst = (reg_t)getbits(instruction, 9, 3);
            src1 = (reg_t)getbits(instruction, 6, 3);
            flag = getbit(instruction, 5);

            // Here is memory for both numbers to add
            val1 = x16_reg(machine, src1);

            // Different behavior when pulling from register
            // vs reading binary number
            if (flag) {
                offset = getbits(instruction, 0, 5);
                val2 = sign_extend(offset, 5);
            } else {
                // Use register value
                src2 = (reg_t)getbits(instruction, 0, 3);
                val2 = x16_reg(machine, src2);
            }
            // Set the register and update cond
            x16_set(machine, dst, val1+val2);
            update_cond(machine, dst);
            break;

        case OP_AND:
            // segment the instruction into reg_t tags
            dst = (reg_t)getbits(instruction, 9, 3);
            src1 = (reg_t)getbits(instruction, 6, 3);
            flag = getbit(instruction, 5);

            // Here is memory for both numbers to add
            val1 = x16_reg(machine, src1);

            // Different behavior when pulling from register
            // vs reading binary number
            if (flag) {
                offset = getbits(instruction, 0, 5);
                val2 = sign_extend(offset, 5);
            } else {
                // Use register value
                src2 = (reg_t)getbits(instruction, 0, 3);
                val2 = x16_reg(machine, src2);
            }
            // Set the register and update cond
            x16_set(machine, dst, val1 & val2);
            update_cond(machine, dst);

            break;

        case OP_NOT:
            // parse instructions to local variables
            dst = (reg_t)getbits(instruction, 9, 3);
            src1 = (reg_t)getbits(instruction, 6, 3);

            // set destination register
            offset = x16_reg(machine, src1);
            x16_set(machine, dst, ~offset);

            // Update the COND flag
            if (~offset == 0) {
                x16_set(machine, R_COND, FL_ZRO);
            } else if (~offset < 0) {
                x16_set(machine, R_COND, FL_NEG);
            } else {
                x16_set(machine, R_COND, FL_POS);
            }
            break;

        case OP_BR:
            // printf("0b%.16b\n", instruction);
            uint16_t N_flag, Z_flag, P_flag;
            N_flag = getbit(instruction, 11);
            Z_flag = getbit(instruction, 10);
            P_flag = getbit(instruction, 9);
            offset = getbits(instruction, 0, 9);
            offset = sign_extend(offset, 9);
            cond = x16_cond(machine);

            if ((N_flag && (cond & FL_NEG)) ||
                (Z_flag && (cond & FL_ZRO)) ||
                (P_flag && (cond & FL_POS)) ||
                ((N_flag | Z_flag | P_flag) == 0)) {
                uint16_t cur_pc = x16_pc(machine);
                x16_set(machine, R_PC, cur_pc + offset);
            }

            break;

        case OP_JMP:
            dst = (reg_t)getbits(instruction, 6, 3);
            result = x16_reg(machine, dst);
            x16_set(machine, R_PC, result);
            break;

        case OP_JSR:
            flag = getbit(instruction, 11);
            if (flag) {
                // immediate mode. grab number from instruction.
                result = getbits(instruction, 0, 11);
                result = sign_extend(result, 11);

                pc = x16_pc(machine);
                x16_set(machine, R_R7, pc);
                x16_set(machine, R_PC, pc+result);
            } else {
                // register mode. grab number from register.
                dst = (reg_t)getbits(instruction, 6, 3);
                result = x16_reg(machine, dst);

                pc = x16_pc(machine);
                x16_set(machine, R_R7, pc);
                x16_set(machine, R_PC, result);
            }
            break;

        case OP_LD:
            // read instruction
            offset = getbits(instruction, 0, 9);
            offset = sign_extend(offset, 9);
            pc = x16_pc(machine);

            // load from memory
            uint16_t mem_value = x16_memread(machine, pc+offset);

            // store into destination register
            dst = (reg_t)getbits(instruction, 9, 3);
            x16_set(machine, dst, mem_value);

            // Update the COND flag
            if (mem_value == 0) {
                x16_set(machine, R_COND, FL_ZRO);
            } else if ((mem_value >> 15) & 1) {
                // negative
                x16_set(machine, R_COND, FL_NEG);
            } else {
                x16_set(machine, R_COND, FL_POS);
            }
            break;

        case OP_LDI:
            // read instruction
            offset = getbits(instruction, 0, 9);
            offset = sign_extend(offset, 9);
            pc = x16_pc(machine);

            // load from memory
            uint16_t mem_intermediate = x16_memread(machine, pc+offset);
            uint16_t mem_final = x16_memread(machine, mem_intermediate);
            // store into destination register
            dst = (reg_t)getbits(instruction, 9, 3);
            x16_set(machine, dst, mem_final);

            // Update the COND flag
            if (mem_final == 0) {
                x16_set(machine, R_COND, FL_ZRO);
            } else if ((mem_final >> 15) & 1) {
                // negative
                x16_set(machine, R_COND, FL_NEG);
            } else {
                x16_set(machine, R_COND, FL_POS);
            }
            break;

        case OP_LDR:
            // read instruction
            offset = getbits(instruction, 0, 6);
            offset = sign_extend(offset, 6);
            src1 = (reg_t)getbits(instruction, 6, 3);
            uint16_t address = x16_reg(machine, src1) + offset;

            // load from memory
            uint16_t mem_ldr = x16_memread(machine, address);
            // store into destination register
            dst = (reg_t)getbits(instruction, 9, 3);
            x16_set(machine, dst, mem_ldr);

            // Update the COND flag
            if (mem_ldr == 0) {
                x16_set(machine, R_COND, FL_ZRO);
            } else if ((mem_ldr >> 15) & 1) {
                // negative
                x16_set(machine, R_COND, FL_NEG);
            } else {
                x16_set(machine, R_COND, FL_POS);
            }
            break;

        case OP_LEA:
            // read instruction
            offset = getbits(instruction, 0, 9);
            offset = sign_extend(offset, 9);
            pc = x16_pc(machine);

            // store into destination register
            dst = (reg_t)getbits(instruction, 9, 3);
            x16_set(machine, dst, pc+offset);

            // Update the COND flag
            if (pc+offset == 0) {
                x16_set(machine, R_COND, FL_ZRO);
            } else if ((pc+offset >> 15) & 1) {
                // negative
                x16_set(machine, R_COND, FL_NEG);
            } else {
                x16_set(machine, R_COND, FL_POS);
            }
            break;

        case OP_ST:
            // read instruction
            offset = getbits(instruction, 0, 9);
            offset = sign_extend(offset, 9);
            pc = x16_pc(machine);

            // read register
            src1 = (reg_t)getbits(instruction, 9, 3);
            result = x16_reg(machine, src1);

            // write to memory
            x16_memwrite(machine, pc+offset, result);
            break;

        case OP_STI:
            // read instruction
            offset = getbits(instruction, 0, 9);
            offset = sign_extend(offset, 9);
            pc = x16_pc(machine);

            // read register
            src1 = (reg_t)getbits(instruction, 9, 3);
            result = x16_reg(machine, src1);

            // write to memory
            uint16_t mem_sti = x16_memread(machine, pc+offset);
            x16_memwrite(machine, mem_sti, result);
            break;

        case OP_STR:
            // read instruction
            offset = getbits(instruction, 0, 6);
            offset = sign_extend(offset, 6);

            // read register
            src1 = (reg_t)getbits(instruction, 9, 3);
            result = x16_reg(machine, src1);
            src2 = (reg_t)getbits(instruction, 6, 3);
            uint16_t baseR = x16_reg(machine, src2);

            // write to memory
            x16_memwrite(machine, baseR+offset, result);

            break;

        case OP_TRAP:
            // Execute the trap -- do not rewrite
            return trap(machine, instruction);

        case OP_RES:
        case OP_RTI:
        default:
            // Bad codes, never used
            abort();
    }

    return 0;
}
