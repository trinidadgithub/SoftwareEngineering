#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define HEATMAP_WIDTH 50

// Function to generate a heatmap bar
void generate_heatmap_bar(size_t size, size_t max_size, char *bar) {
    int bar_length = (int)((double)size / max_size * HEATMAP_WIDTH);
    for (int i = 0; i < HEATMAP_WIDTH; i++) {
        bar[i] = (i < bar_length) ? '#' : ' ';
    }
    bar[HEATMAP_WIDTH] = '\0';
}

// Function to parse and display memory regions from /proc/[pid]/smaps
void display_memory_heatmap(int pid) {
    char smaps_path[64];
    snprintf(smaps_path, sizeof(smaps_path), "/proc/%d/smaps", pid);

    FILE *file = fopen(smaps_path, "r");
    if (!file) {
        perror("Error opening smaps file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    size_t total_size = 0;
    size_t max_size = 0;
    size_t region_sizes[100] = {0}; // Array to store sizes for up to 100 regions
    char *region_names[100] = {0}; // Array to store names of up to 100 regions
    int region_count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Size:")) {
            size_t size;
            sscanf(line, "Size: %zu kB", &size);
            total_size += size;
            if (size > max_size) max_size = size;
            region_sizes[region_count] = size;
        } else if (strchr(line, '-') && region_count < 100) {
            region_names[region_count] = strdup(line);
            region_count++;
        }
    }
    fclose(file);

    printf("\n=== Virtual Memory Heatmap for PID: %d ===\n", pid);
    printf("Total Memory: %zu kB\n\n", total_size);

    // Display heatmap
    for (int i = 0; i < region_count; i++) {
        if (region_sizes[i] > 0) {
            char heatmap_bar[HEATMAP_WIDTH + 1];
            generate_heatmap_bar(region_sizes[i], max_size, heatmap_bar);
            printf("%s[%s] %zu kB\n", heatmap_bar, region_names[i], region_sizes[i]);
            free(region_names[i]);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);
    display_memory_heatmap(pid);

    return 0;
}

