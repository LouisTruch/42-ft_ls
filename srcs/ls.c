#include "../inc/ft_ls.h"

static void handle_recursive(char *argv, t_file *lst_file, t_print_opt *print)
{
    while (lst_file != NULL)
    {
        if (S_ISDIR(lst_file->metadata->mode) && ft_strcmp(lst_file->metadata->name, ".") && ft_strcmp(lst_file->metadata->name, ".."))
        {
            size_t path_len = ft_strlen(argv) + ft_strlen(lst_file->metadata->name) + 1;
            char file_path[path_len];
            get_complete_path(file_path, path_len, argv, lst_file->metadata->name);
            write(1, "\n", 1);
            ls(file_path, print);
        }
        lst_file = lst_file->next;
    }
}

static int handle_not_dir_input(char *argv, t_print_opt *print)
{
    errno = ERRNO_RESET;
    struct stat sb;
    if (lstat(argv, &sb) == -1)
    {
        ft_dprintf(STDERR_FILENO, "ls: cannot access '%s': %s", argv, strerror(errno));
        return errno;
    }
    t_file *new_file = lst_new(argv, &sb, print);
    if (!new_file)
    {
        ft_dprintf(STDERR_FILENO, "Allocation error\n");
        lst_clear(&new_file);
        return MALLOC_FAIL;
    }
    print->option |= NOTDIR;
    if (OPT_IS_LIST(print->option))
        print_list_format(argv, new_file, print);
    else
        print_default_format(new_file, print);
    if (OPT_IS_RECRSV(print->option))
        handle_recursive(argv, new_file, print);
    lst_clear(&new_file);
    return LS_SUCCESS;
}

int ls(char *argv, t_print_opt *print)
{
    if (OPT_IS_ONLYDIR(print->option))
        return (handle_not_dir_input(argv, print));

    if (argv[0] == '/' && argv[1] == '/')
        argv++;

    errno = ERRNO_RESET;
    DIR *dir_stream = opendir(argv);
    if (!dir_stream)
    {
        if (errno == ENOTDIR)
            return (handle_not_dir_input(argv, print));

        ft_dprintf(STDERR_FILENO, "ls: cannot access '%s': %s", argv, strerror(errno));
        return errno;
    }
    if (DO_PRINT_DIR_NAME(print->option))
        ft_printf("%s:\n", argv);
    t_file *lst_file = NULL;
    for (struct dirent *dir = readdir(dir_stream); dir; dir = readdir(dir_stream))
    {
        if (!OPT_IS_HIDDEN(print->option) && !ft_strncmp(".", dir->d_name, 1))
            continue;

        size_t path_len = ft_strlen(argv) + 1 + ft_strlen(dir->d_name);
        char file_path[path_len];
        get_complete_path(file_path, path_len, argv, dir->d_name);

        errno = ERRNO_RESET;
        struct stat sb;
        if (lstat(file_path, &sb) == -1)
        {
            ft_dprintf(STDERR_FILENO, "ls: cannot access '%s': %s\n", file_path, strerror(errno));
            if (errno == 116)
            {
                t_file *new_file_error = lst_new(dir->d_name, NULL, print);
                if (!new_file_error)
                {
                    ft_dprintf(STDERR_FILENO, "Allocation error\n");
                    lst_clear(&lst_file);
                    return MALLOC_FAIL;
                }
                lst_addback(&lst_file, new_file_error);
            }

            continue;
        }
        t_file *new_file = lst_new(dir->d_name, &sb, print);
        if (!new_file)
        {
            ft_dprintf(STDERR_FILENO, "Allocation error\n");
            lst_clear(&lst_file);
            return MALLOC_FAIL;
        }
        if (OPT_IS_LIST(print->option))
        {
            get_cols_max_width(new_file->metadata, print->col_width);
            print->total_blcks_alloc += new_file->metadata->blocks;
        }
        lst_addback(&lst_file, new_file);
    }
    sort_lst_file(&lst_file, print);
    if (OPT_IS_LIST(print->option))
        print_list_format(argv, lst_file, print);
    else
        print_default_format(lst_file, print);
    if (OPT_IS_RECRSV(print->option))
        handle_recursive(argv, lst_file, print);
    closedir(dir_stream);
    lst_clear(&lst_file);
    return LS_SUCCESS;
}