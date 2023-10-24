#include "../inc/ft_ls.h"

static int get_terminal_width(void)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w))
        return -1;
    return w.ws_col;
}

static void print_padding(int file_length, int colsize)
{
    int padding_size = (colsize + 2) - file_length;
    char padding[padding_size + 1];
    padding[padding_size] = '\0';
    ft_memset(padding, ' ', padding_size);
    write(1, padding, padding_size);
}

void print_default_format(t_file *file_lst, t_print_opt *print)
{
    int terminal_width = get_terminal_width();
    if (terminal_width == -1)
    {
        ft_dprintf(STDERR_FILENO, "Error getting terminal width\n");
        return;
    }
    int total_files = (int)lst_size(file_lst);
    int max_idx = terminal_width / MIN_COLUMN_WIDTH - 1;
    int max_col = max_idx < total_files ? max_idx : total_files;
    // Avoid having call to malloc but limits nb of cols to 256
    column_info column_config[256] = {[0 ... 255] = {true, 0, {0, 0}}};

    t_file *head = file_lst;
    for (int file_idx = 0; file_idx < total_files; file_idx++, head = head->next)
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
    }

    // Searching for maximum valid ncols config
    int ncols = max_col - 1;
    while (ncols >= 0 && !column_config[ncols].valid)
        ncols--;
    if (ncols < 0)
        ncols = 0;
    ncols++;

    int nrows = (total_files + ncols - 1) / ncols;
    for (int row = 0; row < nrows; row++)
    {
        for (int col = 0; col < ncols; col++)
        {
            int file_index = row + col * nrows;
            if (file_index >= total_files)
                break;
            t_file *head = file_lst;
            while (file_index)
            {
                if (head->next)
                    head = head->next;
                file_index--;
            }
            print_file_name(head->metadata, print);
            print_padding(ft_strlen(head->metadata->name), column_config[ncols - 1].col_arr[col]);
        }
        write(1, "\n", 1);
    }
}