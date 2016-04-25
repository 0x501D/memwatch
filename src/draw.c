#include <memwatch.h>
#include <common.h>
#include <draw.h>
#include <mem_fields.h>

static void print_bar(uint32_t col, uint32_t used, uint32_t last);
static void print_info(const options_t *options);
/*static void insert_value(mem_t *mem, char *line, int ch);*/

void get_data(const options_t *options)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if ((fp = fopen(MEMFILE, "r")) == NULL)
    {
         nocbreak();
         mvaddstr(1, 1, _("Error opening file:"));
         mvaddstr(2, 1, MEMFILE);
         getch();
         endwin();
         exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char *saveptr = NULL;
        char *field, *value;
        size_t last_ch;
        mem_field_t *fieldp = NULL;

        field = value = NULL;
        field = strtok_r(line, ":", &saveptr);
        fieldp = get_mem_field(field, strlen(field));

        if (!fieldp)
        {
            continue;
        }

        value = strtok_r(NULL, " ", &saveptr);
        last_ch = strlen(value) - 1;
        if (value[last_ch] == '\n')
        {
            value[last_ch] = '\0';
        }

        fieldp->value = strtoul(value, NULL, 10);
    }

    if (line)
    {
        free(line);
    }
    fclose(fp);

    print_info(options);
}

static void print_info(const options_t *options)
{
    mem_field_t *mem_freep, *mem_totalp, *mem_cachp, *mem_slabp;
    mem_field_t *mem_bufp;
    char buf[BUFSIZ] = {0};
    float mem_ratio, mem_bar_used, swap_ratio, swap_bar_used, swap_bar_free;
    uint64_t mem_used, mem_free, mem_cached;
    uint32_t mem_bar_free;
    size_t i;
    uint16_t col;

    mem_used = mem_free = mem_cached = 0;
    mem_freep = mem_totalp = mem_cachp = mem_slabp = NULL;
    mem_bufp = NULL;

    mem_freep  = get_mem_field(KEYMEMFREE,  sizeof(KEYMEMFREE) - 1);
    mem_totalp = get_mem_field(KEYMEMTOTAL, sizeof(KEYMEMTOTAL) - 1);
    mem_cachp  = get_mem_field(KEYCACHED,   sizeof(KEYCACHED) - 1);
    mem_slabp  = get_mem_field(KEYSLAB,     sizeof(KEYSLAB) - 1);
    mem_bufp   = get_mem_field(KEYBUFF,     sizeof(KEYBUFF) - 1);

    if (mem_cachp && mem_slabp)
    {
        mem_cached = mem_cachp->value + mem_slabp->value;
    }

    if (mem_freep && mem_totalp && mem_bufp)
    {
        mem_used = mem_totalp->value - mem_freep->value -
                   mem_cached - mem_bufp->value;
    }

    mem_ratio = (float) mem_used / mem_totalp->value;
    mem_bar_used = BAR_LEHGTH * mem_ratio;
    mem_bar_free = BAR_LEHGTH - floor(mem_bar_used);
    /*swap_ratio = (float) mem->swap_used / mem->swap_total;
    swap_bar_used = BAR_LEHGTH * swap_ratio;
    swap_bar_free = BAR_LEHGTH - floor(swap_bar_used);*/
    col = 1;

    for (i = 1; i <= 20; i++)
    {
         mvaddstr(i, 1, ALOTOFSPACES);
    }

    mvaddstr(col, 1, gen_title(buf, _("Memory"), options->flags));
    col++; col++;
    print_bar(col, mem_bar_used, mem_bar_free);
    col++; col++;
    mvaddstr(col, 1, _("Total"));
    mvaddstr(col++, 10, num_to_str(buf, mem_totalp->value, options->flags));
    mvaddstr(col, 1, _("Free"));
    mvaddstr(col++, 10, num_to_str(buf, mem_freep->value, options->flags));
    mvaddstr(col, 1, _("Used"));
    mvaddstr(col++, 10, num_to_str(buf, mem_used, options->flags));
    mvaddstr(col, 1, _("Buff"));
    mvaddstr(col++, 10, num_to_str(buf, mem_bufp->value, options->flags));
    mvaddstr(col, 1, _("Cache"));
    mvaddstr(col++, 10, num_to_str(buf, mem_cached, options->flags));

    /*if (mem->swap_disabled)
    {
         col++; col++;
         mvaddstr(col, 1, _("Swap disabled."));
    }
    else
    {
         col++; col++;
         mvaddstr(col, 1, gen_title(buf, _("Swap"), mem->options->flags));
         col++; col++;
         print_bar(col, swap_bar_used, swap_bar_free);
         col++; col++;
         mvaddstr(col, 1, _("Total"));
         mvaddstr(col++, 10, num_to_str(buf, mem->swap_total));
         mvaddstr(col, 1, _("Free"));
         mvaddstr(col++, 10, num_to_str(buf, mem->swap_free));
         mvaddstr(col, 1, _("Used"));
         mvaddstr(col, 10, num_to_str(buf, mem->swap_used));
    }*/

    refresh();
}

static void print_bar(uint32_t col, uint32_t used, uint32_t last)
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
#if 0
static void insert_value(mem_t *mem, char *line, int ch)
{
    char *token;
    char *end = NULL;
    char buf[BUFDEC];
    uint64_t num;
    errno = 0;

    token = strtok(line, " ");
    token = strtok(NULL, " ");
    num = strtoull(token, &end, 10);
    if ((token + strlen(token)) != end || errno == ERANGE)
    {
        num = 0;
    }
    else
    {
        if (mem->options->flags & MEGABYTES_FL)
        {
            num /= 1024;
        }
        else if (mem->options->flags & GIGABYTES_FL)
        {
            num /= 1024 * 1024;
        }
        else if (mem->options->flags & BYTES_FL)
        {
            num *= 1024;
        }
    }
    snprintf(buf, sizeof(buf), "%lu", num);

    switch (ch)
    {
        case FREE_MEM:
            mem->mem_free = num;
            mem->mem_used = mem->mem_total - mem->mem_free;
            break;
        case FREE_SWAP:
            if (!mem->swap_disabled)
            {
                mem->swap_free = num;
                mem->swap_used = mem->swap_total - mem->swap_free;
            }
            break;
        case TOTAL_MEM:
            mem->mem_total = num;
            break;
        case BUFF_MEM:
            mem->mem_buff = num;
            break;
        case CACHE_MEM:
            mem->mem_cache = num;
            break;
        case TOTAL_SWAP:
            if (!num)
            {
                mem->swap_disabled = 1;
            }
            else
            {
                mem->swap_total = num;
            }
            break;
    }
}
#endif
