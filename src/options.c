#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>

#include <memwatch.h>

static void print_help(FILE *out)
{
    fprintf(out, _("Usage:\n  %s [options]\n\n"), PACKAGENAME);
    fputs(_("Options:\n"), out);
    fputs(_(" -d N, --delay N: refresh timeout in N seconds\n"), out);
    fputs(_(" -S, --si:        use power of 1000 instead of 1024\n"), out);
    fputs(_(" -b, --bytes:     display the amount of memory in bytes\n"), out);
    fputs(_(" -k, --kilo:      display the amount of memory in kilobytes\n"), out);
    fputs(_(" -m, --mega:      display the amount of memory in megabytes\n"), out);
    fputs(_(" -g, --giga:      display the amount of memory in gigabytes\n"), out);
    fputs(_(" -t, --tera:      display the amount of memory in terabytes\n"), out);
    fputs(_(" -h, --human:     display the amount of memory in human readable format(default)\n"), out);
    fputs(_(" -l, --list:      print memory usage per process\n"), out);
    fputs(_(" -V, --version:   print version\n"), out);
    fputs(_("     --help:      print help and exit\n"), out);
    fprintf(out, _("\nFor more details see %s(1).\n"), PACKAGENAME);
}

void parse_options(int argc, char *const argv[], options_t *options)
{
    int opt;
    float delay = 0;

    enum {
        HELP_OPTION = CHAR_MAX + 1
    };

    static const struct option longopts[] = {
        { "delay",   required_argument, NULL, 'd'         },
        { "si",      no_argument,       NULL, 'S'         },
        { "bytes",   no_argument,       NULL, 'b'         },
        { "kilo",    no_argument,       NULL, 'k'         },
        { "mega",    no_argument,       NULL, 'm'         },
        { "giga",    no_argument,       NULL, 'g'         },
        { "tera",    no_argument,       NULL, 't'         },
        { "human",   no_argument,       NULL, 'h'         },
        { "list",    no_argument,       NULL, 'l'         },
        { "version", no_argument,       NULL, 'V'         },
        { "help",    no_argument,       NULL, HELP_OPTION },
        { NULL, 0, NULL, 0                                }
    };

    while ((opt = getopt_long(argc, argv, "d:SbkmgthlV", longopts, NULL)) != -1)
    {
        switch (opt)
        {
            case 'd':
                delay = atof(optarg);
                if ((delay >= DELAY_MIN) &&
                    (delay <= DELAY_MAX))
                {
                    delay *= DELAY_MULTIPLIER;
                    options->delay = delay;
                    options->flags |= DELAY_FL;
                }
                else
                {
                     fprintf(stderr, _("Supported delay range %.1f-%d\n"),
                             DELAY_MIN, DELAY_MAX);
                     exit(EXIT_FAILURE);
                }
                break;

            case 'S':
                options->power = SI_UNITS_POWER;
                options->flags |= SI_UNITS_FL;
                break;

            case 'b':
                options->flags &= ~CLEAR_SZ_FLAGS;
                options->flags |= BYTES_FL | SIZE_OUT_FL;
                break;

            case 'k':
                options->flags &= ~CLEAR_SZ_FLAGS;
                options->flags |= KILOBYTES_FL | SIZE_OUT_FL;
                break;

            case 'm':
                options->flags &= ~CLEAR_SZ_FLAGS;
                options->flags |= MEGABYTES_FL | SIZE_OUT_FL;
                break;

            case 'g':
                options->flags &= ~CLEAR_SZ_FLAGS;
                options->flags |= GIGABYTES_FL | SIZE_OUT_FL;
                break;

            case 't':
                options->flags &= ~CLEAR_SZ_FLAGS;
                options->flags |= TERABYTES_FL | SIZE_OUT_FL;
                break;

            case 'h':
                options->flags &= ~CLEAR_SZ_FLAGS;
                options->flags |= HUMAN_RD_FL | SIZE_OUT_FL;
                break;

            case 'l':
                options->flags |= PROC_LIST_FL;
                break;

            case 'V':
                printf("%s v%s\n", PACKAGENAME, VERSION);
                exit(EXIT_SUCCESS);

            case HELP_OPTION:
                print_help(stdout);
                exit(EXIT_SUCCESS);

            default:
                print_help(stderr);
                exit(EXIT_FAILURE);
        }
    }

    if (!(options->flags & DELAY_FL))
    {
        options->delay = DEFAULT_DELAY * DELAY_MULTIPLIER;
    }
    if (!(options->flags & SI_UNITS_FL))
    {
        options->power = DEFAULT_POWER;
    }
    if (!(options->flags & SIZE_OUT_FL))
    {
        options->flags |= HUMAN_RD_FL;
    }
    /* set default sort field */
    options->flags |= SORT_RSS_FL;
}
