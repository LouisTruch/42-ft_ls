#include "../inc/ft_ls.h"

void handle_recursive(char *argv, t_file *lst_file, opt option)
{
    while (lst_file != NULL)
    {
        if (S_ISDIR(lst_file->metadata->mode) && ft_strcmp(lst_file->metadata->name, ".") && ft_strcmp(lst_file->metadata->name, ".."))
        {
            size_t path_len = ft_strlen(argv) + 1 + ft_strlen(lst_file->metadata->name);
            char file_path[path_len];
            get_complete_path(file_path, argv, lst_file->metadata->name);
            ft_printf("\n");
            ls(file_path, option);
        }
        lst_file = lst_file->next;
    }
}

void ls(char *argv, opt option)
{
    if (argv[0] == '/' && argv[1] == '/')
    {
        argv++;
    }

    DIR *dir_stream = opendir(argv);
    if (!dir_stream)
    {
        ft_dprintf(STDERR_FILENO, "ls: cannot access '%s': No such file or directory\n", argv);
        return;
    }
    t_file *lst_file = NULL;
    for (struct dirent *dir = readdir(dir_stream); dir; dir = readdir(dir_stream))
    {
        if (!OPT_ISHIDDN(option) && !ft_strncmp(".", dir->d_name, 1))
            continue;

        size_t path_len = ft_strlen(argv) + 1 + ft_strlen(dir->d_name);
        char file_path[path_len];
        get_complete_path(file_path, argv, dir->d_name);

        errno = ERRNO_RESET;
        struct stat sb;
        if (lstat(file_path, &sb) == -1)
        {
            ft_dprintf(STDERR_FILENO, "ls: cannot access '%s'\n", file_path);
            perror("");
            dir = readdir(dir_stream);
        }
        t_file *new_file = lst_new(dir->d_name, sb);
        if (!new_file)
        {
            ft_dprintf(STDERR_FILENO, "Allocation error\n");
            lst_clear(&lst_file);
            return;
        }
        lst_addback(&lst_file, new_file);
    }
    sort_lst_file(&lst_file, option);
    if (OPT_ISRECRSV(option))
        ft_printf("%s:\n", argv);

    if (OPT_ISLIST(option))
        print_list(argv, lst_file);
    else
        print_default(lst_file);

    if (OPT_ISRECRSV(option))
        handle_recursive(argv, lst_file, option);

    closedir(dir_stream);
    lst_clear(&lst_file);
}