# Virtual Memory Heatmap

Create a tool that reads `/proc/[pid]/smaps` and represents virtual memory regions as an ASCII heatmap or as CSV data for Python/Jupyter visualization.

## Build

```bash
gcc -Wall -Wextra -pedantic -std=c11 -o vmh vmh.c
```

## Usage

```bash
./vmh <pid> [--metric size|rss|pss|private-dirty|shared-clean] [--limit n] [--csv file] [--no-heatmap]
```

Examples:

```bash
./vmh 1234 --metric rss --limit 20
./vmh 1234 --metric pss --csv process-memory.csv
./vmh 1234 --csv process-memory.csv --no-heatmap
```

Metrics:

- `size`: virtual address space reserved by the mapping.
- `rss`: resident physical memory currently backed by RAM.
- `pss`: proportional set size; shared pages are divided across processes.
- `private-dirty`: private modified pages, useful for finding process-owned memory growth.
- `shared-clean`: clean pages shared with other processes, often shared libraries.

## CSV Columns

```text
start,end,permissions,type,pathname,size_kb,rss_kb,pss_kb,private_dirty_kb,shared_clean_kb
```

## Jupyter Example

```python
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("process-memory.csv")
top = df.sort_values("rss_kb", ascending=False).head(25)

plt.figure(figsize=(12, 8))
plt.barh(top["pathname"], top["rss_kb"])
plt.xlabel("RSS (kB)")
plt.ylabel("Memory region")
plt.title("Top Virtual Memory Regions by RSS")
plt.gca().invert_yaxis()
plt.tight_layout()
plt.show()
```

## Next Improvements

1. Add `--sort address|size|rss|pss`.
2. Add colorized terminal output by region type.
3. Add sampling mode to write repeated snapshots for time-series plots.
