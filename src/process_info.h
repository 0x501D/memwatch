#ifndef PROCESS_INFO_H_
#define PROCESS_INFO_H_

#include <memwatch.h>

#define STATUS_NAME  "Name"
#define STATUS_PID   "Pid"
#define STATUS_PPID  "PPid"
#define STATUS_UID   "Uid"
#define STATUS_RSS   "VmRSS"
#define STATUS_SWAP  "VmSwap"
#define STATUS_SHR   "RssFile"
#define STATUS_VIRT  "VmSize"
#define STATUS_STATE "State"

void print_process_list(const options_t *options,
                        list_navi_t *navi, vector_process_t *v);
void print_single_process(options_t *options,
                          list_navi_t *navi, vector_process_t *v);

#endif /* PROCESS_INFO_H_ */
