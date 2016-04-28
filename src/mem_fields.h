#ifndef MEM_FIELDS_H_
#define MEM_FIELDS_H_

#include <stdint.h>

typedef struct mem_field_s {
    char *name;
    uint64_t value;
} mem_field_t;

mem_field_t *get_mem_field(const char *, unsigned int);

#endif /* MEM_FIELDS_H_ */
