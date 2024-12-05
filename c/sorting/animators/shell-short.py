import matplotlib.pyplot as plt
import matplotlib.animation as animation

def shell_sort(arr):
    steps = []
    n = len(arr)
    
    # Initial gap calculation
    h = 1
    while h < n // 3:
        h = 3 * h + 1
    
    while h > 0:
        for i in range(h, n):
            temp = arr[i]
            j = i
            steps.append(arr.copy())  # Capture state before potential shifts
            while j >= h and arr[j - h] > temp:
                arr[j] = arr[j - h]
                j -= h
            arr[j] = temp
            steps.append(arr.copy())  # Capture state after each shift
        h //= 3  # Reduce gap

    return steps

# Initial array
data = [64, 34, 25, 12, 22, 11, 90]
steps = shell_sort(data)

# Create animation
fig, ax = plt.subplots()
bars = ax.bar(range(len(data)), data, color="skyblue")

def update(frame):
    for bar, height in zip(bars, steps[frame]):
        bar.set_height(height)

ani = animation.FuncAnimation(fig, update, frames=len(steps), repeat=False)
plt.xlabel("Index")
plt.ylabel("Value")
plt.title("Shell Sort Animation")
plt.tight_layout()

# Save the animation as a GIF
ani.save("shell_sort.gif", fps=2, writer="pillow")