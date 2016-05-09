#include <memwatch.h>
#include <options.h>
#include <common.h>
#include <draw.h>

int main(int argc, char **argv)
{
    int ch, idle = 0, quit = 0;
    int key_pressed = 0;
    int32_t timer;
    options_t options;

    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGENAME, LOCALEDIR);
    textdomain(PACKAGENAME);

    memset(&options, 0, sizeof(options));

    parse_options(argc, argv, &options);
    config_curses();

    timer = options.delay;

    while (!quit)
    {
        if (!idle || key_pressed)
        {
            get_data(&options);
            key_pressed = 0;
        }

        ch = getch();
        switch (ch)
        {
            case 'q':
                quit = 1;
                break;
            case 'S':
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
