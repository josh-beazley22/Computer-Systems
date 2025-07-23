#include <stdio.h>
#include <string.h>

void find_size(char* c) {
    printf("size of c = %ld\n", sizeof(c)); // sizeof now returns size of pointer.
    printf("c = %s\n", c); // prints chars until a terminating \0
}

int main() {
    // Strings store arrays of chars terminated by null char \0
    // name is a pointer to the first char
    char name[] = "Doug Doug";
    printf("name=%s\n", name);
    printf("size of %ld\n", sizeof(name));

    find_size(name);
    char* t = name + 1; // store pointer 1 position later in memory.
    printf("t name = %s\n", t);

    // Segmentation fault occurs when you access memory you're not allowed to.
    *t = 'Z';
    printf("t = %s\n", t);

    // strlen() doesn't include null character in the length.
    int len = strlen(name);
    printf("len of name = %d\n", len);
    int tlen = strlen(t);
    printf("len of t = %d\n", tlen);

    char* u = strdup(t); // copy function
    u += 1;
    printf("copy of t=%s\n", u);

    // Read input
    printf("Enter your name: ");
    char q[50];
    if (fgets(q, sizeof name, stdin) == NULL) {
        printf("ERROR\n");
    } else {
        // Remove trailing new line character
        q[strlen(q) - 1] = '\0';
        printf("Your name is %s\n", q);
    }
}