#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void partition(int a[], int n, int *k, int *p) {
    *k = 1;
    *p = n;
    int i = 1;
    
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

        int k, p;
        partition(a, high, &k, &p);

        // Move pivots to center
        int m = (p - k < high - p + 1) ? p - k : high - p + 1;
        for (int i = 0; i < m; i++) {
            swap(&a[k + i], &a[high - m + 1 + i]);
        }

        // Recursive sorts
        sort(a, low, k - 1);
        sort(a, high - p + k + 1, high);
    }
}

int main() {
    srand(time(NULL)); // seed for random number generation
    
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int a[n + 1]; // +1 because array indexing starts from 1 in the pseudo-code
    printf("Enter %d integers:\n", n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
    }

    // Sort the array
    sort(a, 1, n);

    printf("Sorted array:\n");
    for (int i = 1; i <= n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}