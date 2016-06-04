#include <ctype.h>

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
    start_color();
    use_default_colors();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    keypad(stdscr, TRUE);
    noecho();
    timeout(0);
    curs_set(0);
    cbreak();
    clear();
}

void clear_screen(void)
{
    int i;

    for (i = 0; i <= LINES; i++)
    {
        mvhline(i, 0, ' ', COLS);
    }
}

static void clear_window(WINDOW *w)
{
    int i;

    for (i = 0; i <= LINES; i++)
    {
        mvwhline(w, i, 0, ' ', COLS);
    }
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

void err_exit(const char *fmt, ...)
{
    va_list args;
    WINDOW *win;

    va_start(args, fmt);

    win = newwin(LINES, COLS, 1, 1);
    vwprintw(win, fmt, args);
    wrefresh(win);

    wgetch(win);

    va_end(args);
    delwin(win);
    endwin();
    exit(EXIT_FAILURE);
}

void grep_digits(char *dst, const char *src, size_t len)
{
    size_t i;

    for (i = 0; (i < len && *src != '\0'); i++, src++)
    {
        if (isdigit(*src))
        {
            *(dst++) = *src;
        }
    }

    *dst = '\0';
}

int dirname_only_digits(const char *name)
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

static void do_hidden_game(void)
{
    WINDOW *win;
    int ch, quit = 0, up = 0;
    int power = 0;
    int x_pos = (COLS / 2) - 30;
    int y_pos = LINES / 2;
    const char *tux0 = "(o_";
    const char *tux1 = "//\\";
    const char *tux2 = "V_/_";

    win = newwin(LINES, COLS, 0, 0);
    nodelay(win, TRUE);

    while (!quit)
    {
        clear_window(win);

        if (y_pos == LINES)
        {
            break;
        }

        if (up)
        {
            power = 6;
            up = 0;
        }

        mvwaddstr(win, y_pos-1, x_pos, tux0);
        mvwaddstr(win, y_pos, x_pos,   tux1);
        mvwaddstr(win, y_pos+1, x_pos, tux2);
        wrefresh(win);

        switch (power)
        {
            case 6:
                y_pos -= 3;
                power -= 3;
                break;
            case 3:
                y_pos -= 2;
                power -= 2;
                break;
            case 1:
                y_pos -= 1;
                power -= 1;
                break;
            default:
                y_pos++;
                break;
        }

        ch = wgetch(win);

        switch (ch)
        {
            case ' ':
                up = 1;
                break;
            case ERR:
                break;
            default:
                quit = 1;
                break;
        }

        usleep(80000);
    }

    delwin(win);
}

void print_hotkeys_help(void)
{
    WINDOW *win;
    char ch;

    win = newwin(LINES, COLS, 0, 0);
    mvwaddstr(win, 1, 1, _("Global hotkeys:"));
    mvwprintw(win, 3, 1, "%-11s %s", _("b,k,m,g,t:"),
              _("unit size: bytes, kilobytes, megabytes, gigabytes and terabytes."));
    mvwprintw(win, 4, 1, "%-11s %s", _("h:"),
              _("display the amount of memory in human readable format."));
    mvwprintw(win, 5, 1, "%-11s %s", _("s:"),
              _("print sizes in powers of 1000 instead of 1024."));
    mvwprintw(win, 6, 1, "%-11s %s", _("l:"),
              _("switch between the main window and a list of processes."));
    mvwprintw(win, 7, 1, "%-11s %s", _("F1,?:"),
              _("print this help."));
    mvwprintw(win, 8, 1, "%-11s %s", _("q:"),
              _("exit memwatch."));
    mvwaddstr(win, 10, 1, _("Process list hotkeys:"));
    mvwprintw(win, 12, 1, "%-11s %s", _("Arrows:"),
              _("scroll process list."));
    mvwprintw(win, 13, 1, "%-11s %s", _("Home:"),
              _("go to the begining of the process list."));
    mvwprintw(win, 14, 1, "%-11s %s", _("End:"),
              _("go to the end of the process list."));
    mvwprintw(win, 15, 1, "%-11s %s", _("1,2,3,4:"),
              _("sort by field: 1 -> RSS, 2 -> SHR, 3 -> VIRT, 4 -> SWAP"));
    mvwprintw(win, 16, 1, "%-11s %s", _("r:"),
              _("switch the sort order."));
    mvwprintw(win, 17, 1, "%-11s %s", _("Enter:"),
              _("show information by selected process."));
    mvwaddstr(win, 19, 1, _("Process information hotkeys:"));
    mvwprintw(win, 21, 1, "%-11s %s", _("Enter:"),
              _("back to process list."));
    mvwaddstr(win, 25, 1, _("Press any key to leave help."));
    mvwprintw(win, 27, 1, "%s %s * %s", PACKAGENAME, VERSION, "(C) 2015-2016 Pavel Balaev");

    wrefresh(win);
    ch = wgetch(win);
    if (ch == '*')
    {
        do_hidden_game();
    }

    delwin(win);
}
