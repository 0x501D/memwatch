#ifndef MEMWATCH_H_
#define MEMWATCH_H_

#define BUFDEC (sizeof(uint64_t) * 4)
#define MEMFILE "/proc/meminfo"
#define BAR_LEHGTH 30
#define DELAY 1000
#define DELAY_MIN 100
#define DELAY_MAX 100000
#define DELIM 1024
#ifndef VERSION
#define VERSION "0.0.2"
#endif

typedef struct mem_s {
     uint64_t mem_total, mem_free, mem_used, mem_avail;
     uint64_t swap_total, swap_free, swap_used;
     int swap_disabled;
} mem_t;

enum {
     FREE_MEM,
     TOTAL_MEM,
     FREE_SWAP,
     TOTAL_SWAP,
     AVAIL_MEM
};

void set_timer(uint32_t);
void get_data(int);
void insert_value(mem_t *, char *, int);
void print_info(const mem_t *);
void clear_data(mem_t *);
void print_bar(uint32_t, uint32_t, uint32_t);
char *num_to_str(char *, uint64_t);

#endif /* MEMWATCH_H_ */
