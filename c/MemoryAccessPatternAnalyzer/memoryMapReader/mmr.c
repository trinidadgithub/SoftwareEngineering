#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256
#define LARGE_ANONYMOUS_THRESHOLD 1024 * 1024  // 1 MB

typedef struct {
    char start_address[16];
    char end_address[16];
    char permissions[5];
    char offset[9];
    char device[6];
    char inode[10];
    char pathname[256];
    size_t size;  // Size of the memory region in bytes
} MemoryMap;

void print_memory_map(MemoryMap *map, int show_all, int size_in_mb) {
    if (!show_all && strcmp(map->pathname, "Anonymous") && 
        strcmp(map->pathname, "[heap]") && 
        strcmp(map->pathname, "[stack]")) {
        return;
    }

    if (size_in_mb) {
        double size_mb = map->size / (1024.0 * 1024.0);  // Convert bytes to MB
        printf("%-15s %-15s %-5s %-8s %-6s %-8s %-10.2f MB %s%s\n",
               map->start_address,
               map->end_address,
               map->permissions,
               map->offset,
               map->device,
               map->inode,
               size_mb,
               map->pathname[0] ? map->pathname : "Anonymous",
               (map->size > LARGE_ANONYMOUS_THRESHOLD && strcmp(map->pathname, "Anonymous") == 0) ? " [LARGE]" : "");
    } else {
        printf("%-15s %-15s %-5s %-8s %-6s %-8s %-10lu %s%s\n",
               map->start_address,
               map->end_address,
               map->permissions,
               map->offset,
               map->device,
               map->inode,
               map->size,
               map->pathname[0] ? map->pathname : "Anonymous",
               (map->size > LARGE_ANONYMOUS_THRESHOLD && strcmp(map->pathname, "Anonymous") == 0) ? " [LARGE]" : "");
    }
}

void parse_line(const char *line, MemoryMap *map) {
    unsigned long start, end;
    
    // Extract fields using sscanf
    sscanf(line, "%lx-%lx %4s %8s %5s %9s %255[^\n]",
           &start, &end,
           map->permissions,
           map->offset,
           map->device,
           map->inode,
           map->pathname);

    // Convert addresses and calculate size
    snprintf(map->start_address, sizeof(map->start_address), "%lx", start);
    snprintf(map->end_address, sizeof(map->end_address), "%lx", end);
    map->size = end - start;

    // Set pathname to "Anonymous" if not specified
    if (strlen(map->pathname) == 0) {
        strcpy(map->pathname, "Anonymous");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        fprintf(stderr, "Usage: %s <pid> [all] [--size-mb]\n", argv[0]);
        return 1;
    }

    int show_all = 0;
    int size_in_mb = 0;

    // Parse optional arguments
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "all") == 0) {
            show_all = 1;
        } else if (strcmp(argv[i], "--size-mb") == 0) {
            size_in_mb = 1;
        }
    }

    char path[64];
    snprintf(path, sizeof(path), "/proc/%s/maps", argv[1]);
    FILE *file = fopen(path, "r");
    if (!file) {
        perror("Error opening maps file");
        return 1;
    }

    char line[LINE_LENGTH];
    MemoryMap map;

    // Print table headers
    printf("%-15s %-15s %-5s %-8s %-6s %-8s %-10s %s\n",
           "Start Address", "End Address", "Perm", "Offset", "Dev", "Inode", size_in_mb ? "Size (MB)" : "Size (Bytes)", "Pathname");
    printf("---------------------------------------------------------------------------------------------------------\n");

    // Read and parse each line
    while (fgets(line, sizeof(line), file)) {
        parse_line(line, &map);
        print_memory_map(&map, show_all, size_in_mb);
    }

    fclose(file);
    return 0;
}
