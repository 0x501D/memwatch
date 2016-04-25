#ifndef COMMON_H_
#define COMMON_H_

char *num_to_str(char* buf, uint64_t num, int flags);
char *gen_title(char *buf, const char *mem, int flags);
void set_timer(uint32_t usec);
void config_curses(void);

#endif /* COMMON_H_ */
