#ifndef COMMON_H_
#define COMMON_H_

char *num_to_str(char* buf, size_t len, uint64_t num, const options_t *opt);
char *gen_title(char *buf, const char *mem, int flags);
void set_timer(uint32_t usec);
void config_curses(void);
void print_bar(uint32_t col, uint32_t used, uint32_t last);
void clear_screen(void);
void grep_digits(char *dst, const char *src, size_t len);
int dirname_only_digits(const char *name);
void err_exit(const char *fmt, ...);

#endif /* COMMON_H_ */
