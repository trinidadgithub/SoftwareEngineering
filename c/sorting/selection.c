#include <stdio.h>

/**
 * Performs selection sort on an integer array.
 * 
 * @param a The array to be sorted
 * @param n The number of elements in the array
 */
void selectionSort(int a[], int n) {
    int i, j, k, temp;
    
    // Loop through each position in the array
    for (i = 0; i < n; i++) {
        // Assume the current index 'i' holds the smallest value
        k = i;
        
        // Search for the smallest element in the unsorted portion of the array
        for (j = i + 1; j < n; j++) {
            if (a[j] < a[k]) {
                // Update 'k' if a smaller element is found
                k = j;
            }
            // invariant: a[k] smallest of a[i..n]
        }
        
        // If the smallest element was not at index 'i', we need to swap
        if (k != i) {
            // Swap the smallest element found with the first element of the unsorted part
            temp = a[i];
            a[i] = a[k];
            a[k] = temp;
        }
        // invariant: At this point, a[i] is the smallest of a[i..n-1], and a[0..i] in final position
    }
}

int main() {
    // Example array to sort
    int array[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(array)/sizeof(array[0]);
    
    // Print the original array
    printf("Array before sorting:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    // Sort the array
    selectionSort(array, n);
    
    // Print the sorted array
    printf("Array after sorting:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    return 0;
}