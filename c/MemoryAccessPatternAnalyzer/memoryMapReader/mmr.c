#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // isatty()
#include <math.h>    // fmax, lrint

#define LINE_LENGTH 256
#define LARGE_ANONYMOUS_THRESHOLD (1024 * 1024)  // 1 MB
#define MAX_PATHNAME_WIDTH 40  // cap pathname column width

// ANSI color codes for terminal output
#define RESET  "\033[0m"
#define GREEN  "\033[32m"
#define BLUE   "\033[34m"
#define YELLOW "\033[33m"
#define RED    "\033[31m"

// ===== Memory map record =====
typedef struct {
    char start_address[16];
    char end_address[16];
    char permissions[5];
    char offset[9];
    char device[6];
    char inode[10];
    char pathname[256];
    size_t size;  // region size in bytes (end-start)
} MemoryMap;

// ===== Helpers for ASCII map =====
#define MAP_W 60
static size_t g_max_region_size = 0; // track maximum region size observed

static const char* color_for(const MemoryMap *m) {
    if (strcmp(m->pathname, "[heap]") == 0) return GREEN;
    if (strcmp(m->pathname, "[stack]") == 0) return BLUE;
    if (strstr(m->permissions, "x"))         return YELLOW;
    if (strcmp(m->pathname, "Anonymous") == 0 && m->size > LARGE_ANONYMOUS_THRESHOLD) return RED;
    return RESET;
}

static void ascii_bar(size_t val, char out[MAP_W+1]) {
    int n = (g_max_region_size==0) ? 0 : (int)lrint((double)val / (double)g_max_region_size * MAP_W);
    if (n < 0) n = 0; if (n > MAP_W) n = MAP_W;
    for (int i=0; i<MAP_W; i++) out[i] = (i<n ? '#' : ' ');
    out[MAP_W] = '\0';
}

static const char* short_label(const char *path) {
    const char *s = path;
    const char *slash = strrchr(s, '/');
    if (slash && slash[1]) return slash+1;  // basename for long paths
    return s; // labels like [heap], [stack], or already-short names
}

// ===== Optional ncurses TUI =====
#ifdef ENABLE_NCURSES
#include <ncurses.h>
typedef struct {
    size_t size;
    char perm[5];
    char label[64];
} Row;

static void run_ncurses_view(const char *maps_path) {
    FILE *fp = fopen(maps_path, "r");
    if (!fp) { perror("open maps"); return; }

    Row *rows = NULL; size_t n=0, cap=0;
    char line[LINE_LENGTH];
    MemoryMap m; size_t vmax=0;

    // Reuse parse_line to load rows
    while (fgets(line, sizeof(line), fp)) {
        // parse_line declared below
        // (forward declaration not needed since we compile as one TU)
        unsigned long saddr, eaddr;
        char tmpperm[5], tmpoff[9], tmpdev[6], tmpino[10], temp[256];
        m.pathname[0] = '\0';
        int fields = sscanf(line, "%lx-%lx %4s %8s %5s %9s %255[^\n]",
                            &saddr, &eaddr, tmpperm, tmpoff, tmpdev, tmpino, temp);
        snprintf(m.start_address, sizeof(m.start_address), "%lx", saddr);
        snprintf(m.end_address,   sizeof(m.end_address),   "%lx", eaddr);
        strncpy(m.permissions, tmpperm, sizeof(m.permissions));
        m.permissions[sizeof(m.permissions)-1] = '\0';
        strncpy(m.offset, tmpoff, sizeof(m.offset));
        m.offset[sizeof(m.offset)-1] = '\0';
        strncpy(m.device, tmpdev, sizeof(m.device));
        m.device[sizeof(m.device)-1] = '\0';
        strncpy(m.inode, tmpino, sizeof(m.inode));
        m.inode[sizeof(m.inode)-1] = '\0';
        m.size = eaddr - saddr;
        if (fields >= 7) {
            char *st = temp;
            while (*st && (*st==' '||*st=='\t'||(*st>='0'&&*st<='9'))) st++;
            while (*st==' '||*st=='\t') st++;
            strncpy(m.pathname, st, sizeof(m.pathname)-1);
            m.pathname[sizeof(m.pathname)-1] = '\0';
            char *endp = m.pathname + strlen(m.pathname) - 1;
            while (endp >= m.pathname && (*endp==' '||*endp=='\t'||*endp=='\n')) { *endp='\0'; endp--; }
        }
        if (fields < 7 || m.pathname[0]=='\0' || strspn(m.pathname," \t\n")==strlen(m.pathname)) {
            strcpy(m.pathname, "Anonymous");
        }

        if (n==cap) {
            cap = cap ? cap*2 : 256;
            Row *nr = realloc(rows, cap*sizeof(*nr));
            if (!nr) { perror("realloc"); free(rows); fclose(fp); return; }
            rows = nr;
        }
        rows[n].size = m.size;
        strncpy(rows[n].perm, m.permissions, sizeof(rows[n].perm));
        rows[n].perm[sizeof(rows[n].perm)-1] = '\0';
        snprintf(rows[n].label, sizeof(rows[n].label), "%s", short_label(m.pathname));
        if (m.size > vmax) vmax = m.size;
        n++;
    }
    fclose(fp);

    initscr(); cbreak(); noecho(); keypad(stdscr, TRUE);
    int h, w; getmaxyx(stdscr, h, w);
    int top=0;
    int barw = (w > 40 ? w-32 : (w>10 ? w-10 : 10)); // leave room for text

    while (1) {
        clear();
        mvprintw(0,0,"Virtual Memory Map (n=%zu, max=%zu bytes). Up/Down/PgUp/PgDn, q=quit", n, vmax);
        int rowy=2;
        for (size_t i=top; i<n && rowy<h-1; ++i, ++rowy) {
            int fill = (vmax==0) ? 0 : (int)lrint((double)rows[i].size/(double)vmax * barw);
            if (fill<0) fill=0; if (fill>barw) fill=barw;
            for (int x=0; x<barw; x++) mvaddch(rowy, x, x<fill ? '#' : ' ');
            mvprintw(rowy, barw+1, "%8.2f MB  %-4s  %-*s",
                     rows[i].size/(1024.0*1024.0), rows[i].perm,
                     (w - (barw+1) - 20 > 0 ? w - (barw+1) - 20 : 0), rows[i].label);
        }
        int ch = getch();
        if (ch=='q' || ch=='Q') break;
        if (ch==KEY_DOWN)  { if (top+1 < (int)n) top++; }
        if (ch==KEY_UP)    { if (top>0) top--; }
        if (ch==KEY_NPAGE) { top += (h-3); if (top >= (int)n) top = (int)n-1; if (top<0) top=0; }
        if (ch==KEY_PPAGE) { top -= (h-3); if (top<0) top=0; }
    }
    endwin();
    free(rows);
}
#endif // ENABLE_NCURSES

// ===== Your existing parsing & table code =====

// Parse a /proc/[pid]/maps line into MemoryMap
static void parse_line(const char *line, MemoryMap *map) {
    unsigned long start, end;
    char temp[256]; // capture tail

    map->pathname[0] = '\0';

    int fields = sscanf(line, "%lx-%lx %4s %8s %5s %9s %255[^\n]",
                        &start, &end,
                        map->permissions,
                        map->offset,
                        map->device,
                        map->inode,
                        temp);

    snprintf(map->start_address, sizeof(map->start_address), "%lx", start);
    snprintf(map->end_address,   sizeof(map->end_address),   "%lx", end);
    map->size = end - start;

    if (fields >= 7) {
        char *st = temp;
        while (*st && (*st==' '||*st=='\t'||(*st>='0'&&*st<='9'))) st++;
        while (*st==' '||*st=='\t') st++;
        strncpy(map->pathname, st, sizeof(map->pathname)-1);
        map->pathname[sizeof(map->pathname)-1] = '\0';
        char *end_ptr = map->pathname + strlen(map->pathname) - 1;
        while (end_ptr >= map->pathname && (*end_ptr==' '||*end_ptr=='\t'||*end_ptr=='\n')) {
            *end_ptr = '\0'; end_ptr--;
        }
    }
    if (fields < 7 || map->pathname[0]=='\0' || strspn(map->pathname, " \t\n")==strlen(map->pathname)) {
        strcpy(map->pathname, "Anonymous");
    }
}

// Dynamic column widths
static void calculate_column_widths(FILE *file, int *col_widths, int size_in_mb) {
    char line[LINE_LENGTH];
    MemoryMap map;
    memset(col_widths, 0, 8 * sizeof(int));

    while (fgets(line, sizeof(line), file)) {
        parse_line(line, &map);

        col_widths[0] = (int)fmax(col_widths[0], (int)strlen(map.start_address));
        col_widths[1] = (int)fmax(col_widths[1], (int)strlen(map.end_address));
        col_widths[2] = (int)fmax(col_widths[2], (int)strlen(map.permissions));
        col_widths[3] = (int)fmax(col_widths[3], (int)strlen(map.offset));
        col_widths[4] = (int)fmax(col_widths[4], (int)strlen(map.device));
        col_widths[5] = (int)fmax(col_widths[5], (int)strlen(map.inode));

        double size = size_in_mb ? map.size / (1024.0 * 1024.0) : (double)map.size;
        char size_str[32];
        snprintf(size_str, sizeof(size_str), "%.2f %s", size, size_in_mb ? "MB" : "Bytes");
        col_widths[6] = (int)fmax(col_widths[6], (int)strlen(size_str));

        int pnw = (int)fmin((double)strlen(map.pathname), (double)MAX_PATHNAME_WIDTH);
        col_widths[7] = (int)fmax(col_widths[7], pnw);
    }

    col_widths[0] = (int)fmax(col_widths[0], (int)strlen("Start Address"));
    col_widths[1] = (int)fmax(col_widths[1], (int)strlen("End Address"));
    col_widths[2] = (int)fmax(col_widths[2], (int)strlen("Perm"));
    col_widths[3] = (int)fmax(col_widths[3], (int)strlen("Offset"));
    col_widths[4] = (int)fmax(col_widths[4], (int)strlen("Dev"));
    col_widths[5] = (int)fmax(col_widths[5], (int)strlen("Inode"));
    col_widths[6] = (int)fmax(col_widths[6], (int)strlen("Size"));
    col_widths[7] = (int)fmax(col_widths[7], (int)strlen("Pathname"));

    rewind(file);
}

// Print table header
static void print_table_header(FILE *output, int *col_widths) {
    fprintf(output, "%-*s %-*s %-*s %-*s %-*s %-*s %-*s %-*s\n",
            col_widths[0], "Start Address",
            col_widths[1], "End Address",
            col_widths[2], "Perm",
            col_widths[3], "Offset",
            col_widths[4], "Dev",
            col_widths[5], "Inode",
            col_widths[6], "Size",
            col_widths[7], "Pathname");

    int total_width = col_widths[0]+col_widths[1]+col_widths[2]+col_widths[3]+
                      col_widths[4]+col_widths[5]+col_widths[6]+col_widths[7] + 7;
    for (int i=0; i<total_width; i++) fputc('-', output);
    fputc('\n', output);
}

// Print one row
static void print_memory_map(FILE *output, MemoryMap *map, int *col_widths, int size_in_mb, int use_color) {
    const char *color = "";
    const char *reset = "";

    if (use_color) {
        color = color_for(map);
        reset = RESET;
    }

    double sz = size_in_mb ? map->size / (1024.0 * 1024.0) : (double)map->size;
    char size_str[32];
    snprintf(size_str, sizeof(size_str), "%.2f %s", sz, size_in_mb ? "MB" : "Bytes");

    char truncated_pathname[256];
    if (strlen(map->pathname) > MAX_PATHNAME_WIDTH) {
        strncpy(truncated_pathname, map->pathname, MAX_PATHNAME_WIDTH - 3);
        truncated_pathname[MAX_PATHNAME_WIDTH - 3] = '\0';
        strcat(truncated_pathname, "...");
    } else {
        strcpy(truncated_pathname, map->pathname);
    }

    fprintf(output, "%s%-*s %-*s %-*s %-*s %-*s %-*s %-*s %-*s%s\n",
            color,
            col_widths[0], map->start_address,
            col_widths[1], map->end_address,
            col_widths[2], map->permissions,
            col_widths[3], map->offset,
            col_widths[4], map->device,
            col_widths[5], map->inode,
            col_widths[6], size_str,
            col_widths[7], truncated_pathname[0] ? truncated_pathname : "Anonymous",
            reset);
}

// ===== Main with new flags =====
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pid> [all] [--size-mb] [--output <file>] [--ascii-map] [--ncurses]\n", argv[0]);
        return 1;
    }

    int show_all = 0;      // reserved for future filtering
    int size_in_mb = 0;
    int want_ascii_map = 0;
    int want_ncurses = 0;
    char *output_filename = NULL;

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "all") == 0) {
            show_all = 1;
        } else if (strcmp(argv[i], "--size-mb") == 0) {
            size_in_mb = 1;
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_filename = argv[++i];
        } else if (strcmp(argv[i], "--ascii-map") == 0) {
            want_ascii_map = 1;
        } else if (strcmp(argv[i], "--ncurses") == 0) {
            want_ncurses = 1;
        }
    }

    FILE *output = stdout;
    int use_color = isatty(STDOUT_FILENO);

    if (output_filename) {
        output = fopen(output_filename, "w");
        if (!output) { perror("Error opening output file"); return 1; }
        use_color = 0; // avoid color codes in files
    }

    char path[64];
    snprintf(path, sizeof(path), "/proc/%s/maps", argv[1]);
    FILE *file = fopen(path, "r");
    if (!file) {
        perror("Error opening maps file");
        if (output_filename) fclose(output);
        return 1;
    }

    int col_widths[8];
    calculate_column_widths(file, col_widths, size_in_mb);

    print_table_header(output, col_widths);

    char line[LINE_LENGTH];
    MemoryMap map;

    g_max_region_size = 0;

    while (fgets(line, sizeof(line), file)) {
        parse_line(line, &map);

        // (Optional: apply show_all filter here if you add one later)

        if (map.size > g_max_region_size) g_max_region_size = map.size;
        print_memory_map(output, &map, col_widths, size_in_mb, use_color);
    }

    fclose(file);

    // ---- ASCII map (second pass) ----
    if (want_ascii_map) {
        FILE *f2 = fopen(path, "r");
        if (!f2) {
            perror("Error reopening maps for ascii map");
        } else {
            char l2[LINE_LENGTH];
            MemoryMap m2;
            int tty_colors = use_color && isatty(STDOUT_FILENO);

            printf("\n=== ASCII Memory Map (scaled to largest region: %zu bytes) ===\n", g_max_region_size);
            printf("[%-*s]  Size       Perm  Label\n", MAP_W, "bar");
            while (fgets(l2, sizeof(l2), f2)) {
                parse_line(l2, &m2);
                char bar[MAP_W+1]; ascii_bar(m2.size, bar);
                const char *col = tty_colors ? color_for(&m2) : "";
                const char *rst = tty_colors ? RESET : "";
                double sz_mb = m2.size / (1024.0*1024.0);
                printf("%s%s%s  %8.2f MB  %-4s  %s\n",
                       col, bar, rst, sz_mb, m2.permissions, short_label(m2.pathname));
            }
            fclose(f2);
        }
    }

    // ---- ncurses viewer ----
#ifdef ENABLE_NCURSES
    if (want_ncurses) {
        run_ncurses_view(path);
    }
#else
    if (want_ncurses) {
        fprintf(stderr, "--ncurses requested but binary not built with ncurses.\n"
                        "Rebuild with: gcc -O2 -DENABLE_NCURSES -o mmr mmr.c -lm -lncurses\n");
    }
#endif

    if (output_filename) fclose(output);
    return 0;
}

