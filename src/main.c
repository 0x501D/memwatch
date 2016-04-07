#include <memwatch.h>
#include <options.h>
#include <common.h>
#include <draw.h>

int main(int argc, char **argv)
{
    int c;
    options_t options;

    memset(&options, 0, sizeof(options));

    parse_options(argc, argv, &options);

    set_timer(options.delay);
    signal(SIGALRM, get_data);

    config_curses();

    get_data(0);

    while ((c = getch()) != 'q')
    {
        pause();
    }

    endwin();

    return 0;
}
