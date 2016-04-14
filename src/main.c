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

    config_curses();

    do
    {
        get_data(&options);
    } while ((c = getch()) != 'q');

    endwin();

    return 0;
}
