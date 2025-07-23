#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {

    char number[200];
    printf("Enter number: ");
    fgets(number, 200, stdin);
    int n = atoi(number);

    char word[200];
    printf("Enter word: ");
    fgets(word, 200, stdin);
    // Remove trailing new line character.
    word[strlen(word)-1] = '\0';

    if (n == 1) {
        printf("%d %s\n", n, word);
    }
    else {
        printf("%d %ss\n", n, word);
    }
}