import matplotlib.pyplot as plt
import matplotlib.animation as animation

def selection_sort(arr):
    steps = []
    n = len(arr)
    for i in range(n):
        min_idx = i
        steps.append(arr.copy())  # Capture state before finding the minimum
        for j in range(i + 1, n):
            if arr[j] < arr[min_idx]:
                min_idx = j
        # After finding the minimum, swap if necessary
        if min_idx != i:
            arr[i], arr[min_idx] = arr[min_idx], arr[i]
        steps.append(arr.copy())  # Capture state after swap
    return steps

# Initial array
data = [64, 34, 25, 12, 22, 11, 90]
steps = selection_sort(data)

# Create animation
fig, ax = plt.subplots()
bars = ax.bar(range(len(data)), data, color="skyblue")

def update(frame):
    for bar, height in zip(bars, steps[frame]):
        bar.set_height(height)

ani = animation.FuncAnimation(fig, update, frames=len(steps), repeat=False, interval=500)
plt.xlabel("Index")
plt.ylabel("Value")
plt.title("Selection Sort Animation")
plt.tight_layout()

# Save the animation as a GIF
ani.save("selection_sort.gif", fps=2, writer="pillow")