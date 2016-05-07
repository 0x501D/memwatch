#ifndef MEMWATCH_H_
#define MEMWATCH_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <errno.h>
#include <libintl.h>
#include <locale.h>
#include <sys/time.h>

#define PACKAGENAME "memwatch"
#define _(text) gettext(text)

#ifndef LOCALEDIR
#define LOCALEDIR "/usr/share/locale"
#endif

#define MEMFILE "/proc/meminfo"
#define BAR_LEHGTH 30
#define DEFAULT_DELAY 1
#define DEFAULT_POWER 1024
#define ADVANCED_POWER 1000
#define ADV_POWER_DIV 0.9765625f
#define DELAY_MULTIPLIER 1000000
#define DELAY_MIN 0.1f
#define DELAY_MAX 60
#define CLEAR_SZ_FLAGS 0x7e

#ifndef VERSION
#define VERSION "0.2.1"
#endif

enum {
    DELAY_FL     = 1,
    MEGABYTES_FL = 1 << 1,
    GIGABYTES_FL = 1 << 2,
    KILOBYTES_FL = 1 << 3,
    BYTES_FL     = 1 << 4,
    TERABYTES_FL = 1 << 5,
    HUMAN_RD_FL  = 1 << 6,
    ADV_POWER_FL = 1 << 7,
    SIZE_OUT_FL  = 1 << 8
};

typedef struct options_s {
    int32_t delay;
    uint16_t power;
    int flags;
} options_t;

#endif /* MEMWATCH_H_ */
