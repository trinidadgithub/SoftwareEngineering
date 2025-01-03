#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <complex.h>
#include <stdatomic.h>
#include <inttypes.h>  // For PRI* macros

int main() {
    // Primary Data Types
    int i = 42;
    char c = 'A';
    float f = 3.14f;
    double d = 2.718281828;
    _Bool b1 = true;
    void *ptr = NULL;

    // Modified Types
    short int si = -12345;
    long int li = 1234567890L;
    unsigned int ui = 4000000000U;

    // Derived Types
    int array[3] = {1, 2, 3};
    struct Point {
        int x;
        int y;
    } point = {10, 20};
    union Data {
        int i;
        float f;
    } data;
    data.i = 100;

    // Enumerated Type
    enum Color { RED, GREEN, BLUE };
    enum Color color = GREEN;

    // Standard Integer Types
    int8_t i8 = -128;
    uint8_t ui8 = 255;
    int16_t i16 = -32768;
    uint16_t ui16 = 65535;
    int32_t i32 = -2147483648;
    uint32_t ui32 = 4294967295U;
    int64_t i64 = -9223372036854775807LL;
    uint64_t ui64 = 18446744073709551615ULL;

    // Floating-Point Types
    float _Complex fc = 1.0 + 2.0 * I;
    double _Complex dc = 2.0 + 3.0 * I;
    long double _Complex ldc = 3.0 + 4.0 * I;

    // Atomic Types
    _Atomic int atomic_int = 42;

    // Boolean Type
    bool b2 = true;

    // Printing Values
    printf("Primary Data Types:\n");
    printf("int: %d\n", i);
    printf("char: %c\n", c);
    printf("float: %.2f\n", f);
    printf("double: %.9f\n", d);
    printf("_Bool: %d\n", b1);

    printf("\nModified Types:\n");
    printf("short int: %d\n", si);
    printf("long int: %ld\n", li);
    printf("unsigned int: %u\n", ui);

    printf("\nDerived Types:\n");
    printf("array: %d, %d, %d\n", array[0], array[1], array[2]);
    printf("struct Point: (%d, %d)\n", point.x, point.y);
    printf("union Data (int): %d\n", data.i);

    printf("\nEnumerated Type:\n");
    printf("Color: %d\n", color);

    printf("\nStandard Integer Types:\n");
    printf("int8_t: %" PRId8 "\n", i8);
    printf("uint8_t: %" PRIu8 "\n", ui8);
    printf("int16_t: %" PRId16 "\n", i16);
    printf("uint16_t: %" PRIu16 "\n", ui16);
    printf("int32_t: %" PRId32 "\n", i32);
    printf("uint32_t: %" PRIu32 "\n", ui32);
    printf("int64_t: %" PRId64 "\n", i64);
    printf("uint64_t: %" PRIu64 "\n", ui64);

    printf("\nFloating-Point Types:\n");
    printf("float _Complex: %.2f + %.2fi\n", crealf(fc), cimagf(fc));
    printf("double _Complex: %.2f + %.2fi\n", creal(dc), cimag(dc));
    printf("long double _Complex: %.2Lf + %.2Lfi\n", creall(ldc), cimagl(ldc));

    printf("\nAtomic Types:\n");
    printf("_Atomic int: %d\n", atomic_int);

    printf("\nBoolean Type:\n");
    printf("bool: %d\n", b2);

    return 0;
}

