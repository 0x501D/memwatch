#include <memwatch.h>

char *num_to_str(char* buf, size_t len, uint64_t num, const options_t *opt)
{
    float fract_num = num;

    if (opt->flags & MEGABYTES_FL)
    {
        num /= opt->power;
    }
    else if (opt->flags & GIGABYTES_FL)
    {
        fract_num /= pow(opt->power, 2);
    }
    else if (opt->flags & TERABYTES_FL)
    {
        fract_num /= pow(opt->power, 3);
    }
    else if (opt->flags & BYTES_FL)
    {
        /* In bytes we cannot use power of 1000 */
        num *= DEFAULT_POWER;
    }
    else if ((opt->flags & KILOBYTES_FL) &&
             (opt->flags & SI_UNITS_FL))
    {
        num = fract_num / SI_POWER_DIV;
    }

    if ((opt->flags & GIGABYTES_FL) ||
        (opt->flags & TERABYTES_FL))
    {
        if (!fract_num)
        {
            snprintf(buf, len, "%.0f", fract_num);
        }
        else
        {
            snprintf(buf, len, "%.2f", fract_num);
        }
    }
    else if (opt->flags & HUMAN_RD_FL)
    {
        if (num < opt->power)
        {
            snprintf(buf, len, "%lu Kb", num);
        }
        else if (num < pow(opt->power, 2))
        {
            snprintf(buf, len, "%lu Mb", num / opt->power);
        }
        else if (num < pow(opt->power, 3))
        {
            fract_num /= pow(opt->power, 2);
            snprintf(buf, len, "%.2f Gb", fract_num);
        }
        else if (num < pow(opt->power, 4))
        {
            fract_num /= pow(opt->power, 3);
            snprintf(buf, len, "%.2f Tb", fract_num);
        }
        else
        {
            snprintf(buf, len, "test");
        }
    }
    else
    {
        snprintf(buf, len, "%lu", num);
    }

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
    else if (flags & TERABYTES_FL)
    {
        snprintf(sz, sizeof(sz), "Tb");
    }
    else if (flags & HUMAN_RD_FL)
    {
        snprintf(sz, sizeof(sz), "H");
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
