#include <stdio.h>

// Function to swap two elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to heapify a subtree rooted with node i which is an index in arr[].
// n is size of heap
void sink(int arr[], int i, int n) {
    int lc = 2 * i; // Left child index
    if (lc > n) return; // No children
    
    int rc = lc + 1; // Right child index
    int mc = (rc > n || arr[lc] > arr[rc]) ? lc : rc; // Max child index
    
    if (arr[i] >= arr[mc]) return; // If heap property is maintained, return
    
    swap(&arr[i], &arr[mc]);
    sink(arr, mc, n); // Recursively apply sink to the subtree
}

// Main function to sort an array of given size
void heapSort(int arr[], int n) {
    // Build heap (rearrange array)
    for (int i = n / 2; i >= 1; i--) {
        sink(arr, i, n);
    }
    
    // One by one extract an element from heap
    for (int i = n; i > 1; i--) {
        // Move current root to end
        swap(&arr[1], &arr[i]);
        // call sink on the reduced heap
        sink(arr, 1, i - 1);
    }
}

// A utility function to print an array of size n
void printArray(int arr[], int size) {
    for (int i = 1; i <= size; ++i)
        printf("%d ", arr[i]);
    printf("\n");
}

// Driver program to test above functions
int main() {
    int arr[] = {0, 12, 11, 13, 5, 6, 7}; // 0 is added to make array 1-indexed
    int n = sizeof(arr) / sizeof(arr[0]) - 1; // -1 because array is 1-indexed
    
    printf("Unsorted array: \n");
    printArray(arr, n);

    heapSort(arr, n);

    printf("Sorted array: \n");
    printArray(arr, n);
    return 0;
}