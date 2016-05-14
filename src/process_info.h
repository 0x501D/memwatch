#ifndef PROCESS_INFO_H_
#define PROCESS_INFO_H_

#define MAX_NAME_LEN 128

#include <memwatch.h>

void print_process_list(const options_t *options, list_navi_t *navi);

typedef struct process_data_s {
    pid_t pid;
    pid_t ppid;
    uid_t uid;
    gid_t guid;
    char name[MAX_NAME_LEN + 1];
    uint64_t vm_peak;
    uint64_t vm_size;
    uint64_t vm_hwv;
    uint64_t vm_rss;
    uint64_t vm_data;
    uint64_t vm_stk;
    uint64_t vm_exe;
    uint64_t vm_lib;
    uint64_t vm_swap;
} process_data_t;

#endif /* PROCESS_INFO_H_ */
