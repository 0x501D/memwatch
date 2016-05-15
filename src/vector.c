#include <stdlib.h>
#include <string.h>

#include <memwatch.h>

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
