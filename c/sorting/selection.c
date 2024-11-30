#include <stdio.h>

void selectionSort(int a[], int n) {
    int i, j, k, temp;
    
    for (i = 0; i < n; i++) {
        k = i;
        // Find the smallest element in the unsorted part of the array
        for (j = i + 1; j < n; j++) {
            if (a[j] < a[k]) {
                k = j;
            }
        }
        // Swap the found minimum element with the first element of the unsorted part
        if (k != i) {
            temp = a[i];
            a[i] = a[k];
            a[k] = temp;
        }
    }
}

int main() {
    int array[] = {11, 64, 34, 25, 12, 22, 25, 11, 90};
    int n = sizeof(array)/sizeof(array[0]);
    
    printf("Array before sorting:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    selectionSort(array, n);
    
    printf("Array after sorting:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    return 0;
}