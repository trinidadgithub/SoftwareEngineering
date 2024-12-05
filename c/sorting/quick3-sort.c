#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void partition(int a[], int n, int *k, int *p) {
    *k = 0;
    *p = n;
    int i = 0;
    
    while (i < *p) {
        if (a[i] < a[n]) {
            swap(&a[i++], &a[(*k)++]);
        } else if (a[i] == a[n]) {
            swap(&a[i], &a[--(*p)]);
        } else {
            i++;
        }
    }
}

void sort(int a[], int low, int high) {
    if (low < high) {
        // Choose pivot by swapping last element with a random element
        int pivot_index = low + rand() % (high - low + 1);
        swap(&a[pivot_index], &a[high]);

        int k = low, p = high;
        int i = low;
        
        while (i < p) {
            if (a[i] < a[high]) {
                swap(&a[i++], &a[k++]);
            } else if (a[i] == a[high]) {
                swap(&a[i], &a[--p]);
            } else {
                i++;
            }
        }

        // Move pivots to center
        int m = (p - k < high - p + 1) ? p - k : high - p + 1;
        for (int j = 0; j < m; j++) {
            swap(&a[k + j], &a[high - m + 1 + j]);
        }

        // Recursive sorts
        sort(a, low, k - 1);
        sort(a, high - p + k + 1, high);
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int arr[] = {12, 34, 54, 12, 2, 3, 54};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array before sorting: \n");
    printArray(arr, n);

    srand(time(NULL)); // Seed the random number generator
    sort(arr, 0, n - 1);  // Now use 0-based indexing

    printf("Array after sorting: \n");
    printArray(arr, n);

    return 0;
}