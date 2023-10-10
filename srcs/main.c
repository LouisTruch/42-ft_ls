#include "../inc/ft_ls.h"

opt parse_option(char **argv)
{
    opt option = NO_OPTION;
    for (size_t i = 1; argv[i]; i++)
        for (size_t j = 1; argv[i][0] == '-' && argv[i][j]; j++)
            switch (argv[i][j])
            {
            case 'l':
                option |= OPT_LIST;
                break;
            case 'R':
                option |= OPT_RECRSV;
                break;
            case 'a':
                option |= OPT_HIDDN;
                break;
            case 'r':
                option |= OPT_REVRS;
                break;
            case 't':
                option |= OPT_TIME;
                break;
            default:
                ft_dprintf(STDERR_FILENO, "ls: invalid option -- '%c'\n", argv[i][j]);
                exit(INVALID_OPTION);
            }
    return option;
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

int main(int argc, char **argv)
{
    opt option = parse_option(argv);
    remove_option_argv(&argc, argv);
    if (argc == 0)
    {
        ls(".", option);
        return EXIT_SUCCESS;
    }

    for (int i = 0; argv[i]; i++)
    {
        if (argc > 1)
            ft_printf("%s:\n", argv[i]);
        ls(argv[i], option);
        if (i < argc - 1)
            ft_printf("\n");
    }
    return EXIT_SUCCESS;
}