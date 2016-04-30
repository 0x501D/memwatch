#include <memwatch.h>
#include <common.h>
#include <draw.h>
#include <mem_fields.h>

static void print_info(const options_t *options);

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
    mem_field_t *mem_bufp, *mem_availp, *swap_totalp, *swap_freep;
    char buf[BUFSIZ] = {0};
    float mem_ratio, mem_bar_used, swap_ratio, swap_bar_used, swap_bar_free;
    uint64_t mem_used, mem_free, mem_cached, swap_used;
    uint32_t mem_bar_free;
    size_t i;
    uint16_t col;

    mem_used = mem_free = mem_cached = swap_used = 0;
    mem_freep = mem_totalp = mem_cachp = mem_slabp = NULL;
    mem_availp = mem_bufp = swap_totalp = swap_freep = NULL;

    mem_freep    = get_mem_field(KEYMEMFREE,  sizeof(KEYMEMFREE) - 1);
    mem_totalp   = get_mem_field(KEYMEMTOTAL, sizeof(KEYMEMTOTAL) - 1);
    mem_cachp    = get_mem_field(KEYCACHED,   sizeof(KEYCACHED) - 1);
    mem_slabp    = get_mem_field(KEYSLAB,     sizeof(KEYSLAB) - 1);
    mem_bufp     = get_mem_field(KEYBUFF,     sizeof(KEYBUFF) - 1);
    mem_availp   = get_mem_field(KEYMEMAVAIL, sizeof(KEYMEMAVAIL) - 1);
    swap_totalp  = get_mem_field(KEYSWTOTAL,  sizeof(KEYSWTOTAL) - 1);

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
    mvaddstr(col++, 10, num_to_str(buf, sizeof(buf), mem_totalp->value, options->flags));
    mvaddstr(col, 1, _("Free"));
    mvaddstr(col++, 10, num_to_str(buf, sizeof(buf), mem_freep->value, options->flags));
    mvaddstr(col, 1, _("Used"));
    mvaddstr(col++, 10, num_to_str(buf, sizeof(buf), mem_used, options->flags));
    mvaddstr(col, 1, _("Buff"));
    mvaddstr(col++, 10, num_to_str(buf, sizeof(buf), mem_bufp->value, options->flags));
    mvaddstr(col, 1, _("Cache"));
    mvaddstr(col++, 10, num_to_str(buf, sizeof(buf), mem_cached, options->flags));
    mvaddstr(col, 1, _("Avail"));
    mvaddstr(col++, 10, num_to_str(buf, sizeof(buf), mem_availp->value, options->flags));

    if (swap_totalp && !swap_totalp->value)
    {
         col++; col++;
         mvaddstr(col, 1, _("Swap disabled."));
    }
    else
    {
        swap_freep = get_mem_field(KEYSWFREE, sizeof(KEYSWFREE) - 1);
        if (swap_totalp && swap_freep)
        {
            swap_used = swap_totalp->value - swap_freep->value;
        }

        swap_ratio = (float) swap_used / swap_totalp->value;
        swap_bar_used = BAR_LEHGTH * swap_ratio;
        swap_bar_free = BAR_LEHGTH - floor(swap_bar_used);

        col++; col++;
        mvaddstr(col, 1, gen_title(buf, _("Swap"), options->flags));
        col++; col++;
        print_bar(col, swap_bar_used, swap_bar_free);
        col++; col++;
        mvaddstr(col, 1, _("Total"));
        mvaddstr(col++, 10, num_to_str(buf, sizeof(buf), swap_totalp->value, options->flags));
        mvaddstr(col, 1, _("Free"));
        mvaddstr(col++, 10, num_to_str(buf, sizeof(buf), swap_freep->value, options->flags));
        mvaddstr(col, 1, _("Used"));
        mvaddstr(col, 10, num_to_str(buf, sizeof(buf), swap_used, options->flags));
    }

    refresh();
}
