/* This programm prints memory info in real time. 
 * compile: gcc memwatch.c -o memwatch -lcurses -lm -Wall 
 * author: PascalRD <pascal at unixdev ru> */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <math.h>
#include <libgen.h>
#include <sys/time.h>

#define MEMFILE "/proc/meminfo"
#define BAR_LEHGTH 30
#define DELAY 1000
#define DELAY_MIN 100
#define DELAY_MAX 100000

typedef struct mem_s {
     char *mem_total_s, *mem_free_s, *mem_used_s;
     char *swap_total_s, *swap_free_s, *swap_used_s;
     uint32_t mem_total_d, mem_free_d, mem_used_d;
     uint32_t swap_total_d, swap_free_d, swap_used_d;
} mem_t;

enum {
     FREE_MEM = 1, TOTAL_MEM = 2,
     FREE_SWAP = 3, TOTAL_SWAP = 4
};

void set_timer(uint32_t);
void get_data(int);
void insert_value(mem_t *, char *, int);
void print_info(const mem_t *);
void clear_data(mem_t *);
void print_bar(uint32_t, uint32_t, uint32_t);

int main(int argc, char **argv)
{
     int c;
     int opt, d_flag, u_delay;
     d_flag = 0;

     while((opt = getopt(argc, argv, "hd:")) != -1)
     {
          switch(opt)
          {
               case 'd':
                    u_delay = atoi(optarg);
                    if(u_delay >= DELAY_MIN && u_delay <= DELAY_MAX)
                    {
                         d_flag = 1;
                    }
                    else
                    {
                         fprintf(stderr, "Supported delay range %d-%d\n", DELAY_MIN, DELAY_MAX);
                         exit(EXIT_FAILURE);
                    }
                    break;
               case 'h':
               default:
                    fprintf(stderr, "Usage: %s [-d ms]\n", basename(argv[0]));
                    exit(EXIT_FAILURE);
          }
     }

     d_flag ? set_timer(u_delay) : set_timer(DELAY);
     signal(SIGALRM, get_data);

     initscr();
     noecho();
     curs_set(0);
     cbreak();
     clear();

     get_data(0);

     while((c = getch()) != 'q')
     {
          pause();
     }

     endwin();

     return 0;
}

void set_timer(uint32_t usec)
{
     struct itimerval timer;

     uint32_t msec = (usec % 1000) * 1000;
     uint32_t sec  = usec / 1000;

     timer.it_interval.tv_sec = sec;
     timer.it_interval.tv_usec = msec;
     timer.it_value.tv_sec = sec;
     timer.it_value.tv_usec = msec;

     setitimer(ITIMER_REAL, &timer, NULL);
}

void get_data(int signum)
{
     FILE *fp;
     char *line = NULL;
     size_t len = 0;
     ssize_t read;
     mem_t memory;
     memset(&memory, 0, sizeof(memory));

     if((fp = fopen(MEMFILE, "r")) == NULL)
     {
          nocbreak();
          mvaddstr(1, 1, "Error opening file:");
          mvaddstr(2, 1, MEMFILE);
          getch();
          endwin();
          exit(1);
     }

     while((read = getline(&line, &len, fp)) != -1)
     {
          if(strncmp("MemFree:", line, strlen("MemFree:")) == 0)
          {
               insert_value(&memory, line, FREE_MEM);
          }
          else if(strncmp("MemTotal:", line, strlen("MemTotal:")) == 0)
          {
               insert_value(&memory, line, TOTAL_MEM);
          }
          else if(strncmp("SwapFree:", line, strlen("SwapFree:")) == 0)
          {
               insert_value(&memory, line, FREE_SWAP);
          }
          else if(strncmp("SwapTotal:", line, strlen("SwapTotal:")) == 0)
          {
               insert_value(&memory, line, TOTAL_SWAP);
          }
     }

     free(line);
     fclose(fp);

     print_info(&memory);
     clear_data(&memory);
}

void print_info(const mem_t *mem)
{
     float mem_ratio = (float) mem->mem_used_d / mem->mem_total_d;
     float mem_bar_used = BAR_LEHGTH * mem_ratio;
     uint32_t mem_bar_free = BAR_LEHGTH - floor(mem_bar_used);
     float swap_ratio = (float) mem->swap_used_d / mem->swap_total_d;
     float swap_bar_used = BAR_LEHGTH * swap_ratio;
     uint32_t swap_bar_free = BAR_LEHGTH - floor(swap_bar_used);
     size_t i;
     uint16_t col = 1;

     for(i = 1; i <= 15; i++)
     {
          mvaddstr(i, 1, "                       ");
     }

     mvaddstr(col, 1, "Memory:");
     col++; col++;
     print_bar(col, mem_bar_used, mem_bar_free);
     col++; col++;
     mvaddstr(col, 1, "Total:");
     mvaddstr(col++, 10, mem->mem_total_s);
     mvaddstr(col, 1, "Free:");
     mvaddstr(col++, 10, mem->mem_free_s);
     mvaddstr(col, 1, "Used:");
     mvaddstr(col, 10, mem->mem_used_s);

     col++; col++;
     mvaddstr(col, 1, "Swap:");
     col++; col++;
     print_bar(col, swap_bar_used, swap_bar_free);
     col++; col++;
     mvaddstr(col, 1, "Total:");
     mvaddstr(col++, 10, mem->swap_total_s);
     mvaddstr(col, 1, "Free:");
     mvaddstr(col++, 10, mem->swap_free_s);
     mvaddstr(col, 1, "Used:");
     mvaddstr(col, 10, mem->swap_used_s);

     refresh();
}

void print_bar(uint32_t col, uint32_t used, uint32_t last)
{
     size_t i, row = 2;

     mvaddch(col, 1, '[');
     for(i = 0; i < used; i++, row++)
     {
          mvaddch(col, row, '#');
     }
     for(i = 0; i < last; i++, row++)
     {
          mvaddch(col, row, '-');
     }
     mvaddch(col, row, ']');
}

void clear_data(mem_t *mem)
{
     free(mem->mem_free_s);
     free(mem->mem_total_s);
     free(mem->mem_used_s);
     free(mem->swap_free_s);
     free(mem->swap_total_s);
     free(mem->swap_used_s);
}

void insert_value(mem_t *mem, char *line, int ch)
{
     char *token;
     char buf[21];
     uint32_t num;

     token = strtok(line, " ");
     token = strtok(NULL, " ");
     num = (atoi(token)) / 1024;
     snprintf(buf, 20, "%d", num);

     switch(ch)
     {
          case FREE_MEM:
               mem->mem_free_d = num;
               mem->mem_free_s = strdup(buf);
               mem->mem_used_d = mem->mem_total_d - mem->mem_free_d;
               memset(&buf, 0, sizeof(buf));
               snprintf(buf, 20, "%d", mem->mem_used_d);
               mem->mem_used_s = strdup(buf);
               break;
          case FREE_SWAP:
               mem->swap_free_d = num;
               mem->swap_free_s = strdup(buf);
               mem->swap_used_d = mem->swap_total_d - mem->swap_free_d;
               memset(&buf, 0, sizeof(buf));
               snprintf(buf, 20, "%d", mem->swap_used_d);
               mem->swap_used_s = strdup(buf);
               break;
          case TOTAL_MEM:
               mem->mem_total_d = num;
               mem->mem_total_s = strdup(buf);
               break;
          case TOTAL_SWAP:
               mem->swap_total_d = num;
               mem->swap_total_s = strdup(buf);
               break;
     }
}
