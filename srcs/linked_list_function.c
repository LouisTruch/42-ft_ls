#include "../inc/ft_ls.h"

static void construct_perms(char *perms, mode_t mode)
{
    if (S_ISREG(mode))
        perms[0] = '-';
    else if (S_ISDIR(mode))
        perms[0] = 'd';
    else if (S_ISCHR(mode))
        perms[0] = 'c';
    else if (S_ISBLK(mode))
        perms[0] = 'b';
    else if (S_ISFIFO(mode))
        perms[0] = 'p';
    else if (S_ISLNK(mode))
        perms[0] = 'l';
    else if (S_ISSOCK(mode))
        perms[0] = 's';

    perms[1] = S_IRUSR & mode ? 'r' : '-';
    perms[2] = S_IWUSR & mode ? 'w' : '-';
    perms[3] = S_IXUSR & mode ? 'x' : '-';
    perms[4] = S_IRGRP & mode ? 'r' : '-';
    perms[5] = S_IWGRP & mode ? 'w' : '-';
    perms[6] = S_IXGRP & mode ? 'x' : '-';
    perms[7] = S_IROTH & mode ? 'r' : '-';
    perms[8] = S_IWOTH & mode ? 'w' : '-';
    perms[9] = S_IXOTH & mode ? 'x' : '-';
    // Add here for ACL
    perms[10] = '\0';
}

static void construct_date(time_t last, t_str_file_info *str_file_info)
{
    char *date = ctime(&last);
    if (!date)
    {
        strcpy(str_file_info->month, "?");
        strcpy(str_file_info->day, "?");
        strcpy(str_file_info->hour, "?");
        return;
    }
    while (*date && *date != ' ')
        date++;
    date++;
    int i;
    for (i = 0; date[i] && date[i] != ' ' && i < MAX_WIDTH_DATE; i++)
        str_file_info->month[i] = date[i];
    str_file_info->month[i] = '\0';
    while (*date && *date != ' ')
        date++;
    while (*date && *date == ' ')
        date++;

    for (i = 0; date[i] && date[i] != ' ' && i < MAX_WIDTH_DATE; i++)
        str_file_info->day[i] = date[i];
    str_file_info->day[i] = '\0';
    while (*date && *date != ' ')
        date++;
    date++;

    time_t time_now = time(NULL);
    if (time_now - last < _6MONTHS_IN_SECONDS || time_now == (time_t)-1)
    {
        int colon_cnt = 0;
        for (i = 0; date[i] && i < MAX_WIDTH_DATE; i++)
        {
            if (date[i] == ':')
            {
                colon_cnt++;
                if (colon_cnt == 2)
                    break;
            }
            str_file_info->hour[i] = date[i];
        }
    }
    else
    {
        while (*date && *date != ' ')
            date++;
        date++;
        for (i = 0; date[i] && date[i] != '\n' && i < MAX_WIDTH_DATE; i++)
            str_file_info->hour[i] = date[i];
    }
    // Space between hour and filename
    str_file_info->hour[i] = ' ';
    str_file_info->hour[i + 1] = '\0';
}

t_file *lst_new(const char *file_name, const struct stat *sb, t_print_opt *print)
{
    if (!file_name || !sb || !print)
        return NULL;

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
    new->metadata->name = ft_strdup(file_name);
    if (!new->metadata->name)
    {
        free(new->metadata);
        free(new);
        return NULL;
    }
    new->metadata->mode = sb->st_mode;
    new->metadata->nlink = sb->st_nlink;
    new->metadata->last_modif = sb->st_mtime;
    new->metadata->last_access = sb->st_atim.tv_sec;
    new->metadata->owner = sb->st_uid;
    new->metadata->group = sb->st_gid;
    new->metadata->size = sb->st_size;
    new->metadata->blocks = sb->st_blocks;
    if (OPT_IS_LIST(print->option))
    {
        construct_perms(new->metadata->str_file_info.perms, new->metadata->mode);
        if (print->time_to_print == PRINT_LAST_MODIF)
            construct_date(new->metadata->last_modif, &new->metadata->str_file_info);
        else if (print->time_to_print == PRINT_LAST_ACCESS)
            construct_date(new->metadata->last_access, &new->metadata->str_file_info);
        lltoa_no_alloc(new->metadata->str_file_info.size, new->metadata->size);
        lltoa_no_alloc(new->metadata->str_file_info.nlink, new->metadata->nlink);
    }
    if (OPT_IS_COLOR(print->option))
        get_color(new->metadata->str_file_info.color, new->metadata->mode);

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