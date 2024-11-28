import matplotlib.pyplot as plt
import matplotlib.animation as animation

def bubble_sort(arr):
    steps = []
    n = len(arr)
    for i in range(n - 1):
        swapped = False
        for j in range(n - 1, i, -1):
            steps.append(arr.copy())  # Capture state before a potential swap
            if arr[j] < arr[j - 1]:
                arr[j], arr[j - 1] = arr[j - 1], arr[j]  # Swap
                swapped = True
        steps.append(arr.copy())  # Capture state after the pass
        if not swapped:
            break
    return steps

# Initial array
data = [64, 34, 25, 12, 22, 11, 90]
steps = bubble_sort(data)

# Create animation
fig, ax = plt.subplots()
bars = ax.bar(range(len(data)), data, color="skyblue")

def update(frame):
    for bar, height in zip(bars, steps[frame]):
        bar.set_height(height)

ani = animation.FuncAnimation(fig, update, frames=len(steps), repeat=False)
plt.xlabel("Index")
plt.ylabel("Value")
plt.title("Bubble Sort Animation")
plt.tight_layout()

# Save the animation as a GIF
ani.save("bubble_sort.gif", fps=2, writer="pillow")
