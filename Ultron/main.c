#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ultron.h"


int* read_file(char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    int* buffer = malloc(sizeof(int) * 200);
    int i = 0;
    while (fscanf(file, "%d", &buffer[i]) != EOF) {
        fgetc(file);  // delete new line character
        i++;
    }
    *size = i;
    fclose(file);
    return buffer;
}
int* read_stdin(int length) {
    int* buffer = malloc(sizeof(int) * 200);
    int i = 0;
    for (int i = 0; i < length; i++) {
        if (scanf("%d", &buffer[i]) == EOF) {  // read and store
            exit(1);
        }
        if (getchar() == EOF) {  // delete new line character
            exit(1);
        }
    }
    return buffer;
}


int main(int argc, char** argv) {
    // Handle command line args
    if (argc <= 1) {
        printf("Usage: %s [-i instructions] [-f filename]\n", argv[0]);
    }
    int f_flag = 0;
    int i_flag = 0;
    char* filename = NULL;
    int instructions = 0;
    int opt;
    while ((opt = getopt(argc, argv, "i:f:")) != -1) {
        switch (opt) {
            case 'i':
                // only allow one flag
                i_flag = 1;
                instructions = atoi(optarg);
                break;
            case 'f':
                // only allow one flag
                f_flag = 1;
                filename = optarg;
                break;
            default:
                printf("Usage: %s [-i instructions] [-f filename]\n", argv[0]);
                return 1;
        }
    }
    if (i_flag != 0 && f_flag != 0) {
        printf("Usage: %s [-i instructions] [-f filename]\n", argv[0]);
        return 1;
    }

    int size;
    int* ptr;
    // Prepare input for ultron_load()
    if (i_flag == 0 && f_flag != 0) {
        // read from file
        ptr = read_file(filename, &size);
    }
    if (i_flag != 0 && f_flag == 0) {
        // read from stdout
        size = instructions;
        ptr = read_stdin(instructions);
    }
    // Debug. Check that size and ptr store values correctly.
    // printf("%d\n", size);
    // for (int i = 0; i < size; i++) {
    //     printf("%d\n", *(ptr+i));
    // }

    // Begin instructions for Ultron
    Ultron* ult;
    ultron_init(ult);
    ultron_load(ult, size, ptr);

    // call run() over all instructions or until program termination.
    int result;
    while (0 == 0) {
        result = ultron_run(ult);
        if (result != 10) {
            break;
        }
    }
    printf("Result: %d\n", result);
    ultron_dump(ult);
}
