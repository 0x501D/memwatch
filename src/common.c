#include <memwatch.h>

char *num_to_str(char* buf, size_t len, uint64_t num, int flags)
{
    if (flags & MEGABYTES_FL)
    {
        num /= 1024;
    }
    else if (flags & GIGABYTES_FL)
    {
        num /= 1024 * 1024;
    }
    else if (flags & BYTES_FL)
    {
        num *= 1024;
    }

    snprintf(buf, len, "%lu", num);

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

    snprintf(buf, BUFSIZ, "%s [%s]:", mem, sz);

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

void print_bar(uint32_t col, uint32_t used, uint32_t last)
{
    size_t i, row;
    row = 2;

    mvaddch(col, row - 1, '[');
    for (i = 0; i < used; i++, row++)
    {
         mvaddch(col, row, '#');
    }
    for (i = 0; i < last; i++, row++)
    {
         mvaddch(col, row, '-');
    }
    mvaddch(col, row, ']');
}
