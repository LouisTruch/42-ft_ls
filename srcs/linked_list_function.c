#include "../inc/ft_ls.h"

t_file* lst_new(const char *file_name) {
    t_file* new;

    new = malloc(sizeof(t_file));
    if (!new)
        return NULL;
    new->metadata = malloc(sizeof(t_metadata));
    if (!new->metadata) {
        free (new);
        return NULL;
    }
    new->next = NULL;
    new->prev = NULL;
    if (!file_name) {
        new->metadata->name = NULL;
        return new;
    }
    // new->name = ft_strdup(file_name);
    // if (!new->name) {
    //     free(new);
    //     return NULL;
    // }
    new->metadata->name = ft_strdup(file_name);
    if (!new->metadata->name) {
        free(new);
        free(new->metadata);
        return NULL;
    }
    return new;
}

t_file* lst_last(t_file* lst) {
    if (lst == NULL)
        return NULL;
    t_file* last;
    while (lst != NULL) {
        last = lst;
        lst = lst->next;
    }
    return last;
}

void lst_addback(t_file** lst, t_file* updated_last) {
    if (!lst || !updated_last) {
        return ;
    }
    if (*lst == NULL) {
        *lst = updated_last;
        return ;
    }
    t_file* current_last = lst_last(*lst);
    current_last->next = updated_last;
    updated_last->prev = current_last;
    return ;
}

void lst_clear(t_file** lst) {
    t_file* tmp;
    t_file* current;

    if (!*lst)
        return ;
    current = *lst;
    while (current != NULL) {
        tmp = current;
        current = current->next;
        free(tmp->metadata->name);
        tmp->metadata->name = NULL;
        free(tmp->metadata);
        tmp->metadata = NULL;
        free(tmp);
        tmp = NULL;
    }
    *lst = NULL;
}

size_t lst_size(t_file *lst) {
    if (lst == NULL)
        return 0;
    size_t i = 0;
    while (lst != NULL) {
        lst = lst->next;
        i++;
    }
    return i;
}

char* get_actual_date(const char* ctime) {
    char *date;

    while (*ctime) {
        if (*ctime == ' ') {
            ctime++;
            date = ft_strdup(ctime);
            int commas = 0;
            for (size_t i = 0 ; date[i] ; i++) {
                if (date[i] == ':') {
                    commas++;
                    if (commas == 2) {
                        date[i] = '\0';
                        return date;
                    }
                }
            }
            break;
        }
        ctime++;
    }
    return NULL;
}

static int file_size(off_t fsize) {

    if (fsize == 0)
        return 1;
    int size = 1;
    while (fsize > 9) {
        fsize /= 10;
        size++;
    }
    return size;
}

int biggest_size_in_lst(t_file* lst) {
    if (lst == NULL)
        return 0;
    int max_size = 0;
    while (lst != NULL) {
        int current_file_size = file_size(lst->metadata->size);
        if (current_file_size > max_size)
            max_size = current_file_size;
        lst = lst->next;
    }
    return max_size;
}

char* construct_item(t_file* file, int columns) {
    char* item = ft_strdup("---------- 0 ");
    // item[11] is number of hard links to the file, need to change it
    if (S_ISDIR(file->metadata->mode)) {
        item[0] = 'd';
        item[11] = '2';
    } else {
        item[11] = '1';
    }
    if ((file->metadata->mode & S_IRUSR) != 0)
        item[1] = 'r';
    if ((file->metadata->mode & S_IWUSR) != 0)
        item[2] = 'w';
    if ((file->metadata->mode & S_IXUSR) != 0)
        item[3] = 'x';

    if ((file->metadata->mode & S_IRGRP) != 0)
        item[4] = 'r';
    if ((file->metadata->mode & S_IWGRP) != 0)
        item[5] = 'w';
    if ((file->metadata->mode & S_IXGRP) != 0)
        item[6] = 'x';

    if ((file->metadata->mode & S_IROTH) != 0)
        item[7] = 'r';
    if ((file->metadata->mode & S_IWOTH) != 0)
        item[8] = 'w';
    if ((file->metadata->mode & S_IXOTH) != 0)
        item[9] = 'x';


    //Ugly as fk need to refactor all this
    struct passwd *pw = getpwuid(file->metadata->owner);
    char* tmp = ft_strjoin(item, pw->pw_name);
    free(item);
    item = ft_strjoin(tmp, " ");
    free(tmp);
    struct group *gr = getgrgid(file->metadata->group);
    tmp = ft_strjoin(item, gr->gr_name);
    free(item);
    int nb_spaces = columns - file_size(file->metadata->size) + 1;
    char spaces[nb_spaces + 1];
    spaces[nb_spaces] = '\0';
    ft_memset(spaces, ' ', nb_spaces);
    item = ft_strjoin(tmp, spaces);
    free(tmp);
    // Need to check if itoa is enough cause of int restriction ?
    char* size = ft_itoa(file->metadata->size);
    tmp = ft_strjoin(item, size);
    free(size);
    free(item);
    item = ft_strjoin(tmp, " ");
    free(tmp);
    char* date = get_actual_date(ctime(&file->metadata->last_modif));
    tmp = ft_strjoin(item, date);
    free(date);
    free(item);
    item = ft_strjoin(tmp, " ");
    free(tmp);
    tmp = ft_strjoin(item, file->metadata->name);
    free(item);
    return tmp;
}

int get_total_nb_blocks(t_file* lst) {
    if (lst == NULL)
        return 0;
    int total_blocks = 0;
    while (lst != NULL) {
        total_blocks += lst->metadata->blocks;
        lst = lst->next;
    }
    return total_blocks;
}

void lst_print(t_file* lst, bool ascending, bool l_flag) {
    if (lst == NULL)
        return;

    ft_printf("total %i\n", get_total_nb_blocks(lst) / 2);
    int columns = biggest_size_in_lst(lst);
    if (ascending) {
        while (lst != NULL) {
            if (l_flag) {
                char *item = construct_item(lst, columns);
                ft_printf("%s\n", item);
                free(item);
            } else {
                ft_printf("lst: file_name=%s\n", lst->metadata->name);
            }
            lst = lst->next;
        }
    } else {
        t_file* last = lst_last(lst);
        while (last != NULL) {
            if (l_flag) {
                char *item = construct_item(lst, columns);
                ft_printf("%s\n", item);
                free(item);
            } else {
                ft_printf("lst: file_name=%s\n", last->metadata->name);
            }
            last = last->prev;
        }
    }
}

void lst_sort(t_file** head_ref, int sort_by) {
    if (*head_ref == NULL || (*head_ref)->next == NULL)
        return;

    size_t list_size = lst_size(*head_ref);
    for (size_t i = 0 ; i < list_size ; i++) {
        t_file* lst = *head_ref;
        for (size_t j = 0 ; j < list_size - 1 ; j++) {
            switch (sort_by) {
                case alphabetical:
                    if (ft_strcasecmp(lst->metadata->name, lst->next->metadata->name) > 0) {
                        t_metadata* tmp = lst->next->metadata;
                        lst->next->metadata = lst->metadata;
                        lst->metadata = tmp;
                    }
                    break;

                case chronological:
                    if (lst->metadata->last_modif < lst->next->metadata->last_modif) {
                        t_metadata* tmp = lst->next->metadata;
                        lst->next->metadata = lst->metadata;
                        lst->metadata = tmp;
                    }
                    break;
            }
            lst = lst->next;
        }
    }
}