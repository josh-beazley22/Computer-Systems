#include <stdio.h>

int main() {
    int bits;
    printf("How many bits? ");
    scanf("%d", &bits);

    int bytes;
    printf("Page size in bytes. ");
    scanf("%d", &bytes);

    int var1 = (int) bytes/bits;
    int var2 = (int) bytes*bits/bytes;
    printf("Page bits=%d, offset bits=%d\n", var1, var2);

    int hex;
    printf("Enter address in hex: ");
    scanf("%d", &hex);

    int n = var1 + var2;
    int hex1 = hex >> (n-var1);
    int hex2 = hex && (0xf << var2);
    printf("Page No=%d, Offset=%d\n", hex1, hex2);
}