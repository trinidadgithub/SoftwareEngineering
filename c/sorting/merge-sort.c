#include <stdio.h>
#include <stdlib.h>

// Function to merge two sorted subarrays
void merge(int *a, int low, int mid, int high) {
    int *b = (int *)malloc((mid - low + 1) * sizeof(int)); // Temporary array for left half
    int i, j, k;
    if (b == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Copy data to temp arrays b[] 
    for (i = 0; i < mid - low + 1; i++)
        b[i] = a[low + i];

    i = 0; // Initial index of first subarray
    j = mid + 1; // Initial index of second subarray
    k = low; // Initial index of merged subarray

    while (i < mid - low + 1 && j <= high) {
        if (b[i] <= a[j]) {
            a[k] = b[i];
            i++;
        } else {
            a[k] = a[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of b[], if there are any
    while (i < mid - low + 1) {
        a[k] = b[i];
        i++;
        k++;
    }

    free(b);
}

// Main function that sorts arr[l..r] using merge()
void mergeSort(int *a, int l, int r) {
    if (l < r) {
        // Find the middle point
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(a, l, m);
        mergeSort(a, m + 1, r);

        // Merge the sorted halves
        merge(a, l, m, r);
    }
}

// A utility function to print an array of size n
void printArray(int A[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}

// Driver code
int main() {
    int arr[] = {12, 11, 13, 5, 6, 7};
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    printf("Given array is \n");
    printArray(arr, arr_size);

    mergeSort(arr, 0, arr_size - 1);

    printf("\nSorted array is \n");
    printArray(arr, arr_size);
    return 0;
}