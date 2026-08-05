#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 512
#define PATH_LENGTH 256
#define HEATMAP_WIDTH 50

typedef enum {
    METRIC_SIZE,
    METRIC_RSS,
    METRIC_PSS,
    METRIC_PRIVATE_DIRTY,
    METRIC_SHARED_CLEAN
} Metric;

typedef struct {
    unsigned long start;
    unsigned long end;
    char permissions[5];
    char pathname[PATH_LENGTH];
    char type[32];
    size_t size_kb;
    size_t rss_kb;
    size_t pss_kb;
    size_t private_dirty_kb;
    size_t shared_clean_kb;
} MemoryRegion;

typedef struct {
    MemoryRegion *items;
    size_t count;
    size_t capacity;
} RegionList;

static void usage(const char *program_name);
static int parse_metric(const char *text, Metric *metric);
static const char *metric_name(Metric metric);
static size_t metric_value(const MemoryRegion *region, Metric metric);
static int append_region(RegionList *regions, const MemoryRegion *region);
static int parse_mapping_header(const char *line, MemoryRegion *region);
static void trim_newline(char *text);
static void classify_region(MemoryRegion *region);
static int read_smaps(int pid, RegionList *regions);
static void print_summary(const RegionList *regions);
static void print_heatmap(const RegionList *regions, Metric metric, size_t limit);
static int write_csv(const char *filename, const RegionList *regions);
static void write_csv_field(FILE *file, const char *text);
static void free_regions(RegionList *regions);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    char *end = NULL;
    errno = 0;
    long pid_value = strtol(argv[1], &end, 10);
    if (errno != 0 || end == argv[1] || *end != '\0' || pid_value <= 0) {
        fprintf(stderr, "Invalid PID: %s\n", argv[1]);
        return 1;
    }

    Metric metric = METRIC_RSS;
    const char *csv_filename = NULL;
    size_t limit = 25;
    int show_heatmap = 1;

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--metric") == 0) {
            if (i + 1 >= argc || !parse_metric(argv[i + 1], &metric)) {
                fprintf(stderr, "Invalid or missing --metric value. Use size, rss, pss, private-dirty, or shared-clean.\n");
                return 1;
            }
            i++;
        } else if (strcmp(argv[i], "--csv") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Missing filename after --csv.\n");
                return 1;
            }
            csv_filename = argv[++i];
        } else if (strcmp(argv[i], "--limit") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Missing count after --limit.\n");
                return 1;
            }
            char *limit_end = NULL;
            errno = 0;
            unsigned long parsed_limit = strtoul(argv[++i], &limit_end, 10);
            if (errno != 0 || limit_end == argv[i] || *limit_end != '\0') {
                fprintf(stderr, "Invalid --limit value: %s\n", argv[i]);
                return 1;
            }
            limit = (size_t)parsed_limit;
        } else if (strcmp(argv[i], "--no-heatmap") == 0) {
            show_heatmap = 0;
        } else if (strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(argv[0]);
            return 1;
        }
    }

    RegionList regions = {0};
    if (!read_smaps((int)pid_value, &regions)) {
        free_regions(&regions);
        return 1;
    }

    print_summary(&regions);
    if (show_heatmap) {
        print_heatmap(&regions, metric, limit);
    }

    if (csv_filename && !write_csv(csv_filename, &regions)) {
        free_regions(&regions);
        return 1;
    }

    if (csv_filename) {
        printf("\nCSV written to: %s\n", csv_filename);
    }

    free_regions(&regions);
    return 0;
}

static void usage(const char *program_name) {
    fprintf(stderr,
            "Usage: %s <pid> [--metric size|rss|pss|private-dirty|shared-clean] [--limit n] [--csv file] [--no-heatmap]\n",
            program_name);
}

static int parse_metric(const char *text, Metric *metric) {
    if (strcmp(text, "size") == 0) {
        *metric = METRIC_SIZE;
    } else if (strcmp(text, "rss") == 0) {
        *metric = METRIC_RSS;
    } else if (strcmp(text, "pss") == 0) {
        *metric = METRIC_PSS;
    } else if (strcmp(text, "private-dirty") == 0) {
        *metric = METRIC_PRIVATE_DIRTY;
    } else if (strcmp(text, "shared-clean") == 0) {
        *metric = METRIC_SHARED_CLEAN;
    } else {
        return 0;
    }
    return 1;
}

static const char *metric_name(Metric metric) {
    switch (metric) {
        case METRIC_SIZE: return "Size";
        case METRIC_RSS: return "Rss";
        case METRIC_PSS: return "Pss";
        case METRIC_PRIVATE_DIRTY: return "Private_Dirty";
        case METRIC_SHARED_CLEAN: return "Shared_Clean";
    }
    return "Unknown";
}

static size_t metric_value(const MemoryRegion *region, Metric metric) {
    switch (metric) {
        case METRIC_SIZE: return region->size_kb;
        case METRIC_RSS: return region->rss_kb;
        case METRIC_PSS: return region->pss_kb;
        case METRIC_PRIVATE_DIRTY: return region->private_dirty_kb;
        case METRIC_SHARED_CLEAN: return region->shared_clean_kb;
    }
    return 0;
}

static int append_region(RegionList *regions, const MemoryRegion *region) {
    if (regions->count == regions->capacity) {
        size_t next_capacity = regions->capacity == 0 ? 64 : regions->capacity * 2;
        MemoryRegion *next_items = realloc(regions->items, next_capacity * sizeof(*next_items));
        if (!next_items) {
            perror("realloc");
            return 0;
        }
        regions->items = next_items;
        regions->capacity = next_capacity;
    }

    regions->items[regions->count++] = *region;
    return 1;
}

static int parse_mapping_header(const char *line, MemoryRegion *region) {
    unsigned long offset = 0;
    unsigned long inode = 0;
    char device[32];
    char pathname[PATH_LENGTH];

    memset(region, 0, sizeof(*region));
    pathname[0] = '\0';

    int fields = sscanf(line, "%lx-%lx %4s %lx %31s %lu %255[^\n]",
                        &region->start,
                        &region->end,
                        region->permissions,
                        &offset,
                        device,
                        &inode,
                        pathname);

    if (fields < 6) {
        return 0;
    }

    if (fields == 7) {
        trim_newline(pathname);
        snprintf(region->pathname, sizeof(region->pathname), "%s", pathname);
    } else {
        snprintf(region->pathname, sizeof(region->pathname), "[anonymous]");
    }

    classify_region(region);
    return 1;
}

static void trim_newline(char *text) {
    size_t length = strlen(text);
    while (length > 0 && (text[length - 1] == '\n' || text[length - 1] == '\r' || text[length - 1] == ' ' || text[length - 1] == '\t')) {
        text[--length] = '\0';
    }
}

static void classify_region(MemoryRegion *region) {
    if (strcmp(region->pathname, "[heap]") == 0) {
        snprintf(region->type, sizeof(region->type), "heap");
    } else if (strncmp(region->pathname, "[stack", 6) == 0) {
        snprintf(region->type, sizeof(region->type), "stack");
    } else if (strcmp(region->pathname, "[anonymous]") == 0) {
        snprintf(region->type, sizeof(region->type), "anonymous");
    } else if (region->pathname[0] == '[') {
        snprintf(region->type, sizeof(region->type), "kernel");
    } else if (strchr(region->permissions, 'x')) {
        snprintf(region->type, sizeof(region->type), "code");
    } else {
        snprintf(region->type, sizeof(region->type), "mapped_file");
    }
}

static int read_smaps(int pid, RegionList *regions) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/smaps", pid);

    FILE *file = fopen(path, "r");
    if (!file) {
        perror("Failed to open smaps");
        return 0;
    }

    char line[LINE_LENGTH];
    MemoryRegion current;
    int have_region = 0;

    while (fgets(line, sizeof(line), file)) {
        MemoryRegion parsed;
        if (parse_mapping_header(line, &parsed)) {
            if (have_region && !append_region(regions, &current)) {
                fclose(file);
                return 0;
            }
            current = parsed;
            have_region = 1;
            continue;
        }

        if (!have_region) {
            continue;
        }

        size_t value = 0;
        if (sscanf(line, "Size: %zu kB", &value) == 1) {
            current.size_kb = value;
        } else if (sscanf(line, "Rss: %zu kB", &value) == 1) {
            current.rss_kb = value;
        } else if (sscanf(line, "Pss: %zu kB", &value) == 1) {
            current.pss_kb = value;
        } else if (sscanf(line, "Private_Dirty: %zu kB", &value) == 1) {
            current.private_dirty_kb = value;
        } else if (sscanf(line, "Shared_Clean: %zu kB", &value) == 1) {
            current.shared_clean_kb = value;
        }
    }

    if (have_region && !append_region(regions, &current)) {
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

static void print_summary(const RegionList *regions) {
    size_t total_size = 0;
    size_t total_rss = 0;
    size_t total_pss = 0;
    size_t total_private_dirty = 0;
    size_t total_shared_clean = 0;

    for (size_t i = 0; i < regions->count; i++) {
        total_size += regions->items[i].size_kb;
        total_rss += regions->items[i].rss_kb;
        total_pss += regions->items[i].pss_kb;
        total_private_dirty += regions->items[i].private_dirty_kb;
        total_shared_clean += regions->items[i].shared_clean_kb;
    }

    printf("=== Virtual Memory Summary ===\n");
    printf("Regions:          %zu\n", regions->count);
    printf("Virtual Size:     %zu kB\n", total_size);
    printf("RSS:              %zu kB\n", total_rss);
    printf("PSS:              %zu kB\n", total_pss);
    printf("Private Dirty:    %zu kB\n", total_private_dirty);
    printf("Shared Clean:     %zu kB\n", total_shared_clean);
}

static void print_heatmap(const RegionList *regions, Metric metric, size_t limit) {
    size_t max_value = 0;
    size_t rows_printed = 0;

    for (size_t i = 0; i < regions->count; i++) {
        size_t value = metric_value(&regions->items[i], metric);
        if (value > max_value) {
            max_value = value;
        }
    }

    printf("\n=== ASCII Heatmap by %s (kB) ===\n", metric_name(metric));
    printf("%-50s  %10s  %-4s  %-14s  %s\n", "Heat", metric_name(metric), "Perm", "Type", "Path");

    for (size_t i = 0; i < regions->count; i++) {
        const MemoryRegion *region = &regions->items[i];
        size_t value = metric_value(region, metric);
        int fill = max_value == 0 ? 0 : (int)((double)value / (double)max_value * HEATMAP_WIDTH);
        char bar[HEATMAP_WIDTH + 1];

        if (limit != 0 && rows_printed >= limit) {
            printf("... %zu more regions. Use --limit 0 to print all.\n", regions->count - rows_printed);
            break;
        }

        for (int j = 0; j < HEATMAP_WIDTH; j++) {
            bar[j] = j < fill ? '#' : '.';
        }
        bar[HEATMAP_WIDTH] = '\0';

        printf("%s  %10zu  %-4s  %-14s  %s\n",
               bar,
               value,
               region->permissions,
               region->type,
               region->pathname);
        rows_printed++;
    }
}

static int write_csv(const char *filename, const RegionList *regions) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open CSV output");
        return 0;
    }

    fprintf(file, "start,end,permissions,type,pathname,size_kb,rss_kb,pss_kb,private_dirty_kb,shared_clean_kb\n");
    for (size_t i = 0; i < regions->count; i++) {
        const MemoryRegion *region = &regions->items[i];
        fprintf(file, "0x%lx,0x%lx,", region->start, region->end);
        write_csv_field(file, region->permissions);
        fputc(',', file);
        write_csv_field(file, region->type);
        fputc(',', file);
        write_csv_field(file, region->pathname);
        fprintf(file, ",%zu,%zu,%zu,%zu,%zu\n",
                region->size_kb,
                region->rss_kb,
                region->pss_kb,
                region->private_dirty_kb,
                region->shared_clean_kb);
    }

    fclose(file);
    return 1;
}

static void write_csv_field(FILE *file, const char *text) {
    fputc('"', file);
    for (const char *p = text; *p; p++) {
        if (*p == '"') {
            fputc('"', file);
        }
        fputc(*p, file);
    }
    fputc('"', file);
}

static void free_regions(RegionList *regions) {
    free(regions->items);
    regions->items = NULL;
    regions->count = 0;
    regions->capacity = 0;
}
