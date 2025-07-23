#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <assert.h>
#include "instruction.h"

typedef struct DictEntry {
    char key[100];
    int line_number;
    struct DictEntry *next;
} DictEntry;

DictEntry *dictionary = NULL;

void add_entry(const char *word, int line) {
    DictEntry *entry = malloc(sizeof(DictEntry));
    if (!entry) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    strncpy(entry->key, word, sizeof(entry->key) - 1);
    entry->key[sizeof(entry->key) - 1] = '\0';
    entry->line_number = line;

    // Insert at the head (like HASH_ADD)
    entry->next = dictionary;
    dictionary = entry;
}

DictEntry* find_entry(const char *word) {
    DictEntry *current = dictionary;
    while (current != NULL) {
        if (strcmp(current->key, word) == 0) {
            return current;
        }
        current = current->next;
    }

    fprintf(stderr, "Error: word '%s' not found in dictionary.\n", word);
    exit(2);
}

void print_dict() {
    DictEntry *current = dictionary;
    while (current != NULL) {
        printf("Key: %s, Line: 0x%04x\n", current->key, current->line_number);
        current = current->next;
    }
}


void usage() {
    fprintf(stderr, "Usage: ./xas file");
    exit(1);
}

void clean_line(char *line) {
    // exchange newline with terminating char
    size_t len = strlen(line);
    if (len > 0 && line[len-1] == '\n') {
        line[len-1] = '\0';
    }

    // remove all text after #
    char *comment = strchr(line, '#');
    if (comment) {
        *comment = '\0';
    }
}

int split_words(char *line, char words[][64]) {
    // split line by spaces into 4 words with max size of 64 bytes
    int count = 0;
    char *token = strtok(line, " \t\n\r");
    while (token != NULL && count < 4) {
        strncpy(words[count], token, 64 - 1);
        words[count][64 - 1] = '\0';  // null-terminate
        count++;
        token = strtok(NULL, " \t\n\r");
    }
    return count;  // number of words found
}

reg_t get_register(char *word) {
    // extract register from "%r3,"
    if (word[0] != '%' || word[1] != 'r') {
        // improper format
        fprintf(stderr, "Improper formatting of an assembly code\n");
        exit(2);
    }
    return (reg_t) atoi(&word[2]);
}

int get_number(const char *word) {
    // extract number from "$12"
    if (word[0] != '$') {
        // improper format
        fprintf(stderr, "Improper formatting of an assembly code\n");
        exit(2);
    }
    word++;  // skip '$'
    if (word[0] == '-') {
        word++;
        return -1 * atoi(word);
    }
    if (!isdigit(*word)) {
        // improper format
        fprintf(stderr, "Improper formatting of an assembly code\n");
        exit(2);
    }
    return atoi(word);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        usage();
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        usage();
    }
    const char *p;
    char *word;
    reg_t dst, src1, src2;
    uint16_t instruction, op1, op2, offset;
    char buffer[1024];

    int PC = 0x3000;
    // Pass 1: get the memory locations for all labels
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        clean_line(buffer);

        int len = strlen(buffer);
        if (buffer[0] == '\0' || strspn(buffer, " \t\n\r") == len) {
            // buffer is all whitespace, so skip
            continue;
        } else if (len > 0 && buffer[len - 1] == ':') {
            buffer[len - 1] = '\0';  // Remove colon
            word = strdup(buffer);  // Store label as heap-allocated string
            add_entry(word, PC);
            free(word);
        } else {
            // instruction
            PC += 1;
        }
    }
    print_dict();
    printf("===============\n");

    // Rewind to the start of the file
    rewind(fp);
    // Open file to save instructions to
    FILE *out = fopen("a.obj", "wb");
    if (!out) {
        perror("Failed to open a.obj");
        return 1;
    }
    uint16_t origin = htons(0x3000);
    fwrite(&origin, sizeof(origin), 1, out);

    // print everything for debugging
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        clean_line(buffer);
        printf("%s\n", buffer);
    }
    printf("===============\n");
    rewind(fp);

    // Pass 2: create x16 instructions
    PC = 0x3000;
    char words[4][64];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        clean_line(buffer);
        int num_words = split_words(buffer, words);

        if (num_words == 0) {
            // empty line
            continue;
        } else if (num_words == 1 && strstr(words[0], ":") != NULL) {
            // label
            continue;
        }
        PC += 1;
        if (strcmp(words[0], "add") == 0) {
            dst = get_register(words[1]);
            src1 = get_register(words[2]);
            if (words[3][0] == '$') {
                // add immediate
                op2 = get_number(words[3]);
                instruction = emit_add_imm(dst, src1, op2);
                printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
                instruction = htons(instruction);
                fwrite(&instruction, sizeof(instruction), 1, out);
            } else if (words[3][0] == '%') {
                // add from register
                src2 = get_register(words[3]);
                instruction = emit_add_reg(dst, src1, src2);
                printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
                instruction = htons(instruction);
                fwrite(&instruction, sizeof(instruction), 1, out);
            } else {
                // improper format
                fprintf(stderr, "Improper formatting of an assembly code\n");
                return 2;
            }
        } else if (strcmp(words[0], "and") == 0) {
            dst = get_register(words[1]);
            src1 = get_register(words[2]);
            if (words[3][0] == '$') {
                // and immediate
                op2 = get_number(words[3]);
                instruction = emit_and_imm(dst, src1, op2);
                printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
                instruction = htons(instruction);
                fwrite(&instruction, sizeof(instruction), 1, out);
            } else if (words[3][0] == '%') {
                // and from register
                src2 = get_register(words[3]);
                instruction = emit_and_reg(dst, src1, src2);
                printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
                instruction = htons(instruction);
                fwrite(&instruction, sizeof(instruction), 1, out);
            } else {
                // improper format
                fprintf(stderr, "Improper formatting of an assembly code\n");
                return 2;
            }
        } else if (strstr(words[0], "br") != NULL) {
            bool P = false, Z = false, N = false;
            if (strstr(words[0], "p")) {
                P = true;
            }
            if (strstr(words[0], "n")) {
                N = true;
            }
            if (strstr(words[0], "z")) {
                Z = true;
            }
            DictEntry *result = find_entry(words[1]);
            offset = result->line_number - PC;
            if (result) {
                instruction = emit_br(N, Z, P, offset);
                printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
                instruction = htons(instruction);
                fwrite(&instruction, sizeof(instruction), 1, out);
            } else {
                // improper format
                fprintf(stderr, "Improper formatting of an assembly code\n");
                return 2;
            }
        } else if (strstr(words[0], "jmp") != NULL) {
            dst = get_register(words[1]);
            instruction = emit_jmp(dst);
            printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
            instruction = htons(instruction);
            fwrite(&instruction, sizeof(instruction), 1, out);
        } else if (strstr(words[0], "jsrr") != NULL) {
            dst = get_register(words[1]);
            instruction = emit_jsrr(dst);
            printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
            instruction = htons(instruction);
            fwrite(&instruction, sizeof(instruction), 1, out);
        } else if (strstr(words[0], "jsr") != NULL) {
            DictEntry *result = find_entry(words[1]);
            offset = result->line_number - PC;
            if (result) {
                instruction = emit_jsr(offset);
                printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
                instruction = htons(instruction);
                fwrite(&instruction, sizeof(instruction), 1, out);
            } else {
                // improper format
                fprintf(stderr, "Improper formatting of an assembly code\n");
                return 2;
            }
        } else if (strstr(words[0], "ldi") != NULL) {
            dst = get_register(words[1]);
            DictEntry *result = find_entry(words[2]);
            offset = result->line_number - PC;
            if (result) {
                instruction = emit_ldi(dst, offset);
                printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
                instruction = htons(instruction);
                fwrite(&instruction, sizeof(instruction), 1, out);
            } else {
                // improper format
                fprintf(stderr, "Improper formatting of an assembly code\n");
                return 2;
            }
        } else if (strstr(words[0], "ldr") != NULL) {
            dst = get_register(words[1]);
            src1 = get_register(words[2]);
            op2 = get_number(words[3]);
            instruction = emit_ldr(dst, src1, op2);
            printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
            instruction = htons(instruction);
            fwrite(&instruction, sizeof(instruction), 1, out);
        } else if (strstr(words[0], "lea") != NULL) {
            dst = get_register(words[1]);
            DictEntry *result = find_entry(words[2]);
            offset = result->line_number - PC;
            if (result) {
                instruction = emit_lea(dst, offset);
                printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
                instruction = htons(instruction);
                fwrite(&instruction, sizeof(instruction), 1, out);
            } else {
                // improper format
                fprintf(stderr, "Improper formatting of an assembly code\n");
                return 2;
            }
        } else if (strstr(words[0], "ld") != NULL) {
            dst = get_register(words[1]);
            DictEntry *result = find_entry(words[2]);
            offset = result->line_number - PC;
            if (result) {
                instruction = emit_ld(dst, offset);
                printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
                instruction = htons(instruction);
                fwrite(&instruction, sizeof(instruction), 1, out);
            } else {
                // improper format
                fprintf(stderr, "Improper formatting of an assembly code\n");
                return 2;
            }
        } else if (strstr(words[0], "val") != NULL) {
            op1 = get_number(words[1]);
            instruction = op1;
            printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
            instruction = htons(instruction);
            fwrite(&instruction, sizeof(instruction), 1, out);
        } else if (strstr(words[0], "not") != NULL) {
            dst = get_register(words[1]);
            src1 = get_register(words[2]);
            instruction = emit_not(dst, src1);
            printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
            instruction = htons(instruction);
            fwrite(&instruction, sizeof(instruction), 1, out);
        } else if (strstr(words[0], "sti") != NULL) {
            dst = get_register(words[1]);
            DictEntry *result = find_entry(words[2]);
            offset = result->line_number - PC;
            if (result) {
                instruction = emit_sti(dst, offset);
                printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
                instruction = htons(instruction);
                fwrite(&instruction, sizeof(instruction), 1, out);
            } else {
                // improper format
                fprintf(stderr, "Improper formatting of an assembly code\n");
                return 2;
            }
        } else if (strstr(words[0], "str") != NULL) {
            dst = get_register(words[1]);
            src1 = get_register(words[2]);
            op2 = get_number(words[3]);
            instruction = emit_str(dst, src1, op2);
            printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
            instruction = htons(instruction);
            fwrite(&instruction, sizeof(instruction), 1, out);
        } else if (strstr(words[0], "st") != NULL) {
            dst = get_register(words[1]);
            DictEntry *result = find_entry(words[2]);
            offset = result->line_number - PC;
            if (result) {
                instruction = emit_st(dst, offset);
                printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
                instruction = htons(instruction);
                fwrite(&instruction, sizeof(instruction), 1, out);
            } else {
                // improper format
                fprintf(stderr, "Improper formatting of an assembly code\n");
                return 2;
            }
        } else if (strcmp(words[0], "getc") == 0) {
            instruction = emit_trap(TRAP_GETC);
            printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
            instruction = htons(instruction);
            fwrite(&instruction, sizeof(instruction), 1, out);
        } else if (strstr(words[0], "putc") != NULL) {
            instruction = emit_trap(TRAP_OUT);
            printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
            instruction = htons(instruction);
            fwrite(&instruction, sizeof(instruction), 1, out);
        } else if (strstr(words[0], "putsp") != NULL) {
            instruction = emit_trap(TRAP_PUTSP);
            printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
            instruction = htons(instruction);
            fwrite(&instruction, sizeof(instruction), 1, out);
        } else if (strstr(words[0], "puts") != NULL) {
            instruction = emit_trap(TRAP_PUTS);
            printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
            instruction = htons(instruction);
            fwrite(&instruction, sizeof(instruction), 1, out);
        } else if (strstr(words[0], "enter") != NULL) {
            instruction = emit_trap(TRAP_IN);
            printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
            instruction = htons(instruction);
            fwrite(&instruction, sizeof(instruction), 1, out);
        } else if (strstr(words[0], "halt") != NULL) {
            instruction = emit_trap(TRAP_HALT);
            printf("0x%04x: %.16b, %s\n", PC-1, instruction, buffer);
            instruction = htons(instruction);
            fwrite(&instruction, sizeof(instruction), 1, out);
        }
    }

    // End the program.
    fclose(fp);
    fclose(out);

    // free dictionary properly
    DictEntry *curr = dictionary;
    while (curr != NULL) {
        DictEntry *next = curr->next;
        free(curr);
        curr = next;
    }
    return 0;
}
