#ifndef COMMON_H_
#define COMMON_H_

char *num_to_str(char* buf, uint64_t num, int flags);
char *gen_title(char *buf, const char *mem, int flags);
void set_timer(uint32_t usec);
void config_curses(void);
void print_bar(uint32_t col, uint32_t used, uint32_t last);

#endif /* COMMON_H_ */
