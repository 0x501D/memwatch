#include <stdio.h>
#include <unistd.h>

#include <memwatch.h>

static void print_help(FILE *out)
{
    fprintf(out, _("Usage: %s [options]\n"), PACKAGENAME);
    fputs(_(" -d ms: refresh timeout in milliseconds\n"), out);
    fputs(_(" -v: print version\n"), out);
    fputs(_(" -h: print help and exit\n"), out);
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
                if ((options->delay >= DELAY_MIN) &&
                    (options->delay <= DELAY_MAX))
                {
                    options->delay *= 1000;
                    options->flags |= DELAY_FL;
                }
                else
                {
                     fprintf(stderr, _("Supported delay range %d-%d\n"),
                             DELAY_MIN, DELAY_MAX);
                     exit(EXIT_FAILURE);
                }
                break;

            case 'v':
                printf("%s v%s\n", PACKAGENAME, VERSION);
                exit(EXIT_SUCCESS);

            case 'h':
                print_help(stdout);
                exit(EXIT_SUCCESS);

            default:
                print_help(stderr);
                exit(EXIT_FAILURE);
        }
    }

    if (!(options->flags & DELAY_FL))
    {
        options->delay = DEFAULT_DELAY * 1000;
    }
    options->flags |= MEGABYTES_FL;
}
