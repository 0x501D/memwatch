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
#define STATUS_VPEAK "VmPeak"
#define STATUS_DATA  "VmData"
#define STATUS_STK   "VmStk"
#define STATUS_EXE   "VmExe"
#define STATUS_LIB   "VmLib"
#define STATUS_RPEAK "VmHWM"

void print_process_list(const options_t *options,
                        list_navi_t *navi, vector_process_t *v);
void print_single_process(options_t *options, list_navi_t *navi,
                          process_data_t *ps);
pid_t search_pid_by_name(vector_process_t *v);

#endif /* PROCESS_INFO_H_ */
