#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SMAPS_FILE_TEMPLATE "/proc/%d/smaps"
#define MEMINFO_FILE "/proc/meminfo"
#define BUFFER_SIZE 256

void parse_meminfo() {
    FILE *file = fopen(MEMINFO_FILE, "r");
    if (!file) {
        perror("Failed to open /proc/meminfo");
        return;
    }

    char line[BUFFER_SIZE];
    printf("=== System Memory Information ===\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

void parse_smaps(int pid) {
    char filepath[BUFFER_SIZE];
    snprintf(filepath, sizeof(filepath), SMAPS_FILE_TEMPLATE, pid);

    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Failed to open /proc/[pid]/smaps");
        return;
    }

    char line[BUFFER_SIZE];
    printf("\n=== Memory Regions for PID: %d ===\n", pid);
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Size:") || strstr(line, "Rss:") || strstr(line, "Pss:")) {
            printf("%s", line);
        }
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <pid>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);
    if (pid <= 0) {
        printf("Invalid PID: %s\n", argv[1]);
        return 1;
    }

    parse_meminfo();
    parse_smaps(pid);

    return 0;
}

