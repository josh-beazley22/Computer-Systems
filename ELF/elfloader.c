#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <elf.h>


void print_usage() {
    fprintf(stderr, "Usage: ./elfloader file arg1 arg2\n");
}

int is_ELF(FILE *elf_file) {
    unsigned char buffer[4];
    size_t bytes_read = fread(buffer, 1, 4, elf_file);
    if (bytes_read != 4) {
        return 1;
    }
    if (buffer[0] == 0x7F && buffer[1] == 'E' &&
        buffer[2] == 'L' && buffer[3] == 'F') {
        return 0;
    }
    return 1;
}


int main(int argc, char* argv[]) {
    if (argc != 4) {
        print_usage();
        return 1;
    }
    char *elf_file = argv[1];
    int arg1 = atoi(argv[2]);
    int arg2 = atoi(argv[3]);

    // Read file
    FILE *elf_pointer = fopen(elf_file, "rb");
    if (elf_pointer == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", elf_file);
        return 2;
    }
    // check if this is an elf file using the magic chars 0x7F and 'ELF'
    if (is_ELF(elf_pointer) != 0) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", elf_file);
        fclose(elf_pointer);
        return 2;
    }
    // Read the program e_entry
    fseek(elf_pointer, 24, SEEK_SET);
    int e_entry;
    if (fread(&e_entry, sizeof(e_entry), 1, elf_pointer) != 1) {
        fprintf(stderr, "Error: Cannot read elf e_entry from file '%s'\n", elf_file);
        fclose(elf_pointer);
        return 2;
    }

    // Read the program e_phoff
    fseek(elf_pointer, 32, SEEK_SET);
    int e_phoff;
    if (fread(&e_phoff, sizeof(e_phoff), 1, elf_pointer) != 1) {
        fprintf(stderr, "Error: Cannot read elf e_phoff from file '%s'\n", elf_file);
        fclose(elf_pointer);
        return 2;
    }

    // Read the program e_phnum
    fseek(elf_pointer, 56, SEEK_SET);
    uint16_t e_phnum;
    if (fread(&e_phnum, sizeof(e_phnum), 1, elf_pointer) != 1) {
        fprintf(stderr, "Error: Cannot read elf e_phnum from file '%s'\n", elf_file);
        fclose(elf_pointer);
        return 2;
    }
    // return error if there are 0 program headers.
    if (e_phnum == 0) {
        fprintf(stderr, "Error: There are no program headers for file '%s'\n", elf_file);
        fclose(elf_pointer);
        return 3;
    }

    // Grab all the variables I need from the program header file
    fseek(elf_pointer, e_phoff+8, SEEK_SET);
    int offset;
    if (fread(&offset, sizeof(offset), 1, elf_pointer) != 1) {
        fprintf(stderr, "Error: Cannot read from program header file\n");
        fclose(elf_pointer);
        return 3;
    }

    // TODO: check if program header flag has PF_X set
    fseek(elf_pointer, e_phoff+60, SEEK_SET);
    uint32_t p_flags;
    if (fread(&p_flags, sizeof(p_flags), 1, elf_pointer) != 1) {
        fprintf(stderr, "Error: Cannot read from program header file\n");
        fclose(elf_pointer);
        return 3;
    }
    printf("  p_flags = 0x%x\n", p_flags);
    if (p_flags != 0x04) {
        fprintf(stderr, "Error: This is not executable code.\n");
        fclose(elf_pointer  );
        return 2;
    }

    fseek(elf_pointer, e_phoff+16, SEEK_SET);
    int virtual_addy;
    if (fread(&virtual_addy, sizeof(virtual_addy), 1, elf_pointer) != 1) {
        fprintf(stderr, "Error: Cannot read from program header file\n");
        fclose(elf_pointer);
        return 3;
    }

    fseek(elf_pointer, e_phoff+32, SEEK_SET);
    int filesz;
    if (fread(&filesz, sizeof(filesz), 1, elf_pointer) != 1) {
        fprintf(stderr, "Error: Cannot read from program header file\n");
        fclose(elf_pointer);
        return 3;
    }

    fseek(elf_pointer, e_phoff+40, SEEK_SET);
    int memsz;
    if (fread(&memsz, sizeof(memsz), 1, elf_pointer) != 1) {
        fprintf(stderr, "Error: Cannot read from program header file\n");
        fclose(elf_pointer);
        return 3;
    }

    void* codep = mmap(NULL, memsz, PROT_READ | PROT_WRITE | PROT_EXEC,
    		MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    if (codep == MAP_FAILED) {
        fprintf(stderr, "Error: Failed to allocate memory of executable code for '%s'\n", elf_file);
        fclose(elf_pointer);
        return 2;
    }
    // move to the offset
    if (fseek(elf_pointer, offset, SEEK_SET) != 0) {
        perror("fseek");
        munmap(codep, memsz);
        fclose(elf_pointer);
        return 3;
    }

    // read executable code and store into codep
    if (fread(codep, 1, filesz, elf_pointer) != filesz) {
        perror("fread");
        munmap(codep, memsz);
        fclose(elf_pointer);
        return 3;
    }

    // mainfunc is a pointer to a function that takes two ints 
    // and returns an unsigned int 
    unsigned int (*mainfunc)(int, int) = (void*)((char*)codep + (e_entry - virtual_addy));

    // Call the main func in the ELF program
    int result = (*mainfunc)(arg1, arg2);
    printf("Answer=%d\n", result);

    // Debug
    // printf("0x%x\n", e_entry);
    // printf("%d\n", e_phoff);
    // printf("%d\n", e_phnum);
    // printf("0x%x\n", offset);
    // printf("0x%x\n", virtual_addy);
    // printf("0x%x\n", filesz);
    // printf("0x%x\n", memsz);

    munmap(codep, memsz);
    fclose(elf_pointer);
    return 0;
}
