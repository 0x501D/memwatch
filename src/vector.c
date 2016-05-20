#define _GNU_SOURCE /* for qsort_r */
#include <stdlib.h>
#include <string.h>

#include <memwatch.h>

static int compare(const void *n1, const void *n2, void *arg)
{
    int *flags = (int *) arg;    
    process_data_t *p1 = (process_data_t *) n1; 
    process_data_t *p2 = (process_data_t *) n2; 

    return p2->vm_rss - p1->vm_rss;
}

int vector_init(vector_process_t *v, size_t nmemb)
{
    v->size = 0;
    v->total = nmemb;
    v->items = (process_data_t *) calloc(nmemb, sizeof(process_data_t));
    if (v->items == NULL)
    {
        return 1;
    }

    return 0;
}

int vector_insert(vector_process_t *v, const process_data_t *data)
{
    if (v->size == v->total)
    {
        v->total *= 2;
        v->items = (process_data_t *) realloc(v->items, sizeof(process_data_t) * v->total);
        if (v->items == NULL)
        {
            return 1;
        }
    }
    memcpy(&v->items[v->size++], data, sizeof(process_data_t));

    return 0;
}

const process_data_t *vector_at(const vector_process_t *v, size_t nmemb)
{
    if (nmemb > v->size)
    {
        return NULL;
    }

    return &v->items[nmemb];
}

void vector_sort(vector_process_t *v, int flags)
{
    qsort_r(v->items, v->size, sizeof(process_data_t), compare, &flags);
}

void vector_clear(vector_process_t *v)
{
    memset(v->items, 0, v->size * sizeof(process_data_t));
    v->size = 0;
}

void vector_free(vector_process_t *v)
{
    if (v->items == NULL)
    {
        return;
    }

    free(v->items);
}
