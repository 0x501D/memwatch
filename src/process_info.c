#include <ctype.h>
#include <dirent.h>
#include <pwd.h>
#include <libgen.h>

#include <memwatch.h>
#include <memory_info.h>
#include <process_info.h>
#include <common.h>
#include <vector.h>

static void print_items(uint32_t pos, list_navi_t *navi,
                        const vector_process_t *v,
                        const options_t *options);
static void print_item(const process_data_t *ps, const options_t *options);
static size_t get_process_count(void);
static void get_process_list(vector_process_t *v, uint8_t full);
static int get_process_stats(const char *path, process_data_t *item, uint8_t full);
static uint64_t legacy_get_shmem(const char *path);
static void get_process_cmdline(char *cmdline, pid_t pid, size_t str_len);

void print_process_list(const options_t *options, list_navi_t *navi,
                        vector_process_t *v)
{
    char total[BUFSIZ] = {0};
    char buf[BUFSIZ] = {0};

    clear_screen();

    if (!(options->flags & REPRINT_FL))
    {
        get_process_list(v, 0);
    }

    snprintf(total, sizeof(total), "[%s %d]", _("total:"), (int) v->size);

    mvaddstr(1, 1, gen_title(buf, _("Process List"), options->flags));
    mvaddstr(1, COLS - (strlen(total) + 1), total);
    attron(A_REVERSE);
    mvprintw(3, 0, " %s%8s%10s%12s%13s%12s%10s %s", _("PID"), _("OWNER"),
             _("RES"), _("SHR"), _("VIRT"), _("SWAP"), _("S"), _("Command"));
    mvhline(3, 77, ' ', COLS);
    attroff(A_REVERSE);

    vector_sort(v, options->flags);
    print_items(4, navi, v, options);

    refresh();
}

void print_single_process(options_t *options, list_navi_t *navi, 
                          vector_process_t *v, process_data_t *ps)
{
    clear_screen();

    /* We need to get pid for selected process */
    if (!navi->fixed_ps)
    {
        size_t index = (navi->highlight + navi->offset) - 1;
        const process_data_t *ps = NULL;
        get_process_list(v, 1);
        vector_sort(v, options->flags);
        ps = vector_at(v, index);
        if (!ps)
        {
            goto out;
        }
        print_item(ps, options);
        navi->fixed_ps = (vector_at(v, index))->pid;
        return;
    }

    /* We have pid, no need to use vector */
    {
        char path[PATH_MAX + 1] = {0};

        snprintf(path, sizeof(path), "%s/%d/status", PROCDIR, navi->fixed_ps);
        if (!(options->flags & REPRINT_FL))
        {
            if (get_process_stats(path, ps, 1) == 0)
            {
                print_item(ps, options);
                return;
            }
            else
            {
                goto out;
            }
        }

        print_item(ps, options);
        return;
    }

out:
    options->flags &= ~SINGLE_PS_FL;
    options->flags |= PROC_LIST_FL;
    navi->fixed_ps = 0;
    navi->flags |= NAVI_FIXED_PS_EXITED;
}

static void print_item(const process_data_t *ps, const options_t *options)
{
    char buf[BUFSIZ] = {0};
    char cmdline[MAX_CMDLINE + 1] = {0};
    char num_buf_a[MAX_UINT64_LEN+1] = {0};
    char num_buf_b[MAX_UINT64_LEN+1] = {0};
    get_process_cmdline(cmdline, ps->pid, 2);

    mvaddstr(1, 1, gen_title(buf, _("Process Information"), options->flags));
    mvaddstr(3, 1, cmdline);
    mvprintw(5, 1, "%s: %s, %s %d", _("State"), ps->state,
                                    _("Pid"), ps->pid);
    num_to_str(num_buf_a, sizeof(num_buf_a), ps->vm_size , options),
    num_to_str(num_buf_b, sizeof(num_buf_b), ps->vm_peak , options),
    mvprintw(7, 1, "%-9s %s, %s: %s", _("Virtual:"), num_buf_a,
                                     _("Peak"), num_buf_b);
    num_to_str(num_buf_a, sizeof(num_buf_a), ps->vm_rss , options),
    num_to_str(num_buf_b, sizeof(num_buf_b), ps->vm_hwv , options),
    mvprintw(8, 1, "%-9s %s, %s: %s", _("Resident:"), num_buf_a,
                                     _("Peak"), num_buf_b);
    mvprintw(9, 1, "%-9s %s", _("Shared:"),
        num_to_str(num_buf_a, sizeof(num_buf_a), ps->vm_shr, options));
    mvprintw(10, 1, "%-9s %s", _("Swap:"),
        num_to_str(num_buf_a, sizeof(num_buf_a), ps->vm_swap, options));
    mvprintw(11, 1, "%-9s %s", _("Data:"),
        num_to_str(num_buf_a, sizeof(num_buf_a), ps->vm_data, options));
    mvprintw(12, 1, "%-9s %s", _("Stack:"),
        num_to_str(num_buf_a, sizeof(num_buf_a), ps->vm_stk, options));
    mvprintw(13, 1, "%-9s %s", _("Text:"),
        num_to_str(num_buf_a, sizeof(num_buf_a), ps->vm_exe, options));
    mvprintw(14, 1, "%-9s %s", _("Library:"),
        num_to_str(num_buf_a, sizeof(num_buf_a), ps->vm_lib, options));
}

static void print_items(uint32_t pos, list_navi_t *navi,
                        const vector_process_t *v,
                        const options_t *options)
{
    uint32_t count;
    uint32_t visible_items = LINES - 4;
    uint32_t index = 0;
    struct passwd *user_info = NULL;
    char rss[MAX_UINT64_LEN +1 ] = {0};
    char shr[MAX_UINT64_LEN +1 ] = {0};
    char vir[MAX_UINT64_LEN +1 ] = {0};
    char swp[MAX_UINT64_LEN +1 ] = {0};
    char cmdline[MAX_CMDLINE + 1] = {0};

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
        /* TODO: this is dirty trick, try to fix it in future. */
        if (navi->offset > 0)
        {
            navi->highlight = 1;
            navi->offset = 0;
        }
    }

    for (count = 1; index < v->size; index++, count++, pos++)
    {
        user_info = getpwuid(vector_at(v, index)->uid);

        if (count == navi->highlight)
        {
            attron(A_REVERSE | COLOR_PAIR(1));
        }

        get_process_cmdline(cmdline, vector_at(v, index)->pid, 70);

        mvprintw(pos, 0, "%5d  %-11s %-12s%-12s%-12s%-12s %c %s",
                 vector_at(v, index)->pid,
                 user_info ? user_info->pw_name : _("unknown"),
                 num_to_str(rss, sizeof(rss), vector_at(v, index)->vm_rss, options),
                 num_to_str(shr, sizeof(shr), vector_at(v, index)->vm_shr, options),
                 num_to_str(vir, sizeof(vir), vector_at(v, index)->vm_size, options),
                 num_to_str(swp, sizeof(swp), vector_at(v, index)->vm_swap, options),
                 vector_at(v, index)->state[0],
                 cmdline);

        if (count == navi->highlight)
        {
            attroff(A_REVERSE | COLOR_PAIR(1));
        }
    }
}

static size_t get_process_count(void)
{
    DIR *proc_dirp;
    size_t proc_count = 0;
    struct dirent *dir_info = NULL;

    if ((proc_dirp = opendir(PROCDIR)) == NULL)
    {
        err_exit("%s %s : %s", _("Error opening"),
                 PROCDIR, strerror(errno));
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

static void get_process_list(vector_process_t *v, uint8_t full)
{
    DIR *proc_dirp;
    struct dirent *dir_info = NULL;
    char status_file[PATH_MAX] = {0};
    process_data_t item;

    if (v->total == 0) /* vector was not initialized */
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
        err_exit("%s %s : %s", _("Error opening"),
                 PROCDIR, strerror(errno));
    }

    while ((dir_info = readdir(proc_dirp)) != NULL)
    {
        memset(&item, 0, sizeof(process_data_t));

        if (!dirname_only_digits(dir_info->d_name))
        {
            continue;
        }

        snprintf(status_file, sizeof(status_file), "%s/%s/%s",
                 PROCDIR, dir_info->d_name, STATFILE);

        if (get_process_stats(status_file, &item, full) == 0)
        {
            vector_insert(v, &item);
        }
    }

    closedir(proc_dirp);
}

static int get_process_stats(const char *path, process_data_t *item, uint8_t full)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char num_value[MAX_UINT64_LEN + 1] = {0};
    int user_process = 0, new_shmem = 0;

    if ((fp = fopen(path, "r")) == NULL)
    {
        /* process dissapear, ignore it */
        return 1;
    }

    /* TODO: optimize this madness */
    while ((read = getline(&line, &len, fp)) != -1)
    {
        char *saveptr = NULL;
        char *field, *value;
        size_t last_ch;

        field = value = NULL;
        field = strtok_r(line, ":", &saveptr);
        value = strtok_r(NULL, "\t", &saveptr);

        last_ch = strlen(value) - 1;
        if (value[last_ch] == '\n')
        {
            value[last_ch] = '\0';
        }

        if (strncmp(field, STATUS_PID, sizeof(STATUS_PID)) == 0)
        {
            item->pid = strtoul(value, NULL, 10);
        }
        else if (strncmp(field, STATUS_UID, sizeof(STATUS_UID)) == 0)
        {
            item->uid = strtoul(value, NULL, 10);
        }
        else if (strncmp(field, STATUS_STATE, sizeof(STATUS_STATE)) == 0)
        {
            strncpy(item->state, value, MAX_STATUS_LEN);
        }
        else if (strncmp(field, STATUS_RSS, sizeof(STATUS_RSS)) == 0)
        {
            grep_digits(num_value, value, sizeof(num_value));
            item->vm_rss = strtoul(num_value, NULL, 10);
            user_process = 1;
        }
        else if (strncmp(field, STATUS_SHR, sizeof(STATUS_SHR)) == 0)
        {
            grep_digits(num_value, value, sizeof(num_value));
            item->vm_shr = strtoul(num_value, NULL, 10);

            /* keyword "RssFile" appeared in kernel >= 4.5
             * for kernels < 4.5 we need to get value from /proc/PID/statm */
            new_shmem = 1;
        }
        else if (strncmp(field, STATUS_VIRT, sizeof(STATUS_VIRT)) == 0)
        {
            grep_digits(num_value, value, sizeof(num_value));
            item->vm_size = strtoul(num_value, NULL, 10);
        }
        else if (strncmp(field, STATUS_SWAP, sizeof(STATUS_SWAP)) == 0)
        {
            grep_digits(num_value, value, sizeof(num_value));
            item->vm_swap = strtoul(num_value, NULL, 10);
        }
        if (full)
        {
            if (strncmp(field, STATUS_VPEAK, sizeof(STATUS_VPEAK)) == 0)
            {
                grep_digits(num_value, value, sizeof(num_value));
                item->vm_peak = strtoul(num_value, NULL, 10);
            }
            else if (strncmp(field, STATUS_RPEAK, sizeof(STATUS_RPEAK)) == 0)
            {
                grep_digits(num_value, value, sizeof(num_value));
                item->vm_hwv = strtoul(num_value, NULL, 10);
            }
            else if (strncmp(field, STATUS_DATA, sizeof(STATUS_DATA)) == 0)
            {
                grep_digits(num_value, value, sizeof(num_value));
                item->vm_data = strtoul(num_value, NULL, 10);
            }
            else if (strncmp(field, STATUS_STK, sizeof(STATUS_STK)) == 0)
            {
                grep_digits(num_value, value, sizeof(num_value));
                item->vm_stk = strtoul(num_value, NULL, 10);
            }
            else if (strncmp(field, STATUS_EXE, sizeof(STATUS_EXE)) == 0)
            {
                grep_digits(num_value, value, sizeof(num_value));
                item->vm_exe = strtoul(num_value, NULL, 10);
            }
            else if (strncmp(field, STATUS_LIB, sizeof(STATUS_LIB)) == 0)
            {
                grep_digits(num_value, value, sizeof(num_value));
                item->vm_lib = strtoul(num_value, NULL, 10);
            }
        }
    }

    free(line);
    fclose(fp);

    if (!user_process) /* ignore kernel process */
    {
        return 1;
    }

    if (!new_shmem)
    {
        char status_path[PATH_MAX] = {0};
        char statm_path[PATH_MAX] = {0};
        strncpy(status_path, path, sizeof(status_path));
        snprintf(statm_path, sizeof(statm_path), "%s/statm", dirname(status_path));
        item->vm_shr = legacy_get_shmem(statm_path);
    }

    return 0;
}

uint64_t legacy_get_shmem(const char *path)
{
    uint64_t res = 0;
    char buf[256] = {0};
    FILE *fp;

    if ((fp = fopen(path, "r")) == NULL)
    {
        return res;
    }

    if (fgets(buf, sizeof(buf), fp) == NULL)
    {
        fclose(fp);
        return res;
    }

    fclose(fp);

    {
        uint64_t page_size;
        char *saveptr, *value;
        int i;

        page_size = sysconf(_SC_PAGESIZE);
        value = saveptr = NULL;

        value = strtok_r(buf, " ", &saveptr);
        for (i = 0; i < 2; ++i)
        {
            value = strtok_r(NULL, " ", &saveptr);
            if (value == NULL) return res;
        }
        res = strtoul(value, NULL, 10);
        res = res * (page_size / DEFAULT_POWER);
    }

    return res;
}

static void get_process_cmdline(char *cmdline, pid_t pid, size_t str_len)
{
    char path[PATH_MAX] = {0};
    char buf[MAX_CMDLINE+1] = {0};
    FILE *fp;

    snprintf(path, sizeof(path), "%s/%d/cmdline", PROCDIR, pid);

    if ((fp = fopen(path, "r")) == NULL)
    {
        strncpy(cmdline, _("unknown"), MAX_CMDLINE);
        return;
    }

    if (fgets(buf, sizeof(buf) - 1, fp) == NULL)
    {
        strncpy(cmdline, _("unknown"), MAX_CMDLINE);
        fclose(fp);
        return;
    }

    {
        uint32_t i;
        uint32_t cmd_len = COLS - str_len;
        for (i = 0; (i < cmd_len && i != MAX_CMDLINE); i++)
        {
            if (buf[i] == '\0')
            {
                buf[i] = ' ';
            }
            cmdline[i] = buf[i];
        }
        cmdline[i] = '\0';
    }

    fclose(fp);
}
