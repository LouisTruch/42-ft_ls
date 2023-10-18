#include "../inc/ft_ls.h"

void print_file_name(t_metadata *metadata, t_print_opt *print)
{
    if (OPT_ISCOLOR(print->option))
        ft_printf("%s%s%s", metadata->color, metadata->name, COLOR_RESET);
    else
        ft_printf("%s", metadata->name);
}

void print_ls(char *argv, t_file *lst_file, t_print_opt *print)
{
    // if (OPT_ISRECRSV(print->option))
        // ft_printf("%s:\n", argv);

    if (OPT_ISLIST(print->option))
        print_list_format(argv, lst_file, print);
    else
        print_default_format(lst_file, print);

    if (OPT_ISRECRSV(print->option))
        handle_recursive(argv, lst_file, print);
}