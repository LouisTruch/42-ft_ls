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

    ft_printf("in file:%s\n", arg);

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
            // Error handling here if malloc of new node failed
            lst_clear(&lst_file);
            return ;
        }
        lst_addback(&lst_file, new);
        struct stat sb;
        int status = stat(entity->d_name, &sb);
        (void) status;
        // printf("file=%s %i\n",entity->d_name, sb.st_mode);
        // printf("%s %i %i time=%li\n", new->name, status, sb.st_mode, sb.st_mtime);

        // Handle stat error return -1
        // if (stat(entity->d_name, &sb) == -1) {
        //     perror("stat");
        //     lst_clear(&lst_file);
        //     return ;
        // }

        new->metadata->mode = sb.st_mode;
        new->metadata->last_modif = sb.st_mtime;
        new->metadata->owner = sb.st_uid;
        new->metadata->group = sb.st_gid;
        new->metadata->size = sb.st_size;
        new->metadata->blocks = sb.st_blocks;
        entity = readdir(dir);
    }
    if (flags[t_flag]) {
        lst_sort(&lst_file, chronological);
    } else if (!flags[t_flag]) {
        lst_sort(&lst_file, alphabetical);
    }

    if (!flags[r_flag]) {
        lst_print(lst_file, true, flags[l_flag]);
    } else if (flags[r_flag]) {
        lst_print(lst_file, false, flags[l_flag]);
    }

    if (flags[R_flag]) {}
    // -R call 
    // while (lst_file != NULL) {
    //     if (S_ISDIR(lst_file->type)) {
    //         // ft_printf("isdir\n");
    //         char path[100] = { 0 };
    //         // strcat(path, arg);
    //         // strcat(path, "/");
    //         strcat(path, lst_file->name);
    //         list_dir(path, flags);
    //     }
    //     lst_file = lst_file->next;
    // }
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