#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to swap two elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function for quicksort
int partition(int arr[], int low, int high) {
    // Choose pivot randomly and place it at the beginning
    srand(time(NULL));
    int random = low + rand() % (high - low + 1);
    swap(&arr[low], &arr[random]);
    
    int pivot = arr[low];
    int i = low + 1;
    
    // 2-way partition
    for (int j = low + 1; j <= high; j++) {
        if (arr[j] < pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    
    swap(&arr[low], &arr[i - 1]);
    return i - 1;
}

// QuickSort function
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        // Partition index
        int pi = partition(arr, low, high);
        
        // Recursively sort elements before and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to print an array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Main function to test the quicksort
int main() {
    int arr[] = {0, 12, 11, 13, 5, 6, 7};
    int n = sizeof(arr)/sizeof(arr[0]);
    printf("Unsorted array: \n");
    printArray(arr, n);
    
    quickSort(arr, 0, n - 1);
    
    printf("Sorted array: \n");
    printArray(arr, n);
    return 0;
}