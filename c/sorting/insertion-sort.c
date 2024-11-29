#include <stdio.h>

// Function to swap two numbers
void swap(int *xp, int *yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Insertion sort function according to provided pseudo code
void insertionSort(int arr[], int n) {
    int i, k;
    for (i = 1; i < n; i++) {  
        for (k = i; k > 0 && arr[k] < arr[k-1]; k--) {
            swap(&arr[k], &arr[k-1]);
        }
        // invariant: At this point, arr[0..i] is now sorted
    }
}

// Function to print an array 
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Main function to test the implementation
int main() {
    int arr[] = {12, 11, 13, 5, 6, 15, 3};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("Unsorted array: \n");
    printArray(arr, n);
    
    insertionSort(arr, n);
    
    printf("Sorted array: \n");
    printArray(arr, n);
    
    return 0;
}