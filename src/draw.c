#include <memwatch.h>
#include <common.h>
#include <draw.h>

static void print_bar(uint32_t col, uint32_t used, uint32_t last);
static void print_info(const mem_t *mem);
static void insert_value(mem_t *mem, char *line, int ch);

void get_data(const options_t *options)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    mem_t memory;

    memset(&memory, 0, sizeof(memory));
    memory.swap_disabled = 0;
    memory.options = options;

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
         if (strncmp(KEYMEMFREE, line, strlen(KEYMEMFREE)) == 0)
         {
              insert_value(&memory, line, FREE_MEM);
         }
         else if (strncmp(KEYMEMTOTAL, line, strlen(KEYMEMTOTAL)) == 0)
         {
              insert_value(&memory, line, TOTAL_MEM);
         }
         else if (strncmp(KEYBUFF, line, strlen(KEYBUFF)) == 0)
         {
              insert_value(&memory, line, BUFF_MEM);
         }
         else if (strncmp(KEYCACHED, line, strlen(KEYCACHED)) == 0)
         {
              insert_value(&memory, line, CACHE_MEM);
         }
         else if (strncmp(KEYSWFREE, line, strlen(KEYSWFREE)) == 0)
         {
              insert_value(&memory, line, FREE_SWAP);
         }
         else if (strncmp(KEYSWTOTAL, line, strlen(KEYSWTOTAL)) == 0)
         {
              insert_value(&memory, line, TOTAL_SWAP);
         }
    }

    free(line);
    fclose(fp);

    print_info(&memory);
}

static void print_info(const mem_t *mem)
{
    char buf[BUFDEC] = {0};
    float mem_ratio = (float) mem->mem_used / mem->mem_total;
    float mem_bar_used = BAR_LEHGTH * mem_ratio;
    uint32_t mem_bar_free = BAR_LEHGTH - floor(mem_bar_used);
    float swap_ratio = (float) mem->swap_used / mem->swap_total;
    float swap_bar_used = BAR_LEHGTH * swap_ratio;
    uint32_t swap_bar_free = BAR_LEHGTH - floor(swap_bar_used);
    size_t i;
    uint16_t col = 1;

    for (i = 1; i <= 20; i++)
    {
         mvaddstr(i, 1, ALOTOFSPACES);
    }

    mvaddstr(col, 1, gen_title(buf, _("Memory"), mem->options->flags));
    col++; col++;
    print_bar(col, mem_bar_used, mem_bar_free);
    col++; col++;
    mvaddstr(col, 1, _("Total"));
    mvaddstr(col++, 10, num_to_str(buf, mem->mem_total));
    mvaddstr(col, 1, _("Free"));
    mvaddstr(col++, 10, num_to_str(buf, mem->mem_free));
    mvaddstr(col, 1, _("Used"));
    mvaddstr(col++, 10, num_to_str(buf, mem->mem_used));
    mvaddstr(col, 1, _("Buff"));
    mvaddstr(col++, 10, num_to_str(buf, mem->mem_buff));
    mvaddstr(col, 1, _("Cache"));
    mvaddstr(col++, 10, num_to_str(buf, mem->mem_cache));

    if (mem->swap_disabled)
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
    }

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
