#include <stdio.h>
#include <unistd.h>
#include <libgen.h>

#include <memwatch.h>

static void print_help(FILE *s, char *const prog)
{
    fprintf(s, "Usage: %s [options]\n%s\n", basename(prog),
               " -d ms: refresh timeout in milliseconds\n"
               " -v: print version\n"
               " -h: print help and exit");
}

void parse_options(int argc, char *const argv[], options_t *options)
{
    int opt;

    while ((opt = getopt(argc, argv, "hvd:")) != -1)
    {
        switch (opt)
        {
            case 'd':
                options->delay = atoi(optarg);
                if (options->delay >= DELAY_MIN &&
                    options->delay <= DELAY_MAX)
                {
                    options->delay *= 1000;
                    options->flags |= DELAY_FL;
                }
                else
                {
                     fprintf(stderr, "Supported delay range %d-%d\n", 
                             DELAY_MIN, DELAY_MAX);
                     exit(EXIT_FAILURE);
                }
                break;

            case 'v':
                printf("memwatch v%s\n", VERSION);
                exit(EXIT_SUCCESS);

            case 'h':
                print_help(stdout, argv[0]);
                exit(EXIT_SUCCESS);

            default:
                print_help(stderr, argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (!(options->flags & DELAY_FL))
    {
        options->delay = DEFAULT_DELAY * 1000;
    }
    options->flags |= MEGABYTES_FL;
}
