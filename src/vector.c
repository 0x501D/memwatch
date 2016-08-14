#define _GNU_SOURCE /* for qsort_r */
#include <stdlib.h>
#include <string.h>

#include <memwatch.h>

static int compare(const void *n1, const void *n2, void *arg)
{
    int *flags = (int *) arg;    
    process_data_t *p1 = (process_data_t *) n1; 
    process_data_t *p2 = (process_data_t *) n2; 
    uint64_t val1, val2;

    if (*(flags) & SORT_RSS_FL)
    {
        val1 = p1->vm_rss;
        val2 = p2->vm_rss;
    }
    else if (*(flags) & SORT_SHM_FL)
    {
        val1 = p1->vm_shr;
        val2 = p2->vm_shr;
    }
    else if (*(flags) & SORT_VIR_FL)
    {
        val1 = p1->vm_size;
        val2 = p2->vm_size;
    }
    else if (*(flags) & SORT_SWP_FL)
    {
        val1 = p1->vm_swap;
        val2 = p2->vm_swap;
    }

    if (*(flags) & SORT_REV_FL)
    {
        return val1 - val2;
    }
    return val2 - val1;
}

static int compare_string(const void *n1, const void *n2)
{
    char proc_name_1[MAX_CMDLINE + 1] = {0};
    char proc_name_2[MAX_CMDLINE + 1] = {0};
    char *space = NULL;

    process_data_t *p1 = (process_data_t *) n1;
    process_data_t *p2 = (process_data_t *) n2;

    strncpy(proc_name_1, p1->cmdline, MAX_CMDLINE + 1);
    strncpy(proc_name_2, p2->cmdline, MAX_CMDLINE + 1);

    /* remove process arguments */
    space = strchr(proc_name_1, ' ');
    if (space)
    {
        *space = '\0';
        space = NULL;
    }

    space = strchr(proc_name_2, ' ');
    if (space)
    {
        *space = '\0';
    }

    return strcmp(basename(proc_name_1), basename(proc_name_2));
}

const process_data_t *vector_search(vector_process_t *v, const char *name)
{
    process_data_t key;
    strncpy(key.cmdline, name, MAX_CMDLINE);

    qsort(v->items, v->total, sizeof(process_data_t), compare_string);

    return bsearch(&key, v->items, v->total, sizeof(process_data_t), compare_string);
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
