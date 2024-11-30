### The bubble sort

[Resource Link](https://www.toptal.com/developers/sorting-algorithms/bubble-sort): "Bubble sort has many of the same properties as insertion sort, but has slightly higher overhead. In the case of nearly sorted data, bubble sort takes O(n) time, but requires at least 2 passes through the data (whereas insertion sort requires something more like 1 pass)." as quoted from the source.

[Bubble Sort](bubble-sort.c):  The source code.  

![Bubble Sort Animation](./animators/bubble_sort.gif)

Outer Loop:
        This loop iterates through the array, narrowing down the unsorted portion. For each iteration i, the largest i elements will have reached their final sorted positions at the end of the array.

Inner Loop:
        For each pass, the inner loop compares adjacent elements, arr[j] and arr[j-1], from the end of the unsorted portion towards the beginning.
        If the current element (arr[j]) is smaller than the previous element (arr[j-1]), they are swapped.

Swapped Flag:
        A boolean flag swapped is introduced to track whether any swaps occur during the inner loop.
        If no swaps are performed, the array is already sorted, and the algorithm can terminate early by breaking out of the loop.

Invariant:
        After each outer loop iteration, the subarray arr[0..i] will be sorted and in its final position.

Early Termination:
        The swapped flag optimizes the algorithm by halting the process if the array becomes sorted before all outer loop iterations are completed.

Conclusion:

Time Complexity:
        Best case (already sorted): O(n) (due to the early termination condition).
        Worst/average case: O(n²).
    Space Complexity: O(1) (in-place sorting).
    Stable Sort: Maintains the relative order of equal elements.

The explanation above highlights the process and efficiency considerations for implementing Bubble Sort.

---
### The insertion Sort
[Resource Link](https://www.toptal.com/developers/sorting-algorithms/insertion-sort): 
"Although it is one of the elementary sorting algorithms with O(n2) worst-case time, insertion sort is the algorithm of choice either when the data is nearly sorted (because it is adaptive) or when the problem size is small (because it has low overhead).

For these reasons, and because it is also stable, insertion sort is often used as the recursive base case (when the problem size is small) for higher overhead divide-and-conquer sorting algorithms, such as merge sort or quick sort." as quoted from the source.  

[Insertion Sort](./insertion-sort.c):  The source code.  

![Insertion Sort Annimation](./animators/insertion_sort.gif)  


This C program implements the Insertion Sort algorithm:

- insertionSort: This function takes an array and its size, then sorts the array in-place using the insertion sort method. It iterates through the array starting from the second element, comparing it with the elements before it, moving larger elements one position ahead to insert the current element in its correct position.  
- printArray: A utility function to print the array contents.  
- main: Demonstrates how to use the insertion sort function by initializing an unsorted array, sorting it, and then printing both the unsorted and sorted arrays to show the effect of the sorting algorithm.  


This code follows the logic of insertion sort where you insert each element into its correct position in the sorted portion of the array.
---
### The selection sort
[Resource Link](https://www.toptal.com/developers/sorting-algorithms/selection-sort): "From the comparions presented here, one might conclude that selection sort should never be used. It does not adapt to the data in any way (notice that the four animations above run in lock step), so its runtime is always quadratic.  

However, selection sort has the property of minimizing the number of swaps. In applications where the cost of swapping items is high, selection sort very well may be the algorithm of choice."  as quoted from the source. 

[Selection Sort](./selection.c):  The source code.

The results:
```bash
Array before sorting:
11 64 34 25 12 22 25 11 90
Array after sorting:
11 11 12 22 25 25 34 64 90
```
![Selection Sort Animation](./animators/selection_sort.gif)

This C program implements the Insertion Sort algorithm:

Explanation:

- Loop for i: This loop iterates through each position in the array where we want to place the next smallest element.
- Inner loop for j: This finds the smallest element in the unsorted part of the array (from i+1 to n-1).
- Swapping: After finding the smallest element, it's swapped with the element at index i, ensuring that a[0] to a[i] are in sorted order after each iteration.


This implementation follows the pseudo code by maintaining the invariants mentioned:

- a[k] is the smallest of a[i..n] at each step of the inner loop.
- After swapping, a[1..i] (or a[0..i] in C since indices start at 0) are in their final position in the sorted array.

Additional Notes:

- The comments here explain each step of both the sorting algorithm and the main function, providing clarity on what each part of the code does.
- The loop invariants are described in comments where actions lead to maintaining the sorted state of the array up to the current index i.


