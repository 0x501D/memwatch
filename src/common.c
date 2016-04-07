#include <memwatch.h>

char *num_to_str(char* buf, uint64_t num)
{
    snprintf(buf, BUFDEC, "%lu", num);
    return buf;
}

void set_timer(uint32_t usec)
{
    struct itimerval timer;

    uint32_t msec = (usec % 1000) * 1000;
    uint32_t sec  = usec / 1000;

    timer.it_interval.tv_sec = sec;
    timer.it_interval.tv_usec = msec;
    timer.it_value.tv_sec = sec;
    timer.it_value.tv_usec = msec;

    setitimer(ITIMER_REAL, &timer, NULL);
}

void config_curses(void)
{
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    clear();
}
