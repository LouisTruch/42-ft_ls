#include "../inc/ft_ls.h"

static void parse_option(char **argv, t_print_opt *print)
{
    for (size_t i = 1; argv[i]; i++)
        for (size_t j = 1; argv[i][0] == '-' && argv[i][j]; j++)
        {
            switch (argv[i][j])
            {
            case 'l':
                print->option |= OPT_LIST;
                break;
            case 't':
                print->sort_by = CHRONOLOGICAL;
                break;
            case 'a':
                print->option |= OPT_HIDDN;
                break;
            case 'G':
                if (!OPT_ISFORCE(print->option))
                    print->option |= OPT_COLOR;
                break;
            case 'r':
                if (!OPT_ISFORCE(print->option))
                    print->option |= OPT_REVERS;
                break;
            case 'R':
                print->option |= OPT_RECRSV;
                break;
            case 'f':
                print->sort_by = NO_SORT;
                print->option |= OPT_FORCE;
                print->option |= OPT_HIDDN;
                RESET_BIT(print->option, OPT_COLOR);
                RESET_BIT(print->option, OPT_REVERS);
                RESET_BIT(print->option, OPT_LIST);
                break;
            default:
                ft_dprintf(STDERR_FILENO, "ls: invalid option -- '%c'\n", argv[i][j]);
                exit(EXIT_INVALID_OPTION);
            }
        }
}

// ? Idk
// Remove option and binary argv[0] from argv list
void remove_option_argv(int *argc, char **argv)
{
    for (int argv_idx = 0; argv_idx < *argc; argv_idx++)
        argv[argv_idx] = argv[argv_idx + 1];

    (*argc)--;
    for (int argv_idx = 0; argv[argv_idx]; argv_idx++)
        if (argv[argv_idx][0] == '-' && ft_strlen(argv[argv_idx]) > 1)
        {
            for (int idx = argv_idx; idx < *argc; idx++)
                argv[idx] = argv[idx + 1];
            (*argc)--;
            argv_idx--;
        }
}

#define KRED "\x1b[40;33m"

int main(int argc, char **argv)
{
    t_print_opt print = {ALPHABETICAL, NO_OPTION};
    parse_option(argv, &print);
    remove_option_argv(&argc, argv);
    if (argc == 0)
    {
        ls(".", &print);
        return EXIT_SUCCESS;
    }
    for (int i = 0; argv[i]; i++)
    {
        // if (argc > 1)
        //     ft_printf("%s:\n", argv[i]);
        ls(argv[i], &print);
        if (i < argc - 1)
            ft_printf("\n");
    }
    return EXIT_SUCCESS;
}