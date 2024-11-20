#include <stdio.h>

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"

// Function to convert binary to decimal
int binaryToDecimal(long long binary) {
    int decimal = 0, base = 1, remainder;

    printf(CYAN "Starting conversion process...\n" RESET);
    printf(BLUE "Initial binary: " YELLOW "%lld\n" RESET, binary);
    printf(GREEN "Initial decimal: %d, base: %d\n" RESET, decimal, base);

    while (binary > 0) {
        remainder = binary % 10;
        printf(RED "Current binary digit (remainder): %d\n" RESET, remainder);
        
        decimal += remainder * base;
        printf(GREEN "Updated decimal: %d\n" RESET, decimal);
        
        binary /= 10;
        printf(BLUE "Shifted binary (binary / 10): %lld\n" RESET, binary);
        
        base *= 2;
        printf(YELLOW "Updated base (base * 2): %d\n" RESET, base);
    }

    printf(CYAN "Final decimal value: %d\n" RESET, decimal);
    return decimal;
}

int main() {
    long long binary;

    printf(YELLOW "Enter a binary number: " RESET);
    scanf("%lld", &binary);

    int decimal = binaryToDecimal(binary);
    printf(GREEN "The decimal equivalent is: %d\n" RESET, decimal);

    return 0;
}

