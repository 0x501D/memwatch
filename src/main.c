#include <memwatch.h>
#include <options.h>
#include <common.h>
#include <vector.h>
#include <process_info.h>
#include <memory_info.h>

int main(int argc, char **argv)
{
    int idle = 0, quit = 0;
    int key_pressed = 0;
    int32_t timer;
    options_t options;
    list_navi_t navi;
    vector_process_t v;
    process_data_t *ps = NULL;

    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGENAME, LOCALEDIR);
    textdomain(PACKAGENAME);

    memset(&options, 0, sizeof(options));
    memset(&navi, 0, sizeof(navi));
    memset(&v, 0, sizeof(v));

    parse_options(argc, argv, &options, &navi);
    config_curses();

    timer = options.delay;
    navi.highlight = 1;

    if (navi.flags & NAVI_PID_FROM_ARGS)
    {
        ps = (process_data_t *) malloc(sizeof(process_data_t));
        if (!ps)
            err_exit("alloc failed: %s", strerror(errno));
    }

    while (!quit)
    {
        wchar_t ch = 0;

        if (!idle || key_pressed)
        {
            if (options.flags & PROC_LIST_FL)
            {
                print_process_list(&options, &navi, &v);
            }
            else if (options.flags & SINGLE_PS_FL)
            {
                print_single_process(&options, &navi, ps);
            }
            else
            {
                print_memory_info(&options);
            }

            key_pressed = 0;
            options.flags &= ~REPRINT_FL;
        }

        ch = getch();
        switch (ch)
        {
            case 'q':
                quit = 1;
                break;
            case '?':
            case KEY_F(1):
                print_hotkeys_help();
                key_pressed = 1;
                break;

            case '/':
                {
                    pid_t search_pid;

                    if (options.flags & PROC_LIST_FL)
                    {
                        search_pid = search_pid_by_name(&v);
                        if (search_pid > 0)
                        {
                            navi.cur_ps = search_pid;
                            options.flags &= ~PROC_LIST_FL;
                            options.flags |= SINGLE_PS_FL;
                            ps = (process_data_t *) malloc(sizeof(process_data_t));
                            if (!ps)
                                err_exit("alloc failed: %s", strerror(errno));
                        }
                    }
                }
                key_pressed = 1;
                break;

            case 10: /* Enter */
                if (options.flags & PROC_LIST_FL)
                {
                    options.flags &= ~PROC_LIST_FL;
                    options.flags |= SINGLE_PS_FL;
                    key_pressed = 1;
                    ps = (process_data_t *) malloc(sizeof(process_data_t));
                    if (!ps)
                        err_exit("alloc failed: %s", strerror(errno));
                }
                else if ((options.flags & SINGLE_PS_FL) &&
                        !(navi.flags & NAVI_PID_FROM_ARGS))
                {
                    options.flags &= ~SINGLE_PS_FL;
                    options.flags |= PROC_LIST_FL;
                    key_pressed = 1;
                    free(ps);
                    ps = NULL;
                }
                break;

            case 's':
                if (options.power == DEFAULT_POWER)
                {
                    options.power = SI_UNITS_POWER;
                    options.flags |= SI_UNITS_FL;
                }
                else
                {
                    options.power = DEFAULT_POWER;
                    options.flags &= ~SI_UNITS_FL;
                }
                key_pressed = 1;
                break;

            case 'k':
                options.flags &= ~CLEAR_SZ_FLAGS;
                options.flags |= KILOBYTES_FL;
                key_pressed = 1;
                break;

            case 'm':
                options.flags &= ~CLEAR_SZ_FLAGS;
                options.flags |= MEGABYTES_FL;
                key_pressed = 1;
                break;

            case 'b':
                options.flags &= ~CLEAR_SZ_FLAGS;
                options.flags |= BYTES_FL;
                key_pressed = 1;
                break;

            case 'g':
                options.flags &= ~CLEAR_SZ_FLAGS;
                options.flags |= GIGABYTES_FL;
                key_pressed = 1;
                break;

            case 't':
                options.flags &= ~CLEAR_SZ_FLAGS;
                options.flags |= TERABYTES_FL;
                key_pressed = 1;
                break;

            case 'h':
                options.flags &= ~CLEAR_SZ_FLAGS;
                options.flags |= HUMAN_RD_FL;
                key_pressed = 1;
                break;

            case 'l':
                if (!(options.flags & SINGLE_PS_FL))
                {
                    if (options.flags & PROC_LIST_FL)
                        options.flags &= ~PROC_LIST_FL;
                    else
                        options.flags |= PROC_LIST_FL;

                    key_pressed = 1;
                }
                break;

            case KEY_END:
                navi.flags |= (NAVI_GO_LAST_FL | REPRINT_FL);
                key_pressed = 1;
                break;

            case KEY_HOME:
                navi.highlight = 1;
                navi.offset = 0;
                options.flags |= REPRINT_FL;
                key_pressed = 1;
                break;

            case KEY_UP:
                if (navi.highlight == 1)
                {
                    if (navi.offset > 0)
                        navi.offset--;
                    else
                        navi.flags |= NAVI_GO_LAST_FL;
                }
                else
                    navi.highlight--;

                key_pressed = 1;
                options.flags |= REPRINT_FL;
                break;

            case KEY_DOWN:
                if (navi.highlight == (uint32_t) LINES - 4)
                    navi.offset++;
                else
                    navi.highlight++;

                key_pressed = 1;
                options.flags |= REPRINT_FL;
                break;

            case 'r':
                if (options.flags & SORT_REV_FL)
                    options.flags &= ~SORT_REV_FL;
                else
                    options.flags |= SORT_REV_FL;

                key_pressed = 1;
                break;

            case '1':
                options.flags &= ~CLEAR_SORT_FLAGS;
                options.flags |= SORT_RSS_FL;
                key_pressed = 1;
                break;

            case '2':
                options.flags &= ~CLEAR_SORT_FLAGS;
                options.flags |= SORT_SHM_FL;
                key_pressed = 1;
                break;

            case '3':
                options.flags &= ~CLEAR_SORT_FLAGS;
                options.flags |= SORT_VIR_FL;
                key_pressed = 1;
                break;

            case '4':
                options.flags &= ~CLEAR_SORT_FLAGS;
                options.flags |= SORT_SWP_FL;
                key_pressed = 1;
                break;
        }

        if (navi.flags & NAVI_FIXED_PS_EXITED)
        {
            navi.flags &= ~NAVI_FIXED_PS_EXITED;
            key_pressed = 1;
            if (ps)
            {
                free(ps);
                ps = NULL;
            }
            if (navi.flags & NAVI_NEED_EXIT)
                quit = 1;
        }

        if (timer > 0)
        {
            timer -= 10000;
            usleep(10000);
            idle = 1;
            continue;
        }

        timer = options.delay;
        idle = 0;
    }

    mvhline(LINES - 1, 0, ' ', COLS);
    refresh();
    endwin();
    if (ps)
        free(ps);

    vector_free(&v);

    return 0;
}
