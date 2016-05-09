#include <memwatch.h>
#include <memory_info.h>
#include <process_info.h>

void print_info(const options_t *options)
{
    if (options->flags & PROC_LIST_FL)
    {
        print_process_list(options);
        return;
    }

    print_memory_info(options);
}
