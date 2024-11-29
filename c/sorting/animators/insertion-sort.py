import matplotlib.pyplot as plt
import matplotlib.animation as animation

def insertion_sort(arr):
    steps = []
    n = len(arr)
    for i in range(1, n):
        key = arr[i]
        j = i - 1
        steps.append(arr.copy())  # Capture state before insertion
        while j >= 0 and arr[j] > key:
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key
        steps.append(arr.copy())  # Capture state after insertion
    return steps

# Initial array
data = [64, 34, 25, 12, 22, 11, 90]
steps = insertion_sort(data)

# Create animation
fig, ax = plt.subplots()
bars = ax.bar(range(len(data)), data, color="skyblue")

def update(frame):
    for bar, height in zip(bars, steps[frame]):
        bar.set_height(height)

ani = animation.FuncAnimation(fig, update, frames=len(steps), repeat=False)
plt.xlabel("Index")
plt.ylabel("Value")
plt.title("Insertion Sort Animation")
plt.tight_layout()

# Save the animation as a GIF
ani.save("insertion_sort.gif", fps=2, writer="pillow")