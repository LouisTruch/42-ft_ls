#include "../inc/ft_ls.h"

static int get_terminal_width(void)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w))
        return -1;
    return w.ws_col;
}

static void print_file_padding(char *file_name, int colsize)
{
    // printf("filename:%s colsize%i\n", file_name, colsize);
    ft_printf("%s  ", file_name);
    int file_length = ft_strlen(file_name);
    // char padding[colsize - file_length];
    // ft_memset(padding, ' ', colsize - file_length);
    while (file_length != colsize)
    {
        write(1, " ", 1);
        file_length++;
    }
}

void print_default(t_file *file_lst)
{
    int terminal_width = get_terminal_width();
    if (terminal_width == -1)
    {
        ft_dprintf(2, "Error getting terminal width\n");
        return;
    }
    int total_files = (int)lst_size(file_lst);
    int max_idx = terminal_width / MIN_COLUMN_WIDTH - 1;
    int max_col = max_idx < total_files ? max_idx : total_files;
    // Avoid having call to malloc but limits nb of cols to 256
    column_info column_config[256] = {{true, 0, {0, 0}}};

    t_file *head = file_lst;
    for (int file_idx = 0; file_idx < total_files; file_idx++)
    {
        // Config_idx is also number of columns
        for (int config_idx = 0; config_idx < max_col || config_idx > 255; config_idx++)
        {
            if (!column_config[config_idx].valid)
                continue;

            int rows = (total_files + config_idx) / (config_idx + 1);
            if ((rows * (config_idx + 1) - total_files) > rows)
            {
                column_config[config_idx].valid = false;
                continue;
            }

            int file_col = file_idx / rows;

            int name_width = (int)ft_strlen(head->metadata->name);

            if (name_width > column_config[config_idx].col_arr[file_col])
            {
                column_config[config_idx].line_len += name_width - column_config[config_idx].col_arr[file_col];
                column_config[config_idx].col_arr[file_col] = name_width;
            }
            if (column_config[config_idx].line_len + (2 * config_idx) > terminal_width)
                column_config[config_idx].valid = false;
        }
        head = head->next;
    }

    // Searching for maximum valid ncols config
    int ncols = max_col - 1;
    while (ncols >= 0 && !column_config[ncols].valid)
        ncols--;
    ncols++;

    int nrows = (total_files + ncols - 1) / ncols;
    for (int row = 0; row < nrows; row++)
    {
        for (int col = 0; col < ncols; col++)
        {
            int file_index = row + col * nrows;
            if (file_index >= total_files)
                break;
            // ft_printf("f index:%i\n", file_index);
            t_file *head = file_lst;
            while (file_index)
            {
                if (head->next)
                    head = head->next;
                file_index--;
            }
            print_file_padding(head->metadata->name, column_config[ncols - 1].col_arr[col]);
        }
        ft_printf("\n");
    }
}

static int get_perm_width(t_file *file_lst)
{
    return 10;
}

static int get_nlinks_width(nlink_t nlink)
{
    if (!nlink)
        return 0;
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
        return 0;
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

static int get_col_width(t_file *file_lst, int col_width[NCOLS])
{
    // Perm width to do for ACL
    // ft_printf("d: %s\n", test);
    while (file_lst)
    {
        // col_width[PERMS]
        col_width[NB_LINKS] = max(col_width[NB_LINKS], get_nlinks_width(file_lst->metadata->nlink));
        col_width[OWNER] = max(col_width[OWNER], get_owner_width(file_lst->metadata->owner));
        col_width[GROUP] = max(col_width[GROUP], get_group_width(file_lst->metadata->group));
        col_width[SIZE] = max(col_width[SIZE], get_size_width(file_lst->metadata->size));
        get_date_widths(file_lst->metadata->last_modif, &col_width[MONTH]);
        file_lst = file_lst->next;
    }
    int total_col_width = NCOLS; // nb of cols
    for (int i = 0; i < NCOLS; ++i)
        total_col_width += col_width[i];
    // Need to add space for between each cols
    ft_printf("t_width:%i\n", total_col_width);
    return total_col_width;
}

void print_list(t_file *file_lst)
{
    int col_width[NCOLS] = {0};
    // Change this below
    col_width[PERMS] = 10;

    // Init some min values in case of ctime() fail
    // col_width[MONTH] = 3;
    // col_width[DAY] = 1;
    // col_width[HOUR] = 4;

    int total_col_width = get_col_width(file_lst, col_width);
    size_t nb_lines = lst_size(file_lst);
    int line_length[nb_lines + 1]; // one more line for "total xxx"
    int print_length = 0;          // Need to add size of "total xxx\n"
    for (size_t idx = 1; idx < nb_lines; idx++)
    {
        line_length[idx] = total_col_width + ft_strlen(file_lst->metadata->name) + 1;
        // Need to add more if sym link
        ft_printf("lineLength:%i\n", line_length[idx]);
        print_length += line_length[idx];
    }
    char to_print[print_length + 100];
    ft_printf("total_length:%i\n", print_length);
}