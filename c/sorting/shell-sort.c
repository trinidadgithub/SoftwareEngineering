#include <stdio.h>

// Function to perform insertion sort for h-sorting
void insertionSort(int arr[], int n, int h) {
    for (int i = h; i < n; i++) {
        int temp = arr[i];
        int j = i;
        // Move elements of arr[0..i-1], that are greater than temp,
        // to one position ahead of their current position
        while (j >= h && arr[j - h] > temp) {
            arr[j] = arr[j - h];
            j -= h;
        }
        arr[j] = temp;
    }
}

// Shell Sort function
void shellSort(int arr[], int n) {
    // Start with a big gap, then reduce the gap
    for (int h = 1; h < n; h = 3 * h + 1);

    // Do a h-sort for h values from largest to smallest
    for (; h > 0; h /= 3) {
        for (int i = 0; i < h; i++) {
            // Apply insertion sort for each h-sub-array
            insertionSort(arr + i, n - i, h);
        }
    }
}

// Utility function to print an array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Driver program to test above functions
int main() {
    int arr[] = {12, 34, 54, 2, 3};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array before sorting: \n");
    printArray(arr, n);

    shellSort(arr, n);

    printf("Array after sorting: \n");
    printArray(arr, n);
    return 0;
}