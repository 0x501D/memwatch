#include <memwatch.h>

char *num_to_str(char* buf, uint64_t num)
{
    snprintf(buf, BUFDEC, "%lu", num);
    return buf;
}

void config_curses(void)
{
    initscr();
    noecho();
    timeout(0);
    curs_set(0);
    cbreak();
    clear();
}
