#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Constants for screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int ARRAY_SIZE = 50; // Number of bars to show
const char *OUTPUT_DIR = "./frames/"; // Directory to save frames

// Global array to store frames
SDL_Texture **frames = NULL;
int frameCount = 0;

// Prototypes
void swap(int *a, int *b);
void sink(int arr[], int i, int n);
int partition(int arr[], int low, int high, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity);
void quickSort(int arr[], int low, int high, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity );
void mergeSort(int arr[], int left, int right, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity );
void merge(int arr[], int left, int mid, int right, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity);
void heapSort(int arr[], int n, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity );
void insertionSort(int arr[], int n, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity);
void selectionSort(int arr[], int n, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity);
void bubbleSort(int arr[], int n, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity);
void shellSort(int arr[], int n, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity); 
void threeWayQuickSort(int arr[], int low, int high, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity);
void addFrame(SDL_Renderer *renderer, int *totalFrameCapacity); 
void renderBars(SDL_Renderer *renderer, int arr[], int size, int *frame, int *totalFrameCapacity);
void cleanupFrames( int *totalFrameCapacity); 
void displayFrames(SDL_Renderer *renderer);
void printArray(int arr[], int n);
void cleanupFiles(const char *outputDir); 

int main(int argc, char *argv[]) {
    // Debug:  uncomment to analyze
    // printf("framecount in main is: %d\n", frameCount);

    int totalFrameCapacity = ARRAY_SIZE;

    frames = malloc(ARRAY_SIZE * sizeof(SDL_Texture *));
    if (!frames) {
        fprintf(stderr, "Failed to allocate memory for frames array\n");
        exit(1);
    }

    for (int i = 0; i < ARRAY_SIZE; i++) {
        frames[i] = NULL; // Initialize to NULL for safety
    }

    int frame = 0; //Track the number of frames

    // Check command line argument    
    if (argc != 3 || strcmp(argv[1], "gen-gifs") != 0 || 
        ( strcmp(argv[2], "quick-sort") != 0 && \
          strcmp(argv[2], "merge-sort") != 0 && \
          strcmp(argv[2], "heap-sort") != 0 &&  \
          strcmp(argv[2], "selection-sort") != 0 && \
          strcmp(argv[2], "insertion-sort") != 0 && \
          strcmp(argv[2], "bubble-sort") != 0 && \
          strcmp(argv[2], "shell-sort") != 0 && \
          strcmp(argv[2], "three-way-quick-sort") != 0 )) {
        fprintf(stderr, "Usage: %s gen-gifs [quick-sort|merge-sort|heap-sort|selection-sort|insertion-sort|bubble-sort|shell-sort|three-way-quick-sort]\n", argv[0]);
        return 1;
    }
    
    // Clean up intermediate files
    cleanupFiles(OUTPUT_DIR);


    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Sorting Animation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Generate random array
    int arr[ARRAY_SIZE];
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = rand() % SCREEN_HEIGHT;
        // Debug: Uncomment for analysis
        // printf("Index %d has value %d: ", i, arr[i]);
    }

    // Initialize for Selection Sort
    int n = sizeof(arr) / sizeof(arr[0]);

    // Perform sorting and capture frames
    if (strcmp(argv[2], "quick-sort") == 0) {
        quickSort(arr, 0, ARRAY_SIZE - 1, renderer, &frame, &totalFrameCapacity);
    } else if (strcmp(argv[2], "merge-sort") == 0) {
        mergeSort(arr, 0, ARRAY_SIZE - 1, renderer, &frame, &totalFrameCapacity);
    } else if (strcmp(argv[2], "heap-sort") == 0) {
        heapSort(arr, ARRAY_SIZE, renderer, &frame, &totalFrameCapacity);
    } else if (strcmp(argv[2], "selection-sort") == 0) {
        selectionSort(arr, n, renderer, &frame, &totalFrameCapacity);
    } else if (strcmp(argv[2], "insertion-sort") == 0) {
        insertionSort(arr, ARRAY_SIZE, renderer, &frame, &totalFrameCapacity);
    } else if (strcmp(argv[2], "bubble-sort") == 0) {
        bubbleSort(arr, ARRAY_SIZE, renderer, &frame, &totalFrameCapacity);
    } else if (strcmp(argv[2], "shell-sort") == 0) {
        shellSort(arr, ARRAY_SIZE, renderer, &frame, &totalFrameCapacity);
    } else if (strcmp(argv[2], "three-way-quick-sort") == 0) {
        threeWayQuickSort(arr, 0, ARRAY_SIZE -1,  renderer, &frame, &totalFrameCapacity);
    }

    // Display saved frames on the screen.  Uncomment to debug
    //
    // displayFrames(renderer);

    // Generate a list of PNG files for concatenation
    char gifCommand[512];
    snprintf(gifCommand, sizeof(gifCommand),
          "ffmpeg -y -pattern_type glob -i '%sframe*.png' -filter_complex \"fps=20,scale=740:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\" %s.gif",
          OUTPUT_DIR, argv[2]);

    if (system(gifCommand) != 0) {
        fprintf(stderr, "Failed to generate GIF %s.gif\n", argv[2]);
        return 1;
    }

    // up frames array
    cleanupFrames(&totalFrameCapacity);

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // Optional: Clean up intermediate files
    // char cleanupCommand[512];
    // snprintf(cleanupCommand, sizeof(cleanupCommand), "rm %sframe*.png", OUTPUT_DIR);
    // system(cleanupCommand);

    printf("End of program\n");

    return 0;
}

// Helper functiont clean up PNG files
void cleanupFiles(const char *outputDir) {
    printf("Output Directory %s", outputDir);
    // Check if directory exists
    if (access(outputDir, F_OK) != 0) {
        fprintf(stderr, "Output directory %s does not exist. Skipping cleanup.\n", outputDir);
        return;
    }

    // Construct cleanup command to remove matching files
    char cleanupCommand[512];
    snprintf(cleanupCommand, sizeof(cleanupCommand), "rm -f %sframe*.png 2>/dev/null", outputDir);

    // Execute cleanup command
    int result = system(cleanupCommand);

    if (result != 0) {
        fprintf(stderr, "Failed to clean up PNG files in %s. Ensure you have proper permissions.\n", outputDir);
    } else {
        printf("PNG files in %s cleaned up successfully (if any existed).\n", outputDir);
    }
}


// Helper function for addFrame function
void renderBars(SDL_Renderer *renderer, int arr[], int size, int *frame, int *totalFrameCapacity) {
    // Debugging: Print the array being rendered.  Uncomment to debug
    // printf("Rendering frame %d with bar heights: ", *frame);
    // for (int i = 0; i < size; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 51, 102, 255); // Black background
    SDL_RenderClear(renderer);

    // Draw bars
    int barWidth = SCREEN_WIDTH / size;
    for (int i = 0; i < size; i++) {
        // Normalize height to 0-255 for gray scale
        int gray = (arr[i] * 255) / SCREEN_HEIGHT; 
        SDL_SetRenderDrawColor(renderer, gray, gray, gray, 255);

        // Calculate bar dimensions
        SDL_Rect fillRect = {
            i * barWidth,            // x-coordinate
            SCREEN_HEIGHT - arr[i],  // y-coordinate 
            barWidth,                // width 
            arr[i]                   // height
        };
        SDL_RenderFillRect(renderer, &fillRect);

        // Debugging output for bar dimensions (optional)
        // Uncomment to debug
        // printf("Bar %d: x=%d, y=%d, width=%d, height=%d\n", i, fillRect.x, fillRect.y, fillRect.w, fillRect.h);
    }

    // Present the rendered frame
    SDL_RenderPresent(renderer);

    // Optionally capture the frame and increment the frame counter
    if (frame && totalFrameCapacity) {
        addFrame(renderer, totalFrameCapacity); // Capture the current frame
        (*frame)++;         // Increment the frame counter
    }
}

// Function to add a frame to our global array
void addFrame(SDL_Renderer *renderer, int *totalFrameCapacity) {
    // Debugging:  Uncomment to debug
    // printf("Capturing frame %d\n", frameCount);

    // Extend the frames array if necessary
    if (frameCount >= *totalFrameCapacity) {
        int newCapacity = *totalFrameCapacity * 2;
        SDL_Texture **newFrames = realloc(frames, newCapacity * sizeof(SDL_Texture *));
        if (!newFrames) {
            fprintf(stderr, "Failed to extend frames array\n");
            exit(1);
        }
        frames = newFrames;
        *totalFrameCapacity = newCapacity;
    }

    // Create a surface to capture the current renderer content
    SDL_Surface *tempSurface = SDL_CreateRGBSurfaceWithFormat(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);
    if (!tempSurface) {
        fprintf(stderr, "Failed to create temporary surface: %s\n", SDL_GetError());
        return;
    }

    // Ensure the renderer is updated before capturing
    SDL_RenderPresent(renderer);

    // Read pixels from the renderer into the surface
    if (SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, tempSurface->pixels, tempSurface->pitch) != 0) {
        fprintf(stderr, "Failed to read pixels for frame %d: %s\n", frameCount, SDL_GetError());
        SDL_FreeSurface(tempSurface);
        return;
    }

    // Save the surface as a PNG for debugging
    char debugFilename[128];
    snprintf(debugFilename, sizeof(debugFilename), "%sframe%03d.png", OUTPUT_DIR, frameCount);
    if (IMG_SavePNG(tempSurface, debugFilename) != 0) {
        fprintf(stderr, "Failed to save debug PNG for frame %d: %s\n", frameCount, SDL_GetError());
    } // else {
        // Uncomment to debug
        // printf("Debug frame %d saved as %s\n", frameCount, debugFilename);
    // }

    // Store the frame in a texture (optional, for replaying)
    frames[frameCount] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (frames[frameCount]) {
        SDL_UpdateTexture(frames[frameCount], NULL, tempSurface->pixels, tempSurface->pitch);
    } else {
        fprintf(stderr, "Failed to create texture for frame %d: %s\n", frameCount, SDL_GetError());
    }

    // Free the surface
    SDL_FreeSurface(tempSurface);

    frameCount++;
    // Uncomment to debug
    // printf("Frame %d captured successfully\n", frameCount);
}

void displayFrames(SDL_Renderer *renderer) {
    printf("Displaying %d frames...\n", frameCount);

    for (int i = 0; i < frameCount; i++) {
        // Check if the frame texture is valid
        if (!frames[i]) {
            fprintf(stderr, "Warning: frames[%d] is NULL, skipping\n", i);
            continue;
        }

        // Clear the screen
        SDL_RenderClear(renderer);

        // Copy the texture to the renderer
        if (SDL_RenderCopy(renderer, frames[i], NULL, NULL) != 0) {
            fprintf(stderr, "Failed to copy frame %d to renderer: %s\n", i, SDL_GetError());
            continue;
        }

        // Present the frame on the screen
        SDL_RenderPresent(renderer);

        // Optional: Delay to allow viewing each frame
        // Uncomment to debug
        // SDL_Delay(500);
    }

    printf("Finished displaying frames.\n");
}



void cleanupFrames( int *totalFrameCapacity) {
    if (!frames || frameCount <= 0) {
        fprintf(stderr, "cleanupFrames called with invalid data: frames=%p, frameCount=%d\n", (void *)frames, frameCount);
        return;
    }

    // Uncomment to degub
    // printf("Cleaning up %d frames\n", frameCount);
    for (int i = 0; i < frameCount; i++) {
        if (frames[i]) {
            // Uncomment to debug
            // printf("Cleaning up frame %d\n", i);
            SDL_DestroyTexture(frames[i]);
            frames[i] = NULL;
        }
    }
    free(frames);
    frames = NULL; // For safety purposes
    *totalFrameCapacity = 0;
    frameCount = 0;

    // Uncomment to debug
    // printf("Finished calling cleanupFrames. TotalFrameCapacity = %d, frameCount = %d\n", *totalFrameCapacity, frameCount);

}

//-----------------
// Sorting algorithms
//-----------------

//-----------------
// Quick Sort
//-----------------
void swap(int *a, int *b) {
    // Uncomment to debug
    // printf("Swapping %d and %d\n", *a, *b);
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity) {
    int pivot = arr[high];  
    int i = (low - 1);  

    // Checking the array state before the swap
    // Uncomment to debug
    // printf("Array before swap: ");
    // for (int i = 0; i < ARRAY_SIZE; i++) {
    //    printf("%d ", arr[i]);
    // }
    // printf("\n");

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
            // Update screen in gray tones
            renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity);
        } else {
            renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity);
        }
    }
    // Initiate a swap
    swap(&arr[i + 1], &arr[high]);

    // Capture frame after pivot swap
    renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity); 

    // Checking the array state after the swap
    // Uncomment to debug
    // printf("Array after swap: ");
    // for (int i = 0; i < ARRAY_SIZE; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    return (i + 1);
}

void quickSort(int arr[], int low, int high, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity ) {
    if (low < high) {
        int pi = partition(arr, low, high, renderer, frame, totalFrameCapacity);
        quickSort(arr, low, pi - 1, renderer, frame, totalFrameCapacity);
        quickSort(arr, pi + 1, high, renderer, frame, totalFrameCapacity);
    }
    // Capture the final sorted state
    if (low == 0 && high == ARRAY_SIZE - 1) {
        renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity);
    }
}

//-----------------
//End Quick Sort
//-----------------

//-----------------
// Merge Sort
//-----------------

void mergeSort(int arr[], int left, int right, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity ) {
    if (left < right) {
        // Debug:  Uncomment for analysis
        // printf("Left value: %d\n", left);
        // printf("Right value: %d\n", right);
        int mid = left + (right - left) / 2;
        // Debug:  Uncomment for analysis
        // printf("Middle value:  %d\n", mid); 
        // Sort first and second halves
        mergeSort(arr, left, mid, renderer, frame, totalFrameCapacity);
        mergeSort(arr, mid + 1, right, renderer, frame, totalFrameCapacity);
        
        merge(arr, left, mid, right, renderer, frame, totalFrameCapacity);
    }
}

void merge(int arr[], int left, int mid, int right, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temp arrays
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    if (!L || !R) {
        fprintf(stderr, "Memory allocation failed for temporary arrays\n");
        exit(1);
    }

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++) L[i] = arr[left + i];
    for (j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    // Debugging: Print the temporary arrays
    // printf("Merging arrays:\n");
    // printf("Left array: ");
    // for (i = 0; i < n1; i++) printf("%d ", L[i]);
    // printf("\nRight array: ");
    // for (j = 0; j < n2; j++) printf("%d ", R[j]);
    // printf("\n");

    // Merge the temp arrays back into arr[l..r]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = left; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;

        // Debugging: Print the array after each merge step
        // printf("Merged array after step: ");
        // for (int x = left; x <= right; x++) printf("%d ", arr[x]);
        // printf("\n");

        // Update screen for each merge operation
        renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity);
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;

        // Debugging: Print the array after adding remaining elements of L[]
        // printf("Merged array after adding from Left: ");
        // for (int x = left; x <= right; x++) printf("%d ", arr[x]);
        // printf("\n");

        renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity);
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;

        // Debugging: Print the array after adding remaining elements of R[]
        // printf("Merged array after adding from Right: ");
        // for (int x = left; x <= right; x++) printf("%d ", arr[x]);
        // printf("\n");

        renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity);
    }

    // Debugging: Final state of the merged section
    // printf("Final merged array: ");
    // for (int x = left; x <= right; x++) printf("%d ", arr[x]);
    // printf("\n");

    free(L);
    free(R);
}

//-----------------
// End Merge Sort
//-----------------

//-----------------
// Heap Sort
//-----------------
void heapSort(int arr[], int n, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity) {
    printf("Starting heap sort on array of size %d\n", n);

    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--) {
        sink(arr, i, n);

        // Render the heap after building each node
        renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity);
    }

    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--) {
        // Swap the root (largest) with the last element
        printf("Swapping root %d (value %d) with index %d (value %d)\n", 0, arr[0], i, arr[i]);
        swap(&arr[0], &arr[i]);

        // Re-heapify the reduced heap
        sink(arr, 0, i);

        // Render the array after the entire sinking process is complete
        renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity);
    }

    // Final render to ensure the array is sorted
    renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity);
    printf("Heap sort complete.\n");
}

void sink(int arr[], int i, int n) {
    int largest = i;           // Initialize largest as root
    int left = 2 * i + 1;      // Left child
    int right = 2 * i + 2;     // Right child

    // Check if left child is larger than root
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    // Check if right child is larger than the largest so far
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // Swap and recursively heapify the affected subtree
    if (largest != i) {
        printf("Swapping %d (value %d) with %d (value %d)\n", i, arr[i], largest, arr[largest]);
        swap(&arr[i], &arr[largest]);

        // Recursively sink the subtree
        sink(arr, largest, n);
    }
}


//-----------------
// End Heap Sort
//-----------------

//-----------------
// Selection Sort
//-----------------

void selectionSort(int arr[], int n, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity) {
    printf("Starting selection sort on array of size %d\n", n);

    for (int i = 0; i < n - 1; i++) {
        int k = i; // Assume the current index is the smallest

        // Find the smallest element in the rest of the array
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[k]) {
                k = j;
            }
        }

        // Swap the smallest element with the current index
        if (k != i) {
            printf("Swapping index %d (value %d) with index %d (value %d)\n", i, arr[i], k, arr[k]);
            swap(&arr[i], &arr[k]);

            // Render the array after each swap
            renderBars(renderer, arr, n, frame, totalFrameCapacity);
        }
    }
    // Final render to ensure the array is sorted
    renderBars(renderer, arr, n, frame, totalFrameCapacity);
    printf("Selection sort complete.\n");
}

// Helper function to print the array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
//-----------------
// End Selection Sort
//-----------------

//-----------------
// Insertion Sort
//-----------------
void insertionSort(int arr[], int n, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity) {
    printf("Starting insertion sort on array of size %d\n", n);

       for (int i = 1; i < n; i++) {
        int k = i;

        // Check for changes in the array and capture only if necessary
        while (k > 0 && arr[k] < arr[k - 1]) {
            // Swap the elements
            // Debug:  Uncomment to analyze
            // printf("Swapping %d and %d\n", arr[k], arr[k - 1]);
            swap(&arr[k], &arr[k - 1]);
            k--;

            // Render only after each swap
            renderBars(renderer, arr, n, frame, totalFrameCapacity);
        }
    }

    // Final render to ensure the sorted array is displayed
    renderBars(renderer, arr, n, frame, totalFrameCapacity);
    printf("Insertion sort complete.\n"); 

}
//-------------------
// End Selection Sort
//-------------------

//-------------------
// Bubble Sort
//-------------------
void bubbleSort(int arr[], int n, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity) {
    printf("Starting bubble sort on array of size %d\n", n);

    for (int i = 0; i < n; i++) {
        bool swapped = false;

        for (int j = n - 1; j > i; j--) {
            if (arr[j] < arr[j - 1]) {
                // Swap elements if needed
                // Debug: Uncomment to analyze
                // printf("Swapping elements at indices %d (value %d) and %d (value %d)\n", j, arr[j], j - 1, arr[j - 1]);
                swap(&arr[j], &arr[j - 1]);
                swapped = true;

                // Render the array after each swap
                renderBars(renderer, arr, n, frame, totalFrameCapacity);
            }
        }

        // Render the array at the end of the pass
        renderBars(renderer, arr, n, frame, totalFrameCapacity);

        // If no swaps were made, the array is sorted
        if (!swapped) {
            printf("No swaps made in pass %d, array is sorted\n", i + 1);
            break;
        }
    }

    // Final render to ensure the sorted array is captured
    renderBars(renderer, arr, n, frame, totalFrameCapacity);
    printf("Bubble sort complete.\n");
}
//-------------------
// End Bubble Sort
//-------------------

//-------------------
// Shell Sort
//-------------------
void shellSort(int arr[], int n, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity) {
    printf("Starting Shell sort on array of size %d\n", n);

    // Initialize the gap size using the 3x+1 sequence
    int h = 1;
    while (h < n / 3) {
        h = 3 * h + 1;
    }

    // Perform h-sorting for decreasing gap sizes
    while (h > 0) {
        // Debug:  Uncomment to analyze
        // printf("Sorting with gap size h = %d\n", h);

        for (int k = 0; k < h; k++) {
            // Perform insertion sort on h-subarray starting at index k
            for (int i = k + h; i < n; i += h) {
                int temp = arr[i];
                int j = i;

                // Shift elements in the h-subarray
                while (j >= h && arr[j - h] > temp) {
                    arr[j] = arr[j - h];
                    j -= h;

                    // Render the array after every shift
                    renderBars(renderer, arr, n, frame, totalFrameCapacity);
                }

                arr[j] = temp;

                // Render the array after inserting the element
                renderBars(renderer, arr, n, frame, totalFrameCapacity);
            }
        }

        // Move to the next smaller gap size
        h /= 3;

        // Render the array after finishing with the current gap size
        renderBars(renderer, arr, n, frame, totalFrameCapacity);
    }

    // Final render to ensure the sorted array is captured
    renderBars(renderer, arr, n, frame, totalFrameCapacity);
    printf("Shell sort complete.\n");
}
//-------------------
// End Shell Sort
//-------------------

//---------------------
// Three Way Quick Sort
//---------------------
void threeWayQuickSort(int arr[], int low, int high, SDL_Renderer *renderer, int *frame, int *totalFrameCapacity) {
    if (low >= high) return;

    // Randomly select pivot and swap with the last element
    int pivotIndex = low + rand() % (high - low + 1);
    swap(&arr[high], &arr[pivotIndex]);
    int pivot = arr[high];

    // Debug:  Uncomment to analyze
    // printf("Pivot chosen: %d at index %d\n", pivot, pivotIndex);

    // Render after pivot selection
    renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity);

    // 3-way partition
    int i = low, k = low, p = high;
    while (i < p) {
        if (arr[i] < pivot) {
            swap(&arr[i], &arr[k]);
            i++;
            k++;
        } else if (arr[i] == pivot) {
            p--;
            swap(&arr[i], &arr[p]);
        } else {
            i++;
        }

        // Render after each comparison and swap
        renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity);
    }

    // Move pivots to the center
    int m = p - k < high - p + 1 ? p - k : high - p + 1;
    for (int j = 0; j < m; j++) {
        swap(&arr[k + j], &arr[high - m + 1 + j]);
    }

    // Render after moving pivots to the center
    renderBars(renderer, arr, ARRAY_SIZE, frame, totalFrameCapacity);

    // Recursive calls for the left and right partitions
    threeWayQuickSort(arr, low, k - 1, renderer, frame, totalFrameCapacity);
    threeWayQuickSort(arr, high - p + k + 1, high, renderer, frame, totalFrameCapacity);
}
//-------------------------
// End Three Way Quick Sort
//-------------------------