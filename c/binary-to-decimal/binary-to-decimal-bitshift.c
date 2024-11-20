#include <stdio.h>
#include <string.h>
#include <math.h>

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

// Function to convert binary (as string) to decimal using bit shifting
int binaryToDecimalBitShift(const char *binary) {
    int decimal = 0;
    int length = strlen(binary);

    printf(YELLOW "Starting bit-shift conversion process...\n" RESET);

    for (int i = 0; i < length; i++) {
        int bit = binary[length - 1 - i] - '0'; // Get the bit (0 or 1) from the string
        printf(MAGENTA "Bit at position %d: " RED "%d\n" RESET, i, bit);

        if (bit == 1) {
            decimal |= (1 << i); // Set the corresponding bit in the decimal value
            printf(GREEN "Updated decimal: %d\n" RESET, decimal);
        }
    }

    printf(YELLOW "Final decimal value: " GREEN "%d\n" RESET, decimal);
    return decimal;
}

int main() {
    char binary[65]; // Maximum 64-bit binary number + null terminator

    printf("Enter a binary number: ");
    scanf("%64s", binary); // Read binary as string

    int decimal = binaryToDecimalBitShift(binary);
    printf(YELLOW "The decimal equivalent is: " GREEN "%d\n" RESET, decimal);

    return 0;
}

