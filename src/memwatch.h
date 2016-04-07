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
#include <sys/time.h>

#define BUFDEC (sizeof(uint64_t) * 4)
#define UNUSED __attribute__((unused))

#define MEMFILE "/proc/meminfo"
#define BAR_LEHGTH 30
#define DEFAULT_DELAY 1000
#define DELAY_MIN 100
#define DELAY_MAX 100000
#define DELIM 1024
#ifndef VERSION
#define VERSION "0.1.0"
#endif

enum {
    FLAG_DELAY = 1
};

typedef struct options_s {
    uint32_t delay;
    int flags;
} options_t;

typedef struct mem_s {
     uint64_t mem_total, mem_free, mem_used;
     uint64_t mem_buff, mem_cache;
     uint64_t swap_total, swap_free, swap_used;
     int swap_disabled;
} mem_t;

enum {
     FREE_MEM,
     TOTAL_MEM,
     FREE_SWAP,
     TOTAL_SWAP,
     BUFF_MEM,
     CACHE_MEM
};

void set_timer(uint32_t);
void get_data(int);
void insert_value(mem_t *, char *, int);
void print_info(const mem_t *);
void print_bar(uint32_t, uint32_t, uint32_t);
char *num_to_str(char*, uint64_t);

#endif /* MEMWATCH_H_ */
