#include "../inc/ft_ls.h"

static void construct_perms(char *perms, mode_t mode, const char *filename, t_print_opt *print)
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
    perms[10] = '\0';
    if (!OPT_IS_SHOW_ATTR_ACL(print->option))
    {
        return;
    }
    char list[XATTR_SIZE];
    acl_t acl;
    if ((acl = acl_get_file(filename, ACL_TYPE_ACCESS)) != NULL && errno != ENODATA)
    {
        perms[10] = '+';
        perms[11] = '\0';
        acl_free(acl);
    }
    else if (listxattr(filename, list, XATTR_SIZE) > 0)
    {
        perms[10] = '@';
        perms[11] = '\0';
        return;
    }
}

static void construct_date(time_t last, t_str_file_info *str_file_info)
{
    char *date = ctime(&last);
    if (!date)
    {
        ft_strcpy(str_file_info->month, "?");
        ft_strcpy(str_file_info->day, "?");
        ft_strcpy(str_file_info->hour, "?");
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

static void fill_metadata_error(t_metadata *metadata, t_print_opt *print)
{
    if (OPT_IS_LIST(print->option))
    {
        ft_strcpy(metadata->str_file_info.perms, "??????????");
        ft_strcpy(metadata->str_file_info.nlink, "?");
        ft_strcpy(metadata->str_file_info.size, "?");
        ft_strcpy(metadata->str_file_info.month, " ");
        ft_strcpy(metadata->str_file_info.day, " ");
        ft_strcpy(metadata->str_file_info.hour, "? ");
    }
    if (OPT_IS_COLOR(print->option))
        ft_strcpy(metadata->str_file_info.color, COLOR_BROKEN);
}

static void fill_metadata_stats(t_metadata *metadata, const struct stat *sb, t_print_opt *print)
{
    metadata->mode = sb->st_mode;
    metadata->nlink = sb->st_nlink;
    metadata->last_modif = sb->st_mtime;
    metadata->last_access = sb->st_atim.tv_sec;
    metadata->owner = sb->st_uid;
    metadata->group = sb->st_gid;
    metadata->size = sb->st_size;
    metadata->blocks = sb->st_blocks;
    if (OPT_IS_LIST(print->option))
    {
        construct_perms(metadata->str_file_info.perms, metadata->mode, metadata->name, print);
        if (print->time_to_print == PRINT_LAST_MODIF)
            construct_date(metadata->last_modif, &metadata->str_file_info);
        else if (print->time_to_print == PRINT_LAST_ACCESS)
            construct_date(metadata->last_access, &metadata->str_file_info);
        if (!metadata->size)
            ft_strcpy(metadata->str_file_info.size, "0");
        else
            lltoa_no_alloc(metadata->str_file_info.size, metadata->size);
        lltoa_no_alloc(metadata->str_file_info.nlink, metadata->nlink);
    }
    if (OPT_IS_COLOR(print->option))
        get_color(metadata->str_file_info.color, metadata->mode);
}

t_file *lst_new(const char *file_name, const struct stat *sb, t_print_opt *print)
{
    if (!file_name || !print)
        return NULL;

    t_file *new = malloc(sizeof(t_file));
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
    if (sb)
        fill_metadata_stats(new->metadata, sb, print);
    else
        fill_metadata_error(new->metadata, print);
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