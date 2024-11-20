#include <stdio.h>

// Function to convert binary to decimal using basic features
int binaryToDecimal(long long binary) {
    int decimal = 0, base = 1, remainder;
    
    printf("Starting conversion process...\n");
    printf("Initial binary: %lld\n", binary);
    printf("Initial decimal: %d, base: %d\n", decimal, base);

    while (binary > 0) {
        remainder = binary % 10;
        printf("Current binary digit (remainder): %d\n", remainder);
        
        decimal += remainder * base;
        printf("Updated decimal: %d\n", decimal);
        
        binary /= 10;
        printf("Shifted binary (binary / 10): %lld\n", binary);
        
        base *= 2;
        printf("Updated base (base * 2): %d\n", base);
    }
    
    printf("Final decimal value: %d\n", decimal);
    return decimal;
}

int main() {
    long long binary;
    
    printf("Enter a binary number: ");
    scanf("%lld", &binary);
    
    int decimal = binaryToDecimal(binary);
    printf("The decimal equivalent is: %d\n", decimal);
    
    return 0;
}

