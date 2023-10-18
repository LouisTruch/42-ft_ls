#include "../inc/ft_ls.h"

int main(int argc, char **argv)
{
    t_print_opt print = {SORT_ALPHABETICAL, NO_OPTION, PRINT_LAST_MODIF, {1, 1, 1, 1, 1, 3, 1, 4}, 0};
    parse_option(argv, &print);
    remove_flags_argv(&argc, argv, &print);
    if (argc == 0)
    {
        ls(".", &print);
        return LS_SUCCESS;
    }
    t_file *arg_lst = NULL;
    for (int i = 0; argv[i]; i++)
    {
        errno = ERRNO_RESET;
        struct stat sb;
        if (lstat(argv[i], &sb) == -1)
        {
            ft_printf("ls: cannot access '%s': %s\n", argv[i], strerror(errno));
            continue;
        }
        t_file *new_file = lst_new(argv[i], &sb, &print);
        if (!new_file)
        {
            ft_dprintf(STDERR_FILENO, "Allocation error\n");
            lst_clear(&arg_lst);
            return MALLOC_FAIL;
        }
        lst_addback(&arg_lst, new_file);
    }
    sort_lst_file(&arg_lst, &print);
    t_file *head = arg_lst;
    while (head)
    {
        ls(head->metadata->name, &print);
        if (head->next)
            write(1, "\n", 1);
        head = head->next;
    }

    lst_clear(&arg_lst);
    return LS_SUCCESS;
}