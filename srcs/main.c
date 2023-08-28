#include "../inc/ft_ls.h"
#include <stdio.h>

void parse_options(const char argv[], bool (*options)[5], bool *no_dir) {
    if (argv[0] == '-') {
        for (size_t i = 1 ; i < ft_strlen(argv) ; ++i) {
            switch (argv[i]) {
                case 'l':
                (*options)[l_option] = true;
                break;
                case 'r':
                (*options)[r_option] = true;
                break;
                case 'R':
                (*options)[R_option] = true;
                break;
                case 'a':
                (*options)[a_option] = true;
                break;
                case 't':
                (*options)[t_option] = true;
                break;
                default:
                // Replace with dprintf on fd 2
                ft_printf("ls: invalid option -- '%c'\n", argv[i]);
                exit (EXIT_FAILURE);
            }
        }
    } else {
        *no_dir = false;
    }
}

void my_ls(const char *arg, const bool options[5]);
void recursive_ls(const char* arg, t_file* lst, const bool options[5]) {
    while (lst != NULL) {
        if (S_ISDIR(lst->metadata->mode)) {
            // Change this
            char path[1024] = { 0 };
            strcat(path, arg);
            strcat(path, "/");
            strcat(path, lst->metadata->name);
            my_ls(path, options);
        }
        lst = lst->next;
    }
}

void my_ls(const char *arg, const bool options[5]) {
    if (arg[0] == '-') {
        return ;
    }

    DIR* dir = opendir(arg);
    if (dir == NULL) {
        // Change for fd2
        ft_printf("ls: cannot access '%s': No such file or directory\n");
        return;
    }

    ft_printf("%s:\n", arg);

    t_file* lst_file = NULL;
    struct dirent* entity;
    entity = readdir(dir);
    while (entity != NULL) {
        if (!options[a_option] && !ft_strncmp(".", entity->d_name, 1)) {
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
        char buffer[1024] = { 0 };
        strcat(buffer, arg);
        strcat(buffer, "/");
        strcat(buffer, entity->d_name);
        int status = stat(buffer, &sb);
        if (status == -1) {
            ft_printf("status error\n");
        }
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

    // Make this this function pointer ?
    if (options[t_option]) {
        lst_sort(&lst_file, chronological);
    } else if (!options[t_option]) {
        lst_sort(&lst_file, alphabetical);
    }

    lst_print(lst_file, options[r_option], options[l_option]);

    if (options[R_option]) {
        recursive_ls(arg, lst_file, options);
    }

    lst_clear(&lst_file);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    bool options[5] = { 0 };
    bool no_dir = true;

    for (int i = 1 ; i < argc ; i++) {
        parse_options(argv[i], &options, &no_dir);
    }
    if (no_dir) {
        my_ls(".", options);
        return EXIT_SUCCESS;
    }
    for (int i = 1 ; i < argc ; i++) {
        my_ls(argv[i], options);
    }
    return EXIT_SUCCESS;
}