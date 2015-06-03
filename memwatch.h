#ifndef MEMWATCH_H_
#define MEMWATCH_H_

#define BUFDEC (sizeof(uint64_t) * 4)
#define MEMFILE "/proc/meminfo"
#define BAR_LEHGTH 30
#define DELAY 1000
#define DELAY_MIN 100
#define DELAY_MAX 100000
#define DELIM 1024
#define VERSION "0.0.2"

typedef struct mem_s {
     char *mem_total_s, *mem_free_s, *mem_used_s;
     char *mem_avail_s, *mem_buff_s, *mem_cache_s;
     char *swap_total_s, *swap_free_s, *swap_used_s;
     uint64_t mem_total_d, mem_free_d, mem_used_d, mem_avail_d;
     uint64_t swap_total_d, swap_free_d, swap_used_d;
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

#endif /* MEMWATCH_H_ */
