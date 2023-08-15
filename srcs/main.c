#include "../inc/ft_ls.h"
#include <stdio.h>

void parse_flags(const char argv[], bool (*flags)[5]) {
    if (argv[0] == '-') {
        for (size_t i = 1 ; i < ft_strlen(argv) ; ++i) {
            switch (argv[i]) {
                case 'l':
                (*flags)[l_flag] = true;
                break;
                case 'r':
                (*flags)[r_flag] = true;
                break;
                case 'R':
                (*flags)[R_flag] = true;
                break;
                case 'a':
                (*flags)[a_flag] = true;
                break;
                case 't':
                (*flags)[t_flag] = true;
                break;
                default:
                // Replace with dprintf on fd 2
                ft_printf("ls: invalid option -- '%c'\n", argv[i]);
                exit (EXIT_FAILURE);
            }
        }
    }
}

void list_dir(const char *arg, const bool flags[5]) {
    if (arg[0] == '-') {
        return ;
    }
    DIR* dir = opendir(arg);
    if (dir == NULL) {
        // Change for fd2
        ft_printf("ls: cannot access '%s': No such file or directory\n");
        return ;
    }
    t_file* lst_file = NULL;
    struct dirent* entity;
    entity = readdir(dir);
    while (entity != NULL) {
        if (!flags[a_flag] && !ft_strncmp(".", entity->d_name, 1)) {
            entity = readdir(dir);
            continue;
        }
        t_file* new = lst_new(entity->d_name);
        if (!new) {
            // Error handling here
            lst_clear(&lst_file);
            return ;
        }
        lst_addback(&lst_file, new);
        // struct stat sb;
        // stat(entity->d_name, &sb);
        entity = readdir(dir);
    }
    lst_print(lst_file);
    lst_sort_ascii(&lst_file);
    lst_print(lst_file);
    lst_clear(&lst_file);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    bool flags[5];

    ft_bzero(flags, 5 * sizeof(bool));
    for (int i = 1 ; i < argc ; ++i) {
        parse_flags(argv[i], &flags);
    }
    for (int i = 1 ; i < argc ; ++i) {
        list_dir(argv[i], flags);
    }
    return (EXIT_SUCCESS);
}