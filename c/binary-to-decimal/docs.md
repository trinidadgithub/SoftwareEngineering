### Binary to Decimal Conversion in C

Introduction:  This project demonstrates three different approaches to converting binary numbers into their decimal equivalents using the C programming language. Each version of the program builds upon the previous one, showcasing techniques such as basic arithmetic, debugging, color-coded output, and bit shifting.
Project Structure

The project folder contains the following files:

    binary-to-decimal.c
    A basic implementation that converts a binary number to its decimal equivalent using decimal arithmetic.

    binary_to_decimal_debug.c
    An enhanced version of the basic implementation with debug information and color-coded output for better traceability.

    binary_to_decimal_bitshift.c
    A more advanced implementation that performs the conversion using bitwise operations and bit shifting, with color-coded output.

How to Compile and Run  
Prerequisites

    A C compiler, such as GCC.

Compilation

Use the following command to compile any of the programs:
```
gcc <filename>.c -o <output_name>
```
For example:
```
gcc binary_to_decimal.c -o binary_to_decimal
gcc binary_to_decimal_debug.c -o binary_to_decimal_debug
gcc binary_to_decimal_bitshift.c -o binary_to_decimal_bitshift
```
Running the Program

Run the compiled program using:
```
./<output_name>
```
For example:
```
./binary_to_decimal
```
Program Details  
### 1. binary_to_decimal.c

Description:
This is a basic implementation that uses decimal arithmetic to process binary input digit by digit. The binary number is read as a numeric value, and each digit is processed by dividing the number by 10 to extract individual bits.

Features:

    Simple and straightforward logic.
    Supports large binary numbers within the limits of long long.

### 2. binary_to_decimal_debug.c

Description:
This program adds debugging statements and ANSI color-coded output to the basic implementation. It provides step-by-step insights into the conversion process, highlighting the extracted digits, updated decimal values, and base calculations.

Features:

    Debug messages for every step of the conversion.
    Color-coded output for better readability:
        Red: Current binary digit being processed.
        Green: Updated decimal value.
        Cyan: Base multiplier.
    User-friendly and visually appealing.

### 3. binary_to_decimal_bitshift.c

Description:
This program performs the conversion using bitwise operations and bit shifting. It reads the binary number as a string and processes it one bit at a time. The use of bitwise shifts (<< and >>) highlights a lower-level approach to binary manipulation.

Features:

    Processes binary numbers as strings for accurate handling.
    Bitwise operations (<<, >>, |) replace arithmetic operations.
    Color-coded output:
        Red: Current bit value.
        Green: Updated decimal value.
        Magenta: Bit position.
    Supports up to 64-bit binary numbers.

### Example Outputs
**binary_to_decimal.c**
```
Input: 1011

Output: The decimal equivalent is 11
```
----------------------------------
**binary_to_decimal_debug.c**

```
Input: 1011

Output:

Starting conversion process...
Initial binary: 1011
Initial decimal: 0, base: 1
Current binary digit (remainder): 1
Updated decimal: 1
Shifted binary (binary / 10): 101
Updated base (base * 2): 2
Current binary digit (remainder): 1
Updated decimal: 3
Shifted binary (binary / 10): 10
Updated base (base * 2): 4
Current binary digit (remainder): 0
Updated decimal: 3
Shifted binary (binary / 10): 1
Updated base (base * 2): 8
Current binary digit (remainder): 1
Updated decimal: 11
Shifted binary (binary / 10): 0
Updated base (base * 2): 16
Final decimal value: 11
The decimal equivalent is: 11
```

------------------------------

**binary_to_decimal_bitshift.c**
```
Input: 1011

Output:

Starting bit-shift conversion process...
Bit at position 0: 1
Updated decimal: 1
Bit at position 1: 1
Updated decimal: 3
Bit at position 2: 0
Bit at position 3: 1
Updated decimal: 11
Final decimal value: 11
The decimal equivalent is: 11
```

----------------------

### Learning Objectives

    Understand different approaches to binary-to-decimal conversion.
    Gain insights into debugging and color-coded output for better program traceability.
    Explore bitwise operations and their use in processing binary data.

### License

This project is licensed under the MIT License.