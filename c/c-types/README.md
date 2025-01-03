📄 ##Print Types in C Program  
📋 Description

The Print Types program is a comprehensive C application that demonstrates the usage of all major C data types. It covers primary, modified, derived, standard integer, floating-point, complex, atomic, and boolean types, ensuring platform-independent format specifiers.

This program serves as a reference for C developers looking to understand the various data types available in the language and how to print them correctly across different platforms.
🛠️ Features

    Demonstrates usage of primary C data types (int, char, float, double, etc.)
    Covers standard integer types from <stdint.h>
    Shows complex numbers from <complex.h>
    Utilizes atomic types from <stdatomic.h>
    Includes enumerated types and derived types (structs, unions, arrays)
    Uses portable format specifiers from <inttypes.h> to avoid warnings

🗂️ Project Structure

printTypes/
│
├── printTypes.c    # Main C program
└── README.md       # Documentation

🚀 How to Run the Program
Prerequisites

Ensure you have a C compiler installed. For example:

    GCC (GNU Compiler Collection)
    Clang

Steps to Compile and Run

    Clone the repository:

git clone https://github.com/yourusername/printTypes.git
cd printTypes

Compile the program:

gcc printTypes.c -o printTypes -lm

Run the executable:

    ./printTypes

📖 Usage Example

Primary Data Types:
int: 42
char: A
float: 3.14
double: 2.718281828
_Bool: 1

Modified Types:
short int: -12345
long int: 1234567890
unsigned int: 4000000000

Derived Types:
array: 1, 2, 3
struct Point: (10, 20)
union Data (int): 100

Enumerated Type:
Color: 1

📚 Data Types Demonstrated
| Category |	Types Demonstrated |
|----------|-----------------------|
| *Primary Types* | *int, char, float, double, void, _Bool* |
| *Modified Types* |*short int, long int, unsigned int* |
| *Derived Types* |	*Arrays, Structures, Unions* |
| *Enumerated Types* | *enum* |
| *Standard Integer Types* | *int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t* |
| *Floating-Point Types* | *float _Complex, double _Complex, long double _Complex* |
| *Atomic Types* | *_Atomic int* |
| *Boolean Types* |*bool* |

📦 Dependencies

    C Standard Library (<stdio.h>, <stdbool.h>, <stdint.h>, etc.)
    Complex Number Library (<complex.h>)
    Atomic Operations (<stdatomic.h>)
    Portable Format Specifiers (<inttypes.h>)

📝 License

This project is licensed under the MIT License. See the LICENSE file for details.
🤝 Contributing

Contributions are welcome! Feel free to submit a pull request or open an issue for suggestions and improvements.
📧 Contact

Author: T Marroquin, Jr.
GitHub: github.com/trinidadgithub
