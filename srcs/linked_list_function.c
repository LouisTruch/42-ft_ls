#include "../inc/ft_ls.h"

t_file *lst_new(const char *file_name, struct stat sb)
{
    t_file *new;

    new = malloc(sizeof(t_file));
    if (!new)
        return NULL;
    new->metadata = malloc(sizeof(t_metadata));
    if (!new->metadata)
    {
        free(new);
        return NULL;
    }
    new->next = NULL;
    new->prev = NULL;
    if (!file_name)
    {
        new->metadata->name = NULL;
        return new;
    }
    new->metadata->name = ft_strdup(file_name);
    if (!new->metadata->name)
    {
        free(new);
        free(new->metadata);
        return NULL;
    }
    new->metadata->mode = sb.st_mode;
    new->metadata->nlink = sb.st_nlink;
    new->metadata->last_modif = sb.st_mtime;
    new->metadata->owner = sb.st_uid;
    new->metadata->group = sb.st_gid;
    new->metadata->size = sb.st_size;
    new->metadata->blocks = sb.st_blocks;
    return new;
}

t_file *lst_last(t_file *lst)
{
    if (lst == NULL)
        return NULL;
    t_file *last;
    while (lst != NULL)
    {
        last = lst;
        lst = lst->next;
    }
    return last;
}

void lst_addback(t_file **lst, t_file *updated_last)
{
    if (!lst || !updated_last)
    {
        return;
    }
    if (*lst == NULL)
    {
        *lst = updated_last;
        return;
    }
    t_file *current_last = lst_last(*lst);
    current_last->next = updated_last;
    updated_last->prev = current_last;
    return;
}

void lst_clear(t_file **lst)
{
    t_file *tmp;
    t_file *current;

    if (!*lst)
        return;
    current = *lst;
    while (current != NULL)
    {
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

size_t lst_size(t_file *lst)
{
    if (lst == NULL)
        return 0;
    size_t i = 0;
    while (lst != NULL)
    {
        lst = lst->next;
        i++;
    }
    return i;
}