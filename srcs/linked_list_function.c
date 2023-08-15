#include "../inc/ft_ls.h"

t_file* lst_new(const char *file_name) {
    t_file* new;

    new = malloc(sizeof(t_file));
    if (!new)
        return NULL;
    new->next = NULL;
    new->prev = NULL;
    if (!file_name) {
        new->name = NULL;
        return new;
    }
    new->name = ft_strdup(file_name);
    if (!new->name) {
        free(new);
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
        free(tmp->name);
        tmp->name = NULL;
        free(tmp);
        tmp = NULL;
    }
    *lst = NULL;
}

void lst_print(t_file* lst) {
    if (lst == NULL)
        return;
    while (lst != NULL) {
        ft_printf("lst: file_name=%s\n", lst->name);
        lst = lst->next;
    }
}

void lst_swap(t_file* node1, t_file* node2) {
    t_file* tmp;
    tmp = node1;
    node1 = node2;
    node2 = tmp;
}

void lst_sort_ascii(t_file** head_ref) {
    if (*head_ref == NULL || (*head_ref)->next == NULL)
        return;
    t_file* curr = *head_ref;
    t_file* prev_tmp;
    // t_file *tmp2;
    while (curr != NULL) {
        if (curr->next && ft_strcmp(curr->name, curr->next->name) > 0) {
            t_file* tmp = curr->next;
            curr->next = curr;
            curr = tmp;
            ft_printf("swap\n");
        }
        prev_tmp = curr;
        curr = curr->next;
    }
}