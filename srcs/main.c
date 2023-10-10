#include "../inc/ft_ls.h"

opt parse_option(char *argv[])
{
    opt option = NO_OPTION;
    for (size_t i = 1; argv[i]; i++)
    {
        for (size_t j = 1; argv[i][0] == '-' && argv[i][j]; j++)
        {
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
        }
    }
    return option;
}



int main(int argc, char **argv)
{
    if (argc == 1)
        ls(".", NO_OPTION);
    opt option = parse_option(argv);

    // Better way of doing this ?
    for (size_t i = 1; argv[i]; i++)
        if (argv[i][0] != '-')
        {
            ft_printf("%s:\n", argv[i]);
            ls(argv[i], option);
            if ((int)i < argc - 1)
                ft_printf("\n");
        }
}