### A collection of c programs, I have built a number of them over the years and this is their new home

[binary-to-decimal](./binary-to-decimal):  A study in bit shifting for converting binary to decimal  
[db](./db):  A small database built in c, based on the sqllite architecture.  
[sorting](./sorting/): A collection of classic sorting algorithms.  More will be added as time permits.   
[url-download](./sorting/): This is a basic implementation of a command-line tool that downloads web content using libcurl and saves it to a file. The filename is derived from the URL provided, ensuring a unique name for each download.  
[c-types](./c-types): A study on the various built in types available in C.  
[MemoryAccessPatternAnalyzer](./MemoryAccessPatternAnalyzer): A study on virtual memory analysis on local.  
[curl](./curl): An excercise on building your own curl program.  
[db](./db):  Build your own database.   
[sslcomms](./sslcomms): Building two process threads in C that communicate over SSL.  

---

Here are additional ideas to work on.

1. Understanding String Manipulation

    Exercise: Write a function that counts the number of directories in the PATH string. Use strtok to split the PATH into parts and count them.
    Goal: Get familiar with splitting strings and handling null-terminated arrays.


2. Dynamic Memory Management

    Exercise: Modify the find_in_path function to return dynamically allocated memory instead of using a static buffer. Make sure to free this memory when it's no longer needed.
    Goal: Practice dynamic memory allocation (malloc, free) and understand memory lifecycle.


3. Error Handling

    Exercise: Add error handling in the find_in_path function for cases where strdup might return NULL due to allocation failure.
    Goal: Learn to manage and respond to memory allocation errors.


4. String Parsing

    Exercise: Implement a function that checks if a command has any arguments (e.g., echo hello). This should work for any command, not just built-ins.
    Goal: Enhance your understanding of parsing command-line arguments.


5. Expanding Built-in Commands

    Exercise: Add a new built-in command like pwd (print working directory). Implement is_builtin and command execution for this new command.
    Goal: Practice extending the shell with new functionalities.


6. File Permissions

    Exercise: Write a helper function to check if a file is executable in a more sophisticated way than just using access(). Perhaps by checking file permissions directly (stat function).
    Goal: Learn how to interact with file system attributes.


7. Command Completion

    Exercise: Implement a simple version of command completion. When you type part of a command followed by a special character (e.g., Tab), suggest or complete the command if it's found in PATH or is a built-in.
    Goal: Understand how command line interfaces can provide user assistance.


8. Environment Variables

    Exercise: Implement a basic set command to change environment variables (like PATH) at runtime and immediately reflect these changes in your shell's behavior.
    Goal: Learn how environment variables affect program execution.


9. Performance Tuning

    Exercise: Optimize the find_in_path function by caching results for commands that have already been searched for. Use a simple structure or array to store command-path pairs.
    Goal: Understand caching and performance optimization in simple scenarios.


10. User Feedback

    Exercise: Add color to your shell output (using escape sequences for terminals) to differentiate between types of messages (like error messages, success messages, built-in vs. external commands).
    Goal: Enhance user interaction by making output more readable and informative.


11. Testing

    Exercise: Write a script or set of commands to automatically test all your shell's functionalities, including edge cases like commands not in PATH, built-ins, and commands with arguments.


