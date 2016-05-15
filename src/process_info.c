#include <ctype.h>
#include <dirent.h>

#include <memwatch.h>
#include <memory_info.h>
#include <common.h>
#include <vector.h>

static void print_items(uint32_t pos, list_navi_t *navi, const vector_process_t *v);
static int dirname_only_digits(const char *name);
static size_t get_process_count(void);
static void get_process_list(vector_process_t *v);

void print_process_list(const options_t *options, list_navi_t *navi,
                        vector_process_t *v)
{
    int i;
    char total[BUFSIZ] = {0};

    clear_screen();

    if (!(options->flags & REPRINT_FL))
    {
        get_process_list(v);
    }

    snprintf(total, sizeof(total), "%s%d]", _("[total: "), (int) v->size);

    mvaddstr(1, 1, _("Process List:"));
    mvaddstr(1, COLS - (strlen(total) + 1), total);
    attron(A_REVERSE);
    mvprintw(3, 0, " %s%10s%10s%10s%15s", _("PID"), _("OWNER"),
             _("RSS"), _("SWAP"), _("Command"));
    for(i = 49; i < COLS; ++i)
    {
        mvaddch(3, i, ' ');
    }
    attroff(A_REVERSE);

    print_items(4, navi, v);

    refresh();
}

static void print_items(uint32_t pos, list_navi_t *navi, const vector_process_t *v)
{
    uint32_t num;
    uint32_t visible_items = LINES - 4;
    uint32_t index = 0;

    /* up key pressed at the top of list, goto last element */
    if (navi->flags & NAVI_GO_LAST_FL)
    {
        /* items count greater lines count, shift list */
        if (v->size > visible_items)
        {
            navi->offset = v->size - visible_items;
            navi->highlight = visible_items;
        }
        else
        {
            navi->highlight = v->size;
        }
        navi->flags &= ~NAVI_GO_LAST_FL;
    }

    if (v->size > visible_items)
    {
        if (navi->offset <= v->size - visible_items)
        {
            index = navi->offset;
        }
        else
        {
            navi->highlight = 1;
            navi->offset = 0;
        }
    }
    else
    {
        if (navi->highlight > v->size)
        {
            navi->highlight = 1;
        }
        /* this is dirty trick, try to fix it in future. */
        if (navi->offset > 0)
        {
            navi->highlight = 1;
            navi->offset = 0;
        }
    }

    for (num = 1; index < v->size; index++, num++, pos++)
    {
        if (num == navi->highlight)
        {
            attron(A_REVERSE | COLOR_PAIR(1));
            mvprintw(pos, 0, " %s", vector_at(v, index)->name);
            attroff(A_REVERSE | COLOR_PAIR(1));
            continue;
        }

        mvprintw(pos, 0, " %s", vector_at(v, index)->name);
    }
}

static int dirname_only_digits(const char *name)
{
    int res = 1;

    for (; *name != '\0'; name += sizeof(char))
    {
        if (!isdigit(*name))
        {
            res = 0;
            break;
        }
    }

    return res;
}

static size_t get_process_count(void)
{
    DIR *proc_dirp;
    size_t proc_count = 0;
    struct dirent *dir_info = NULL;

    if ((proc_dirp = opendir(PROCDIR)) == NULL)
    {
        fprintf(stderr, "Error opening %s : %s\n", 
                PROCDIR, strerror(errno));
        exit(1);
    }

    while ((dir_info = readdir(proc_dirp)) != NULL)
    {
        if (!dirname_only_digits(dir_info->d_name))
        {
            continue;
        }

        proc_count++;
    }

    closedir(proc_dirp);

    return proc_count;
}

static void get_process_list(vector_process_t *v)
{
    DIR *proc_dirp;
    size_t proc_count = 0;
    struct dirent *dir_info = NULL;
    process_data_t item;

    /* vector was not initialized */
    if (v->total == 0)
    {
        size_t nmemb = get_process_count();
        vector_init(v, nmemb * 2);
    }
    else /* clear old vector data before use */
    {
        vector_clear(v);
    }

    if ((proc_dirp = opendir(PROCDIR)) == NULL)
    {
        fprintf(stderr, "Error opening %s : %s\n", 
                PROCDIR, strerror(errno));
        exit(1);
    }

    while ((dir_info = readdir(proc_dirp)) != NULL)
    {
        memset(&item, 0, sizeof(process_data_t));

        if (!dirname_only_digits(dir_info->d_name))
        {
            continue;
        }
        strncpy(item.name, dir_info->d_name, MAX_NAME_LEN);
        vector_insert(v, &item);

        proc_count++;
    }

    closedir(proc_dirp);
}
