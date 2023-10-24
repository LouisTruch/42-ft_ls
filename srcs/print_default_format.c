#include "../inc/ft_ls.h"

static int get_terminal_width(void)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w))
        return -1;
    return w.ws_col;
}

static void fill_buffer_padding(int file_length, int colsize, t_printer *printer)
{
    int padding_size = (colsize + 2) - file_length;
    char padding[padding_size + 1];
    padding[padding_size] = '\0';
    ft_memset(padding, ' ', padding_size);
    ft_strcatindex(printer->buff, padding, &printer->i);
}

static int get_optimal_nb_cols(t_file *file_lst, int total_files, int terminal_width, column_info column_config[256])
{
    int max_idx = terminal_width / MIN_COLUMN_WIDTH - 1;
    int max_col = max_idx < total_files ? max_idx : total_files;
    // Avoid having call to malloc but limits nb of cols to 256

    t_file *head = file_lst;
    for (int file_idx = 0; file_idx < total_files; file_idx++, head = head->next)
    {
        // Config_idx is number of columns
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
    return ncols;
}

static void fill_buffer_filename(t_metadata *metadata, t_printer *printer, t_print_opt *print)
{
    if (OPT_IS_COLOR(print->option))
    {
        ft_strcatindex(printer->buff, metadata->str_file_info.color, &printer->i);
        ft_strcatindex(printer->buff, metadata->name, &printer->i);
        ft_strcatindex(printer->buff, COLOR_RESET, &printer->i);
    }
    else
        ft_strcatindex(printer->buff, metadata->name, &printer->i);
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
    column_info column_config[256] = {[0 ... 255] = {true, 0, {0, 0}}};
    int ncols = get_optimal_nb_cols(file_lst, total_files, terminal_width, column_config);
    int nrows = (total_files + ncols - 1) / ncols;

    t_printer printer = {0, {0, 0}};
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
            fill_buffer_filename(head->metadata, &printer, print);
            fill_buffer_padding(ft_strlen(head->metadata->name), column_config[ncols - 1].col_arr[col], &printer);
            write(1, printer.buff, printer.i);
            ft_bzero(&printer, sizeof(printer));
        }
        write(1, "\n", 1);
    }
}