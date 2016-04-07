#include <memwatch.h>

char *num_to_str(char* buf, uint64_t num)
{
    snprintf(buf, BUFDEC, "%lu", num);
    return buf;
}
