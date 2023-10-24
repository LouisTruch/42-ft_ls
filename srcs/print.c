#include "../inc/ft_ls.h"

void print_ls(char *argv, t_file *lst_file, t_print_opt *print)
{
    if (OPT_IS_LIST(print->option))
        print_list_format(argv, lst_file, print);
    else
        print_default_format(lst_file, print);

    if (OPT_IS_RECRSV(print->option))
        handle_recursive(argv, lst_file, print);
}