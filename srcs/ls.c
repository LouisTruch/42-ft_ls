#include "../inc/ft_ls.h"

void handle_recursive(char *argv, t_file *lst_file, t_print_opt *print)
{
    while (lst_file != NULL)
    {
        if (S_ISDIR(lst_file->metadata->mode) && ft_strcmp(lst_file->metadata->name, ".") && ft_strcmp(lst_file->metadata->name, ".."))
        {
            size_t path_len = ft_strlen(argv) + 1 + ft_strlen(lst_file->metadata->name);
            char file_path[path_len];
            get_complete_path(file_path, argv, lst_file->metadata->name);
            ft_printf("\n");
            ls(file_path, print);
        }
        lst_file = lst_file->next;
    }
}

static void handle_not_dir_input(char *argv, t_print_opt *print)
{
    struct stat sb;
    if (lstat(argv, &sb) == -1)
    {
        ft_dprintf(STDERR_FILENO, "ls: cannot access '%s':%s", argv, strerror(errno));
        return;
    }
    t_file *new_file = lst_new(argv, &sb, OPT_ISCOLOR(print->option));
    if (!new_file)
    {
        ft_dprintf(STDERR_FILENO, "Allocation error\n");
        lst_clear(&new_file);
        return;
    }
    print->option |= notdir;
    print_ls(argv, new_file, print);
    lst_clear(&new_file);
}

void ls(char *argv, t_print_opt *print)
{
    if (argv[0] == '/' && argv[1] == '/')
        argv++;

    errno = ERRNO_RESET;
    DIR *dir_stream = opendir(argv);
    if (!dir_stream)
    {
        if (errno == ENOTDIR)
        {
            handle_not_dir_input(argv, print);
            return;
        }
        ft_dprintf(STDERR_FILENO, "ls: cannot access '%s':%s", argv, strerror(errno));
        return;
    }
    if (OPT_ISLIST(print->option))
        ft_printf("%s:\n", argv);
    t_file *lst_file = NULL;
    for (struct dirent *dir = readdir(dir_stream); dir; dir = readdir(dir_stream))
    {
        if (!OPT_ISHIDDN(print->option) && !ft_strncmp(".", dir->d_name, 1))
            continue;

        size_t path_len = ft_strlen(argv) + 1 + ft_strlen(dir->d_name);
        char file_path[path_len];
        get_complete_path(file_path, argv, dir->d_name);

        errno = ERRNO_RESET;
        struct stat sb;
        if (lstat(file_path, &sb) == -1)
        {
            ft_dprintf(STDERR_FILENO, "ls: cannot access '%s':%s", file_path, strerror(errno));
            continue;
        }
        t_file *new_file = lst_new(dir->d_name, &sb, OPT_ISCOLOR(print->option));
        if (!new_file)
        {
            ft_dprintf(STDERR_FILENO, "Allocation error\n");
            lst_clear(&lst_file);
            return;
        }
        lst_addback(&lst_file, new_file);
    }
    sort_lst_file(&lst_file, print);

    print_ls(argv, lst_file, print);

    closedir(dir_stream);
    lst_clear(&lst_file);
}