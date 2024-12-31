#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>  // For seeding randomness

// Constants for histogram bins
#define BIN_SIZE 100   // Size of each bin (e.g., 0-99, 100-199)
#define NUM_BINS 10    // Number of bins (e.g., up to 1000 bytes)

// Structure to track each allocation
typedef struct Allocation {
    void *address;           // Address of the allocated memory
    size_t size;             // Size of the allocation
    struct Allocation *next; // Pointer to the next allocation
} Allocation;

// Global Variables
static Allocation *allocations = NULL; // Linked list for active allocations
static size_t total_allocated = 0;     // Total memory allocated
static size_t total_freed = 0;         // Total memory freed
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static size_t histogram[NUM_BINS] = {0}; // Histogram bins

// Function Prototypes
void add_allocation(void *address, size_t size);
void remove_allocation(void *address);
void *my_malloc(size_t size);
void my_free(void *ptr);
void *my_realloc(void *ptr, size_t size);
void update_histogram(size_t size);
void print_statistics();
void print_histogram();

/**
 * Main function for testing the custom allocator.
 */
int main() {
    // Seed the random number generator
    srand(time(NULL));

    // Array to store pointers for later freeing
    void *pointers[100];
    int pointer_count = 0;

    // Test the custom allocator with random allocations for each bin
    for (int bin = 0; bin < NUM_BINS; bin++) {
        int count = (rand() % 10) + 1;  // Random count between 1 and 10
        for (int i = 0; i < count; i++) {
            size_t size = (bin * BIN_SIZE) + (rand() % BIN_SIZE);  // Random size within the bin
            void *ptr = my_malloc(size);
            if (ptr && pointer_count < 100) { // Store the pointer for later freeing
                pointers[pointer_count++] = ptr;
            }
        }
    }

    // Free some of the allocated memory to create fragmentation
    for (int i = 0; i < pointer_count; i += 2) { // Free every other pointer
        my_free(pointers[i]);
    }

    // Print statistics and histogram
    print_statistics();
    print_histogram();

    // Free the remaining allocations
    for (int i = 1; i < pointer_count; i += 2) {
        my_free(pointers[i]);
    }

    // Final statistics after freeing all
    print_statistics();
    print_histogram();

    return 0;
}

/**
 * Function Implementations.
 */

/**
 * Adds an allocation to the global list and updates the histogram.
 */
void add_allocation(void *address, size_t size) {
    pthread_mutex_lock(&lock);
    Allocation *new_alloc = malloc(sizeof(Allocation));
    if (!new_alloc) {
        perror("Error allocating memory for metadata");
        pthread_mutex_unlock(&lock);
        return;
    }
    new_alloc->address = address;
    new_alloc->size = size;
    new_alloc->next = allocations;
    allocations = new_alloc;
    total_allocated += size;
    update_histogram(size); // Update histogram for allocation size
    pthread_mutex_unlock(&lock);
}

/**
 * Removes an allocation from the global list.
 */
void remove_allocation(void *address) {
    Allocation **current = &allocations;

    while (*current) {
        if ((*current)->address == address) {
            Allocation *to_free = *current;
            total_freed += to_free->size; // Update total freed memory
            // printf("Freed %zu bytes. Total Freed: %zu bytes\n", to_free->size, total_freed);
            *current = (*current)->next; // Update the list pointer
            free(to_free);               // Free the allocation metadata
            return;                      // Exit after removing the node
        }
        current = &(*current)->next;     // Move to the next node
    }
}

/**
 * Updates the histogram for a given allocation size.
 */
void update_histogram(size_t size) {
    int bin = size / BIN_SIZE;
    if (bin < NUM_BINS) {
        histogram[bin]++;
    }
}

/**
 * Custom malloc wrapper.
 */
void *my_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) {
        add_allocation(ptr, size);
    }
    return ptr;
}

/**
 * Custom free wrapper.
 */
void my_free(void *ptr) {
    if (ptr) {
        // printf("Attempting to free memory at %p\n", ptr);
        pthread_mutex_lock(&lock);

        remove_allocation(ptr);
        // printf("Removed allocation for %p\n", ptr);

        free(ptr);
        // printf("Freed memory at %p\n", ptr);

        if (allocations == NULL) {
            printf("All memory freed. Resetting histogram.\n");
            memset(histogram, 0, sizeof(histogram));
        }

        pthread_mutex_unlock(&lock);
        // printf("Completed free for %p\n", ptr);
    }
}


/**
 * Custom realloc wrapper.
 */
void *my_realloc(void *ptr, size_t size) {
    if (!ptr) {
        return my_malloc(size); // Equivalent to malloc
    }
    if (size == 0) {
        my_free(ptr); // Equivalent to free
        return NULL;
    }
    void *new_ptr = realloc(ptr, size);
    if (new_ptr) {
        remove_allocation(ptr);
        add_allocation(new_ptr, size);
    }
    return new_ptr;
}

/**
 * Prints heap allocation statistics.
 */
void print_statistics() {
    pthread_mutex_lock(&lock);

    printf("=== Heap Statistics ===\n");
    printf("Total Allocated: %zu bytes\n", total_allocated);
    printf("Total Freed: %zu bytes\n", total_freed);
    printf("Net Usage: %zu bytes\n", total_allocated - total_freed);

    // Calculate total active allocation size
    size_t active_allocations = 0;
    Allocation *current = allocations;
    while (current) {
        active_allocations += current->size;
        current = current->next;
    }

    // Free blocks = Total allocated memory - Active allocations
    size_t free_blocks = total_allocated - active_allocations;

    // Calculate fragmentation
    double fragmentation = 0.0;
    if (active_allocations > 0) {
        fragmentation = (free_blocks / (double)total_allocated) * 100;
    }

    printf("Fragmentation: %.2f%%\n", fragmentation);

    pthread_mutex_unlock(&lock);
}



/**
 * Prints the histogram of allocation sizes.
 */
void print_histogram() {
    printf("\n=== Allocation Size Histogram ===\n");
    for (int i = 0; i < NUM_BINS; i++) {
        printf("%3d-%3d bytes: ", i * BIN_SIZE, (i + 1) * BIN_SIZE - 1);
        for (size_t j = 0; j < histogram[i]; j++) {
            printf("#");
        }
        printf(" (%zu)\n", histogram[i]); // Number of allocations in the bin
    }
    printf("\n");
}

