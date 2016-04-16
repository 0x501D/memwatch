#include <memwatch.h>

char *num_to_str(char* buf, uint64_t num)
{
    snprintf(buf, BUFDEC, "%lu", num);
    return buf;
}

char *gen_title(char *buf, const char *mem, int flags)
{
    char sz[3] = {0};

    if (flags & BYTES_FL)
    {
        snprintf(sz, sizeof(sz), "B");
    }
    else if (flags & KILOBYTES_FL)
    {
        snprintf(sz, sizeof(sz), "Kb");
    }
    else if (flags & MEGABYTES_FL)
    {
        snprintf(sz, sizeof(sz), "Mb");
    }
    else if (flags & GIGABYTES_FL)
    {
        snprintf(sz, sizeof(sz), "Gb");
    }

    snprintf(buf, BUFDEC, "%s [%s]:", mem, sz);

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
