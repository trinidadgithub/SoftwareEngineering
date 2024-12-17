Improvements Overview

- Python to C port: Ported sorting prototypes from Python to C.
- REfactored Rendering logic: Centralized rendering logic instead of rendering within the sorting logic.
- Refactored sorting logic: Combined sorting logic into one C program as functions instead of separate programs.
- Graphics: Implemented Simple DirectMedia Layer library.
- Refactor Sorting Logic Dispatch: Move the if-else block for choosing the sorting algorithm into a separate function.
- Abstract SDL Initialization: Encapsulate SDL setup (window and renderer creation) into a helper function.
- Abstract Cleanup Logic: Combine the cleanups (cleanupFrames and cleanupFiles) into a dedicated cleanup function.
- Error-Handling Macros: Add simple macros for clean error handling to avoid repetitive if checks.
- Comments and Indentation: Ensure consistent and concise comments.
- Code Separation: Use function prototypes and declarations to separate main logic and helper functions for better modularity.

### The source code
[Sort Code](./sort-util.c):  The sorting utility with graphics rendering.

### How to run
This program was compile on Ubuntu.  To compile:

```bash
gcc sort-util.c -o sort-util `sdl2-config --cflags --libs` -lSDL2_image -g -O0
```

**Usage**
```bash
./sort-util
Usage: ./sort-util gen-gifs [quick-sort|merge-sort|heap-sort|selection-sort|insertion-sort|bubble-sort|shell-sort|three-way-quick-sort]
```
**Dependencies**  
You made need to install the following:
```bash
sudo apt-get install fastfetch
sudo apt-get install libsdl2-dev
sudo apt install ffmpeg
sudo apt-get install libsdl2-image-dev
sudo apt install gdb
sudo apt install valgrind
sudo apt install imagemagick-6.q16
```

**Trouble shooting**
- Use gdb to analyze core dumps and faulty logic
- Use valgrind to see how bad your wrote your code


