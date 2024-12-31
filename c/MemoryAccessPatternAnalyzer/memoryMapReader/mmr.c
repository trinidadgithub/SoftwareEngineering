#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For isatty()

#define LINE_LENGTH 256
#define LARGE_ANONYMOUS_THRESHOLD 1024 * 1024  // 1 MB

// ANSI color codes for terminal output
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define RED "\033[31m"

// Struct to hold memory map information
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

// Function to calculate column widths
void calculate_column_widths(FILE *file, int *col_widths) {
    char line[LINE_LENGTH];
    MemoryMap map;
    memset(col_widths, 0, 7 * sizeof(int));

    while (fgets(line, sizeof(line), file)) {
        unsigned long start, end;
        sscanf(line, "%lx-%lx %4s %8s %5s %9s %255[^\n]",
               &start, &end, map.permissions, map.offset, map.device, map.inode, map.pathname);

        char start_address[16], end_address[16];
        snprintf(start_address, sizeof(start_address), "%lx", start);
        snprintf(end_address, sizeof(end_address), "%lx", end);

        col_widths[0] = fmax(col_widths[0], strlen(start_address));         // Start Address
        col_widths[1] = fmax(col_widths[1], strlen(end_address));           // End Address
        col_widths[2] = fmax(col_widths[2], strlen(map.permissions));       // Permissions
        col_widths[3] = fmax(col_widths[3], strlen(map.offset));            // Offset
        col_widths[4] = fmax(col_widths[4], strlen(map.device));            // Device
        col_widths[5] = fmax(col_widths[5], strlen(map.inode));             // Inode
        col_widths[6] = fmax(col_widths[6], strlen(map.pathname));          // Pathname
    }

    rewind(file); // Reset file pointer
}

// Function to print table header
void print_table_header(FILE *output, int *col_widths, int size_in_mb) {
    fprintf(output, "%-*s %-*s %-*s %-*s %-*s %-*s %-*s %-*s\n",
            col_widths[0], "Start Address",
            col_widths[1], "End Address",
            col_widths[2], "Perm",
            col_widths[3], "Offset",
            col_widths[4], "Dev",
            col_widths[5], "Inode",
            12, size_in_mb ? "Size (MB)" : "Size (Bytes)",
            col_widths[6], "Pathname");
    for (int i = 0; i < col_widths[0] + col_widths[1] + col_widths[2] + col_widths[3] + col_widths[4] + col_widths[5] + 12 + col_widths[6] + 8; i++) {
        fputc('-', output);
    }
    fputc('\n', output);
}

// Function to print a memory map row
void print_memory_map(FILE *output, MemoryMap *map, int *col_widths, int size_in_mb, int use_color) {
    const char *color = "";
    const char *reset = "";

    if (use_color) {
        // Assign colors based on segment type
        if (strcmp(map->pathname, "[heap]") == 0) {
            color = GREEN;
        } else if (strcmp(map->pathname, "[stack]") == 0) {
            color = BLUE;
        } else if (strstr(map->permissions, "x")) {
            color = YELLOW;
        } else if (strcmp(map->pathname, "Anonymous") == 0 && map->size > LARGE_ANONYMOUS_THRESHOLD) {
            color = RED;
        }
        reset = RESET;
    }

    double size = size_in_mb ? map->size / (1024.0 * 1024.0) : map->size;
    const char *size_unit = size_in_mb ? "MB" : "Bytes";

    fprintf(output, "%s%-*s %-*s %-*s %-*s %-*s %-*s %-12.2f %-*s%s\n",
            color,
            col_widths[0], map->start_address,
            col_widths[1], map->end_address,
            col_widths[2], map->permissions,
            col_widths[3], map->offset,
            col_widths[4], map->device,
            col_widths[5], map->inode,
            size,
            col_widths[6], map->pathname[0] ? map->pathname : "Anonymous",
            reset);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pid> [all] [--size-mb] [--output <filename>]\n", argv[0]);
        return 1;
    }

    int show_all = 0;
    int size_in_mb = 0;
    char *output_filename = NULL;

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "all") == 0) {
            show_all = 1;
        } else if (strcmp(argv[i], "--size-mb") == 0) {
            size_in_mb = 1;
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_filename = argv[++i];
        }
    }

    FILE *output = stdout;
    int use_color = isatty(STDOUT_FILENO);

    if (output_filename) {
        output = fopen(output_filename, "w");
        if (!output) {
            perror("Error opening output file");
            return 1;
        }
        use_color = 0;
    }

    char path[64];
    snprintf(path, sizeof(path), "/proc/%s/maps", argv[1]);
    FILE *file = fopen(path, "r");
    if (!file) {
        perror("Error opening maps file");
        if (output_filename) fclose(output);
        return 1;
    }

    int col_widths[7];
    calculate_column_widths(file, col_widths);

    print_table_header(output, col_widths, size_in_mb);

    char line[LINE_LENGTH];
    MemoryMap map;

    while (fgets(line, sizeof(line), file)) {
        parse_line(line, &map);
        print_memory_map(output, &map, col_widths, size_in_mb, use_color);
    }

    fclose(file);
    if (output_filename) fclose(output);

    return 0;
}

