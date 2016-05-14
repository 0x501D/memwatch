#ifndef VECTOR_H_
#define VECTOR_H_

#include <process_info.h>

typedef struct vector_process_s {
    size_t size;
    size_t total;
    process_data_t *items;
} vector_process_t;

#endif /* VECTOR_H_ */
