#include "../inc/ft_ls.h"

int main(int argc, char **argv)
{
    t_print_opt print = {ALPHABETICAL, NO_OPTION};
    parse_option(argv, &print);
    remove_flags_argv(&argc, argv, &print);
    if (argc == 0)
    {
        ls(".", &print);
        return EXIT_SUCCESS;
    }
    for (int i = 0; argv[i]; i++)
    {
        if (!ls(argv[i], &print) && i < argc - 1 && !OPT_ISONLYDIR(print.option))
            ft_printf("\n");
    }
    return EXIT_SUCCESS;
}