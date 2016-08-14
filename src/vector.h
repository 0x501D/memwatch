#ifndef VECTOR_H_
#define VECTOR_H_

int vector_init(vector_process_t *v, size_t nmemb);
int vector_insert(vector_process_t *v, const process_data_t *data);
const process_data_t *vector_at(const vector_process_t *v, size_t nmemb);
void vector_free(vector_process_t *v);
void vector_clear(vector_process_t *v);
void vector_sort(vector_process_t *v, int flags);
const process_data_t *vector_search(vector_process_t *v, const char *name);

#endif /* VECTOR_H_ */
