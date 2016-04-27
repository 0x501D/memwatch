#ifndef MEM_FIELDS_H_
#define MEM_FIELDS_H_

#include <stdint.h>

struct mem_field {
    char *name;
    uint64_t value;
};

struct mem_field *get_mem_field(const char *, unsigned int);

#endif /* MEM_FIELDS_H_ */
