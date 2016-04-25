#ifndef MEMWATCH_H_
#define MEMWATCH_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <errno.h>
#include <libintl.h>
#include <locale.h>
#include <sys/time.h>

#define PACKAGENAME "memwatch"
#define UNUSED __attribute__((unused))
#define _(text) gettext(text)

#ifndef LOCALEDIR
#define LOCALEDIR "/usr/share/locale"
#endif

#define MEMFILE "/proc/meminfo"
#define BAR_LEHGTH 30
#define DEFAULT_DELAY 1000
#define DEFAULT_DELIM 1024
#define DELAY_MIN 100
#define DELAY_MAX 100000
#define CLEAR_SZ_FLAGS 0x1e

#ifndef VERSION
#define VERSION "0.2.0"
#endif

enum {
    DELAY_FL     = 1,
    MEGABYTES_FL = 1 << 1,
    GIGABYTES_FL = 1 << 2,
    KILOBYTES_FL = 1 << 3,
    BYTES_FL     = 1 << 4
};

typedef struct options_s {
    int32_t delay;
    int flags;
} options_t;

typedef struct mem_s {
    uint64_t mem_total, mem_free, mem_used;
    uint64_t mem_buff, mem_cache;
    uint64_t swap_total, swap_free, swap_used;
    int swap_disabled;
} mem_t;

typedef struct bar_s {
    float mem_ratio, mem_bar_used, swap_ratio;
    float swap_bar_used, swap_bar_free;
    uint32_t mem_bar_free;
} bar_t;

#endif /* MEMWATCH_H_ */
