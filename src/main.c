#include <memwatch.h>
#include <options.h>
#include <common.h>
#include <draw.h>

int main(int argc, char **argv)
{
    int ch, idle = 0, quit = 0;
    int32_t timer;
    struct options options;

    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGENAME, LOCALEDIR);
    textdomain(PACKAGENAME);

    memset(&options, 0, sizeof(options));

    parse_options(argc, argv, &options);
    config_curses();

    timer = options.delay;

    while (!quit)
    {
        if (!idle)
        {
            get_data(&options);
        }

        ch = getch();
        switch (ch)
        {
            case 'q':
                quit = 1;
                break;
            case 'k':
                options.flags &= ~CLEAR_SZ_FLAGS;
                options.flags |= KILOBYTES_FL;
                break;
            case 'm':
                options.flags &= ~CLEAR_SZ_FLAGS;
                options.flags |= MEGABYTES_FL;
                break;
            case 'b':
                options.flags &= ~CLEAR_SZ_FLAGS;
                options.flags |= BYTES_FL;
                break;
        }

        if (timer > 0)
        {
            timer -= 1000;
            usleep(1000);
            idle = 1;
            continue;
        }

        timer = options.delay;
        idle = 0;
    }

    endwin();

    return 0;
}
