import matplotlib.pyplot as plt
import matplotlib.animation as animation
import random

def swap(arr, i, j):
    arr[i], arr[j] = arr[j], arr[i]

def partition(arr, low, high):
    steps = []
    pivot = arr[high]
    i = low
    k = low
    p = high
    
    while i < p:
        if arr[i] < pivot:
            swap(arr, i, k)
            k += 1
            i += 1
        elif arr[i] == pivot:
            p -= 1
            swap(arr, i, p)
        else:
            i += 1
        steps.append(arr[:])  # Capture state after each comparison/swap

    # Move pivots to center
    m = min(p - k, high - p + 1)
    for j in range(m):
        swap(arr, k + j, high - m + 1 + j)
    steps.append(arr[:])  # Capture state after moving pivots

    return k, p, steps

def quicksort(arr, low, high, steps):
    if low < high:
        k, p, step = partition(arr, low, high)
        steps.extend(step)
        quicksort(arr, low, k - 1, steps)
        quicksort(arr, high - p + k + 1, high, steps)

def three_way_quicksort(arr):
    steps = []
    random.shuffle(arr)  # Randomize the array to avoid worst-case scenarios
    quicksort(arr, 0, len(arr) - 1, steps)
    return steps

# Initial array
data = [64, 34, 25, 12, 22, 11, 90]
steps = three_way_quicksort(data)

# Create animation
fig, ax = plt.subplots()
bars = ax.bar(range(len(data)), data, color="skyblue")

def update(frame):
    for bar, height in zip(bars, steps[frame]):
        bar.set_height(height)

ani = animation.FuncAnimation(fig, update, frames=len(steps), interval=500, repeat=False)
plt.xlabel("Index")
plt.ylabel("Value")
plt.title("Three-Way Quicksort Animation")
plt.tight_layout()

# Save the animation as a GIF
ani.save("three_way_quicksort.gif", writer="pillow")