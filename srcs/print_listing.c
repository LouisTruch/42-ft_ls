#include "../inc/ft_ls.h"

static int get_nlinks_width(nlink_t nlink)
{
    if (!nlink)
        return 1;
    int nlink_width = 0;
    while (nlink)
    {
        nlink_width++;
        nlink /= 10;
    }
    return nlink_width;
}

static int get_owner_width(uid_t owner)
{
    struct passwd *pw = getpwuid(owner);
    if (!pw)
        return 0;
    return ft_strlen(pw->pw_name);
}

static int get_group_width(gid_t group)
{
    struct group *gr = getgrgid(group);
    if (!gr)
        return 0;
    return ft_strlen(gr->gr_name);
}

static int get_size_width(off_t size)
{
    if (!size)
        return 1;
    int size_width = 0;
    while (size)
    {
        size_width++;
        size /= 10;
    }
    return size_width;
}

static int get_hour_width(time_t last_modif)
{
    time_t time_now = time(NULL);
    if (time_now - last_modif >= _6MONTHS_IN_SECONDS)
        return YEAR_WIDTH;
    return HOUR_WIDTH;
}

static void get_date_widths(time_t last_modif, int *month_width_ptr)
{
    char *date = ctime(&last_modif);
    char *month_str = ft_strchr(date, ' ') + 1;
    char *day_str = ft_strchr(month_str, ' ') + 1;
    // if (!date || !month_str || !day_str)
    // return;

    int idx = 0;
    while (month_str[idx] && month_str[idx] != ' ')
        idx++;
    month_str[idx] = '\0';
    *month_width_ptr = max(*month_width_ptr, ft_strlen(month_str));

    idx = 0;
    while (day_str[idx] && day_str[idx] != ' ')
        idx++;
    day_str[idx] = '\0';
    int *day_width_ptr = month_width_ptr + 1;
    *day_width_ptr = max(*day_width_ptr, ft_strlen(day_str));

    int *hour_width_ptr = month_width_ptr + 2;
    *hour_width_ptr = max(*hour_width_ptr, get_hour_width(last_modif));
}

static int get_col_width_and_blcks(t_file *file_lst, int col_width[NCOLS], blkcnt_t *total_blocks)
{
    while (file_lst)
    {
        col_width[NB_LINKS] = max(col_width[NB_LINKS], get_nlinks_width(file_lst->metadata->nlink));
        col_width[OWNER] = max(col_width[OWNER], get_owner_width(file_lst->metadata->owner));
        col_width[GROUP] = max(col_width[GROUP], get_group_width(file_lst->metadata->group));
        col_width[SIZE] = max(col_width[SIZE], get_size_width(file_lst->metadata->size));
        *total_blocks += file_lst->metadata->blocks;
        get_date_widths(file_lst->metadata->last_modif, &col_width[MONTH]);
        file_lst = file_lst->next;
    }
    int total_col_width = NCOLS; //  nb of cols
    for (int i = 0; i < NCOLS; ++i)
        total_col_width += col_width[i];
    return total_col_width;
}

static void construct_perms(mode_t mode, char *line)
{
    if (S_ISREG(mode))
        line[0] = '-';
    else if (S_ISDIR(mode))
        line[0] = 'd';
    else if (S_ISCHR(mode))
        line[0] = 'c';
    else if (S_ISBLK(mode))
        line[0] = 'b';
    else if (S_ISFIFO(mode))
        line[0] = 'p';
    else if (S_ISLNK(mode))
        line[0] = 'l';
    else if (S_ISSOCK(mode))
        line[0] = 's';

    line[1] = S_IRUSR & mode ? 'r' : '-';
    line[2] = S_IWUSR & mode ? 'w' : '-';
    line[3] = S_IXUSR & mode ? 'x' : '-';
    line[4] = S_IRGRP & mode ? 'r' : '-';
    line[5] = S_IWGRP & mode ? 'w' : '-';
    line[6] = S_IXGRP & mode ? 'x' : '-';
    line[7] = S_IROTH & mode ? 'r' : '-';
    line[8] = S_IWOTH & mode ? 'w' : '-';
    line[9] = S_IXOTH & mode ? 'x' : '-';
    line[10] = ' ';
}

static void fill_line_padding(char *str, char *line, int width, int max_width)
{
    int i = ft_strlen(line);
    while (width != max_width)
    {
        line[i] = ' ';
        i++;
        width++;
    }
    int j = 0;
    while (str[j])
    {
        line[i + j] = str[j];
        j++;
    }
    line[i + j] = ' ';
}

static void ultoa_no_alloc(char str[20], unsigned long nb)
{
    int i = 0;
    int sign = 1;

    if (nb == 0)
        str[0] = '0';

    if (nb < 0)
    {
        nb = -nb;
        sign = -1;
    }
    for (; nb; i++, nb /= 10)
        str[i] = nb % 10 + '0';
    if (sign < 0)
        str[i] = '-';
    reverse_str(str);
}

static void construct_nlink(nlink_t nlink, char *line, int max_width)
{
    char str[20] = {0};
    ultoa_no_alloc(str, nlink);
    fill_line_padding(str, line, ft_strlen(str), max_width);
}

static void construct_owner(uid_t owner, char *line, int max_width)
{
    struct passwd *pw = getpwuid(owner);
    if (!pw)
        return;
    fill_line_padding(pw->pw_name, line, ft_strlen(pw->pw_name), max_width);
}

static void construct_group(gid_t group, char *line, int max_width)
{
    struct group *gr = getgrgid(group);
    if (!gr)
        return;
    fill_line_padding(gr->gr_name, line, ft_strlen(gr->gr_name), max_width);
}

static void construct_size(off_t size, char *line, int max_width)
{
    char str[20] = {0};
    ultoa_no_alloc(str, size);
    fill_line_padding(str, line, ft_strlen(str), max_width);
}

static void construct_date(time_t last_modif, char *line, int *month_width_ptr)
{
    char *date = ctime(&last_modif);
    char *month_str = ft_strchr(date, ' ') + 1;
    char *day_str = ft_strchr(month_str, ' ') + 1;
    if (day_str[0] == ' ')
        day_str++;
    char *hour_str = ft_strchr(day_str, ' ') + 1;

    int idx = 0;
    while (month_str[idx] && month_str[idx] != ' ')
        idx++;
    month_str[idx] = '\0';
    fill_line_padding(month_str, line, ft_strlen(month_str), *month_width_ptr);

    idx = 0;
    while (day_str[idx] && day_str[idx] != ' ')
        idx++;
    day_str[idx] = '\0';
    time_t time_now = time(NULL);
    if (time_now - last_modif >= _6MONTHS_IN_SECONDS)
    {
        hour_str = ft_strchr(hour_str, ' ') + 1;
        hour_str[4] = '\0';
    }
    else
        hour_str[5] = '\0';
    fill_line_padding(day_str, line, ft_strlen(day_str), *(month_width_ptr + 1));
    fill_line_padding(hour_str, line, ft_strlen(hour_str), *(month_width_ptr + 2));
}

static void construct_line(t_metadata *metadata, char *line, int col_width[NCOLS])
{
    construct_perms(metadata->mode, line);
    construct_nlink(metadata->nlink, line, col_width[NB_LINKS]);
    construct_owner(metadata->owner, line, col_width[OWNER]);
    construct_group(metadata->group, line, col_width[GROUP]);
    construct_size(metadata->size, line, col_width[SIZE]);
    construct_date(metadata->last_modif, line, &col_width[MONTH]);
}

void print_list(char *argv, t_file *file_lst)
{
    int col_width[NCOLS] = {1};

    col_width[PERMS] = 10;
    col_width[MONTH] = 3;
    col_width[DAY] = 1;
    col_width[HOUR] = 4;

    blkcnt_t total_blocks = 0;
    int total_col_width = get_col_width_and_blcks(file_lst, col_width, &total_blocks) + 1;
    // size_t nb_lines = lst_size(file_lst);
    char line[total_col_width];
    printf("total %zu\n", total_blocks / 2);
    for (t_file *head = file_lst; head; head = head->next)
    {
        bzero(line, total_col_width);
        construct_line(head->metadata, line, col_width);
        ft_printf("%s%s", line, head->metadata->name);
        if (S_ISLNK(head->metadata->mode))
        {
            char buff[1000] = {0};
            char link_path[1000] = {0};
            strcat(link_path, argv);
            strcat(link_path, "/");
            strcat(link_path, head->metadata->name);
            errno = 0;
            if (readlink(link_path, buff, 100) < 0)
                perror("readlink");
            ft_printf(" -> %s", buff);
        }
        ft_printf("\n");
    }
}