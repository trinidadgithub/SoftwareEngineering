#!/bin/bash
# Build the test program
gcc -o test_myshell test_myshell.c myshell.c -DTEST_MODE -I/usr/include -L/usr/lib/x86_64-linux-gnu -L/lib/x86_64-linux-gnu -lcheck -lsubunit -lrt -lpthread -lm
