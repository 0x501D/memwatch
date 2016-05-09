#include <memwatch.h>
#include <memory_info.h>

void print_process_list(const options_t *options)
{
    size_t i;
    for (i = 1; i <= 20; i++)
    {
         mvaddstr(i, 1, ALOTOFSPACES);
    }
    mvaddstr(2, 1, _("Under development"));
    refresh();
}
