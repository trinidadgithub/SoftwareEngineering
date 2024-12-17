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