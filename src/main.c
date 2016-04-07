#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <sys/time.h>

#include <memwatch.h>
#include <options.h>

int main(int argc, char **argv)
{
     int c;
     options_t options;

     memset(&options, 0, sizeof(options));

     parse_options(argc, argv, &options);

     if (options.flags & FLAG_DELAY)
     {
        set_timer(options.delay);
     }
     else
     {
        set_timer(DELAY);
     }

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

void get_data(UNUSED int signum)
{
     FILE *fp;
     char *line = NULL;
     size_t len = 0;
     ssize_t read;
     mem_t memory;

     memset(&memory, 0, sizeof(memory));
     memory.swap_disabled = 0;

     if((fp = fopen(MEMFILE, "r")) == NULL)
     {
          nocbreak();
          mvaddstr(1, 1, "Error opening file:");
          mvaddstr(2, 1, MEMFILE);
          getch();
          endwin();
          exit(EXIT_FAILURE);
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
          else if(strncmp("Buffers:", line, strlen("Buffers:")) == 0)
          {
               insert_value(&memory, line, BUFF_MEM);
          }
          else if(strncmp("Cached:", line, strlen("Cached:")) == 0)
          {
               insert_value(&memory, line, CACHE_MEM);
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
}

void print_info(const mem_t *mem)
{
     char buf[BUFDEC] = {0};
     float mem_ratio = (float) mem->mem_used / mem->mem_total;
     float mem_bar_used = BAR_LEHGTH * mem_ratio;
     uint32_t mem_bar_free = BAR_LEHGTH - floor(mem_bar_used);
     float swap_ratio = (float) mem->swap_used / mem->swap_total;
     float swap_bar_used = BAR_LEHGTH * swap_ratio;
     uint32_t swap_bar_free = BAR_LEHGTH - floor(swap_bar_used);
     size_t i;
     uint16_t col = 1;

     for(i = 1; i <= 20; i++)
     {
          mvaddstr(i, 1, "                                        ");
     }

     mvaddstr(col, 1, "Memory:");
     col++; col++;
     print_bar(col, mem_bar_used, mem_bar_free);
     col++; col++;
     mvaddstr(col, 1, "Total");
     mvaddstr(col++, 10, num_to_str(buf, mem->mem_total));
     mvaddstr(col, 1, "Free");
     mvaddstr(col++, 10, num_to_str(buf, mem->mem_free));
     mvaddstr(col, 1, "Used");
     mvaddstr(col++, 10, num_to_str(buf, mem->mem_used));
     mvaddstr(col, 1, "Buff");
     mvaddstr(col++, 10, num_to_str(buf, mem->mem_buff));
     mvaddstr(col, 1, "Cache");
     mvaddstr(col++, 10, num_to_str(buf, mem->mem_cache));

     if(mem->swap_disabled)
     {
          col++; col++;
          mvaddstr(col, 1, "Swap disabled.");
     }
     else
     {
          col++; col++;
          mvaddstr(col, 1, "Swap:");
          col++; col++;
          print_bar(col, swap_bar_used, swap_bar_free);
          col++; col++;
          mvaddstr(col, 1, "Total");
          mvaddstr(col++, 10, num_to_str(buf, mem->swap_total));
          mvaddstr(col, 1, "Free");
          mvaddstr(col++, 10, num_to_str(buf, mem->swap_free));
          mvaddstr(col, 1, "Used");
          mvaddstr(col, 10, num_to_str(buf, mem->swap_used));
     }

     refresh();
}

void print_bar(uint32_t col, uint32_t used, uint32_t last)
{
     size_t i, row;
     row = 2;

     mvaddch(col, row - 1, '[');
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

void insert_value(mem_t *mem, char *line, int ch)
{
     char *token;
     char *end = NULL;
     char buf[BUFDEC];
     uint64_t num;
     errno = 0;

     token = strtok(line, " ");
     token = strtok(NULL, " ");
     num = strtoull(token, &end, 10);
     if((token + strlen(token)) != end || errno == ERANGE)
     {
          num = 0;
     }
     else
     {
          num /= DELIM;
     }
     snprintf(buf, sizeof(buf), "%lu", num);

     switch(ch)
     {
          case FREE_MEM:
               mem->mem_free = num;
               mem->mem_used = mem->mem_total - mem->mem_free;
               break;
          case FREE_SWAP:
               if(! mem->swap_disabled)
               {
                    mem->swap_free = num;
                    mem->swap_used = mem->swap_total - mem->swap_free;
               }
               break;
          case TOTAL_MEM:
               mem->mem_total = num;
               break;
          case BUFF_MEM:
               mem->mem_buff = num;
               break;
          case CACHE_MEM:
               mem->mem_cache = num;
               break;
          case TOTAL_SWAP:
               if(! num)
               {
                    mem->swap_disabled = 1;
               }
               else
               {
                    mem->swap_total = num;
               }
               break;
     }
}

char *num_to_str(char* buf, uint64_t num)
{
    snprintf(buf, BUFDEC, "%lu", num);
    return buf;
}
