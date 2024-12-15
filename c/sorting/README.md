### The Heap Sort

[Resource Link](https://www.toptal.com/developers/sorting-algorithms/merge-heap):  "Merge sort is very predictable. It makes between 0.5lg(n) and lg(n) comparisons per element, and between lg(n) and 1.5lg(n) swaps per element. The minima are achieved for already sorted data; the maxima are achieved, on average, for random data. If using Θ(n) extra space is of no concern, then merge sort is an excellent choice: It is simple to implement, and it is the only stable O(n·lg(n)) sorting algorithm. Note that when sorting linked lists, merge sort requires only Θ(lg(n)) extra space (for recursion).Heap sort is simple to implement, performs an O(n·lg(n)) in-place sort, but is not stable."

The first loop, the Θ(n) “heapify” phase, puts the array into heap order. The second loop, the O(n·lg(n)) “sortdown” phase, repeatedly extracts the maximum and restores heap order.

The sink function is written recursively for clarity. Thus, as shown, the code requires Θ(lg(n)) space for the recursive call stack. However, the tail recursion in sink() is easily converted to iteration, which yields the O(1) space bound.

Both phases are slightly adaptive, though not in any particularly useful manner. In the nearly sorted case, the heapify phase destroys the original order. In the reversed case, the heapify phase is as fast as possible since the array starts in heap order, but then the sortdown phase is typical. In the few unique keys case, there is some speedup but not as much as in shell sort or 3-way quicksort.

**ALGORITHM**

```bash
# heapify
for i = n/2:1, sink(a,i,n)
→ invariant: a[1,n] in heap order

# sortdown
for i = 1:n,
    swap a[1,n-i+1]
    sink(a,1,n-i)
    → invariant: a[n-i+1,n] in final position
end

# sink from i in a[1..n]
function sink(a,i,n):
    # {lc,rc,mc} = {left,right,max} child index
    lc = 2*i
    if lc > n, return # no children
    rc = lc + 1
    mc = (rc > n) ? lc : (a[lc] > a[rc]) ? lc : rc
    if a[i] >= a[mc], return # heap ordered
    swap a[i,mc]
    sink(a,mc,n)
 ```

[Heap Sort](./heap-sort.c): The source code

The results:

```bash
Unsorted array: 
12 11 13 5 6 7
Sorted array:
5 6 7 11 12 13
```

### The Heap Sort Animation ###
![Heap Sort Animation](./animators/heap-sort.gif)

**Explanation**

This C code implements the heap sort algorithm:

- sink function is responsible for ensuring the Max-Heap property by moving a node down the heap until it is larger than its children.
- heapSort is the main function that first builds a Max-Heap from the array and then repeatedly extracts the largest element (root), placing it at the end of the array.
- The sorting process is visualized by rendering the array after each major operation.

Key points:

- The Max-Heap property ensures the largest element is always at the root.
- The sorting is done in-place, so no additional arrays are needed.
- Array indices in C start at 0, so calculations for parent and child nodes use standard formulas like 2 * i + 1 for the left child.

### The Merge Sort
[Resource Link](https://www.toptal.com/developers/sorting-algorithms/merge-sort):  "Merge sort is very predictable. It makes between 0.5lg(n) and lg(n) comparisons per element, and between lg(n) and 1.5lg(n) swaps per element. The minima are achieved for already sorted data; the maxima are achieved, on average, for random data. If using Θ(n) extra space is of no concern, then merge sort is an excellent choice: It is simple to implement, and it is the only stable O(n·lg(n)) sorting algorithm. Note that when sorting linked lists, merge sort requires only Θ(lg(n)) extra space (for recursion).

Merge sort is the algorithm of choice for a variety of situations: when stability is required, when sorting linked lists, and when random access is much more expensive than sequential access (for example, external sorting on tape).

There do exist linear time in-place merge algorithms for the last step of the algorithm, but they are both expensive and complex. The complexity is justified for applications such as external sorting when Θ(n) extra space is not available."

**ALGORITHM**
```bash
# split in half
m = n / 2

# recursive sorts
sort a[1..m]
sort a[m+1..n]

# merge sorted sub-arrays using temp array
b = copy of a[1..m]
i = 1, j = m+1, k = 1
while i <= m and j <= n,
    a[k++] = (a[j] < b[i]) ? a[j++] : b[i++]
    → invariant: a[1..k] in final position
while i <= m,
    a[k++] = b[i++]
    → invariant: a[1..k] in final position
```

[Merge Sort](./merge-sort.c): The source code
### The Merge Sort Animation ###
![Merge Sort Animation](./animators/merge-sort.gif)

**Explanation**
This C code implements the merge sort algorithm:

- merge function is responsible for merging two sorted subarrays of a[].
- mergeSort is the recursive function that splits the array into two halves, sorts them, and then merges them back.
- The main function shows how to use mergeSort with an example array.


Please note:

- Error handling for memory allocation has been included in the merge function.
- Array indices in C start at 0, unlike some pseudo-code where they might start at 1. Hence, the adjustment in index calculations.

### The Quick Sort
[Resource Link](https://www.toptal.com/developers/sorting-algorithms/quick-sort):  "When carefully implemented, quick sort is robust and has low overhead. When a stable sort is not needed, quick sort is an excellent general-purpose sort – although the 3-way partitioning version should always be used instead.

The 2-way partitioning code shown above is written for clarity rather than optimal performance; it exhibits poor locality, and, critically, exhibits O(n2) time when there are few unique keys. A more efficient and robust 2-way partitioning method is given in Quicksort is Optimal by Robert Sedgewick and Jon Bentley. The robust partitioning produces balanced recursion when there are many values equal to the pivot, yielding probabilistic guarantees of O(n·lg(n)) time and O(lg(n)) space for all inputs.

With both sub-sorts performed recursively, quick sort requires O(n) extra space for the recursion stack in the worst case when recursion is not balanced. This is exceedingly unlikely to occur, but it can be avoided by sorting the smaller sub-array recursively first; the second sub-array sort is a tail recursive call, which may be done with iteration instead. With this optimization, the algorithm uses O(lg(n)) extra space in the worst case."

**ALGORITHM**

```bash
_# choose pivot_
swap a[1,rand(1,n)]

_# 2-way partition_
k = 1
for i = 2:n, if a[i] < a[1], swap a[++k,i]
swap a[1,k]
_→ invariant: a[1..k-1] < a[k] <= a[k+1..n]_

_# recursive sorts_
sort a[1..k-1]
sort a[k+1,n]
```

[Quick Sort](./quick-sort.c): The source code.  
### Quick Sort Animation ###
![Quick Sort Animation](./animators/quick-sort.gif)  


**Explanation:**

Includes and Global Setup
- stdio.h: For input and output operations like printf.
- stdlib.h: Provides rand() and srand() for random number generation.
- time.h: Used to seed the random number generator with time(NULL).

Swap Function
- A utility function to exchange the values of two integers using pointers.

Partition Function
- Pivot Selection: Randomly choose a pivot to improve average-case performance and avoid worst-case scenarios for already sorted arrays.
  
- Partitioning: 
  - i keeps track of where elements smaller than the pivot should end up.
  - The loop moves elements less than the pivot to the left side of i.
  
- Final Pivot Placement: The pivot is placed in its correct position, and the function returns this position.

QuickSort Function
- Base Case: If low is not less than high, there's nothing to sort (segment has 0 or 1 element).
- Divide and Conquer: 
  - The array is partitioned, and the pivot's final position is obtained.
  - The function then recursively sorts the sub-arrays on either side of the pivot.

Print Function
- Helper function to display the array content for debugging or visualization.

Main Function
- Initializes an array to be sorted.
- Calculates the size of the array.
- Prints the array before and after sorting to show the effect of the quickSort function.
- ---

### The Three Way Quick Sort
[Resource Link](https://www.toptal.com/developers/sorting-algorithms/quick-sort-3-way): "The 3-way partition variation of quick sort has slightly higher overhead compared to the standard 2-way partition version. Both have the same best, typical, and worst case time bounds, but this version is highly adaptive in the very common case of sorting with few unique keys.

The 3-way partitioning code shown above is written for clarity rather than optimal performance; it exhibits poor locality, and performs more swaps than necessary. A more efficient but more elaborate 3-way partitioning method is given in Quicksort is Optimal by Robert Sedgewick and Jon Bentley.

When stability is not required, quick sort is the general purpose sorting algorithm of choice. Recently, a novel dual-pivot variant of 3-way partitioning has been discovered that beats the single-pivot 3-way partitioning method both in theory and in practice."

[Three Way Quick Sort](./quick3-sort.c): The source code.

![Three Way Quick Sort Animation](./animators/three-way-quick-sort.gif)

Explanation:

- Pivot Selection: Randomly selects a pivot and swaps it with the last element.
- Partitioning: Partitions the array into three parts: elements less than the pivot, elements equal to the pivot, and elements greater than the pivot.
- Pivot Movement: After partitioning, equal elements are moved to the center to maintain stability for equal elements.
- Recursion: The function sort is called recursively for the parts of the array excluding the pivots.

This implementation follows the 3-way partitioning scheme which is particularly efficient for arrays with many duplicate elements. 

---


### The Shell sort
[Resource Link](https://www.toptal.com/developers/sorting-algorithms/shell-sort): "The worse-case time complexity of shell sort depends on the increment sequence. For the increments 1 4 13 40 121…, which is what is used here, the time complexity is O(n3/2). For other increments, time complexity is known to be O(n4/3) and even O(n·lg2(n)). Neither tight upper bounds on time complexity nor the best increment sequence are known.

Because shell sort is based on insertion sort, shell sort inherits insertion sort’s adaptive properties. The adapation is not as dramatic because shell sort requires one pass through the data for each increment, but it is significant. For the increment sequence shown above, there are log3(n) increments, so the time complexity for nearly sorted data is O(n·log3(n)).

Because of its low overhead, relatively simple implementation, adaptive properties, and sub-quadratic time complexity, shell sort may be a viable alternative to the O(n·lg(n)) sorting algorithms for some applications when the data to be sorted is not very large."  as quoted from the source

[Shell Sort](./shell.c): The source code.

![Shell Sort Animation](./animators/shell-sort.gif)

Explanation:

Insertion Sort with Gap: The insertionSort function does an insertion sort with a gap h. This is essentially the same as normal insertion sort, but instead of comparing adjacent elements, we compare elements h positions apart.

Shell Sort: 
- The initial gap h is calculated until h >= n. Here h grows by the formula 3*h + 1 which is a common increment sequence for Shell Sort.
- Then, h is reduced by dividing by 3 in each iteration, effectively sorting larger and larger sub-arrays until h becomes 1, which then sorts the array completely.

Main Function: Demonstrates the use of the shellSort function with a small array to show before and after sorting results.



---
### The bubble sort

[Resource Link](https://www.toptal.com/developers/sorting-algorithms/bubble-sort): "Bubble sort has many of the same properties as insertion sort, but has slightly higher overhead. In the case of nearly sorted data, bubble sort takes O(n) time, but requires at least 2 passes through the data (whereas insertion sort requires something more like 1 pass)." as quoted from the source.

[Bubble Sort](bubble-sort.c):  The source code.  

![Bubble Sort Animation](./animators/bubble-sort.gif)

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

![Insertion Sort Annimation](./animators/insertion-sort.gif)  


This C program implements the Insertion Sort algorithm:

- insertionSort: This function takes an array and its size, then sorts the array in-place using the insertion sort method. It iterates through the array starting from the second element, comparing it with the elements before it, moving larger elements one position ahead to insert the current element in its correct position.  
- printArray: A utility function to print the array contents.  
- main: Demonstrates how to use the insertion sort function by initializing an unsorted array, sorting it, and then printing both the unsorted and sorted arrays to show the effect of the sorting algorithm.  


This code follows the logic of insertion sort where you insert each element into its correct position in the sorted portion of the array.
---
### The selection sort
[Resource Link](https://www.toptal.com/developers/sorting-algorithms/selection-sort): "From the comparions presented here, one might conclude that selection sort should never be used. It does not adapt to the data in any way (notice that the four animations above run in lock step), so its runtime is always quadratic.  

However, selection sort has the property of minimizing the number of swaps. In applications where the cost of swapping items is high, selection sort very well may be the algorithm of choice."  as quoted from the source. 

[Selection Sort](./selection-sort.c):  The source code.

The results:
```bash
Array before sorting:
11 64 34 25 12 22 25 11 90
Array after sorting:
11 11 12 22 25 25 34 64 90
```
![Selection Sort Animation](./animators/selection-sort.gif)

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



