#include <memwatch.h>
#include <memory_info.h>
#include <process_info.h>

void print_info(const options_t *options, list_navi_t *navi)
{
    if (options->flags & PROC_LIST_FL)
    {
        print_process_list(options, navi);
        return;
    }

    print_memory_info(options);
}
