#include "../inc/ft_ls.h"

static int get_owner_width(uid_t owner)
{
    struct passwd *pw = getpwuid(owner);
    if (!pw)
        return 1;
    return ft_strlen(pw->pw_name);
}

static int get_group_width(gid_t group)
{
    struct group *gr = getgrgid(group);
    if (!gr)
        return 1;
    return ft_strlen(gr->gr_name);
}

void get_cols_max_width(t_metadata *metadata, t_col_width col_width[NCOLS])
{
    col_width[PERMS] = max(col_width[PERMS], ft_strlen(metadata->str_file_info.perms));
    col_width[NB_LINKS] = max(col_width[NB_LINKS], ft_strlen(metadata->str_file_info.nlink));
    col_width[OWNER] = max(col_width[OWNER], get_owner_width(metadata->owner));
    col_width[GROUP] = max(col_width[GROUP], get_group_width(metadata->group));
    col_width[SIZE] = max(col_width[SIZE], ft_strlen(metadata->str_file_info.size));
    col_width[MONTH] = max(col_width[MONTH], ft_strlen(metadata->str_file_info.month));
    col_width[DAY] = max(col_width[DAY], ft_strlen(metadata->str_file_info.day));
    col_width[HOUR] = max(col_width[HOUR], ft_strlen(metadata->str_file_info.hour));
}

static int add_padding_column(t_print_buff *printer, t_col_width width, t_col_width max_width)
{
    // + 1 is space between each col
    int padding_size = max_width - width + 1;
    // ft_printf("%i-%i %i\n", max_width, width, padding_size);
    char padding[padding_size + 1];
    padding[padding_size] = '\0';
    ft_memset(padding, ' ', padding_size);
    ft_strcat(printer->buff, padding);
    return padding_size;
}

static void fill_buffer_list(char *argv, t_metadata *metadata, t_print_opt *print, t_print_buff *printer)
{
    ft_strcat(printer->buff, metadata->str_file_info.perms);
    // Add padding ACL here
    add_padding_column(printer, ft_strlen(metadata->str_file_info.nlink), print->col_width[NB_LINKS]);
    ft_strcat(printer->buff, metadata->str_file_info.nlink);

    if (!OPT_IS_NO_USERLIST(print->option))
    {
        struct passwd *pw = getpwuid(metadata->owner);
        if (!pw)
        {
            add_padding_column(printer, 1, print->col_width[OWNER]);
            ft_strcat(printer->buff, "?");
        }
        else
        {
            add_padding_column(printer, ft_strlen(pw->pw_name), print->col_width[OWNER]);
            ft_strcat(printer->buff, pw->pw_name);
        }
    }
    if (!OPT_IS_NO_GRPLIST(print->option))
    {
        struct group *gr = getgrgid(metadata->group);
        if (!gr)
        {
            add_padding_column(printer, 1, print->col_width[GROUP]);
            ft_strcat(printer->buff, "?");
        }
        else
        {
            add_padding_column(printer, ft_strlen(gr->gr_name), print->col_width[GROUP]);
            ft_strcat(printer->buff, gr->gr_name);
        }
    }

    add_padding_column(printer, ft_strlen(metadata->str_file_info.size), print->col_width[SIZE]);
    ft_strcat(printer->buff, metadata->str_file_info.size);
    add_padding_column(printer, ft_strlen(metadata->str_file_info.month), print->col_width[MONTH]);
    ft_strcat(printer->buff, metadata->str_file_info.month);
    add_padding_column(printer, ft_strlen(metadata->str_file_info.day), print->col_width[DAY]);
    ft_strcat(printer->buff, metadata->str_file_info.day);
    add_padding_column(printer, ft_strlen(metadata->str_file_info.hour), print->col_width[HOUR]);
    ft_strcat(printer->buff, metadata->str_file_info.hour);
    if (OPT_IS_COLOR(print->option))
        ft_strcat(printer->buff, metadata->str_file_info.color);
    ft_strcat(printer->buff, metadata->name);
    if (OPT_IS_COLOR(print->option))
        ft_strcat(printer->buff, COLOR_RESET);

    if (S_ISLNK(metadata->mode))
    {
        ft_strcat(printer->buff, " -> ");
        char buff[1000] = {0};
        char link_path[1000] = {0};
        ft_strcat(link_path, argv);
        ft_strcat(link_path, "/");
        ft_strcat(link_path, metadata->name);
        if (readlink(link_path, buff, 100) < 0)
        {
        }
        // perror("readlink");
        ft_strcat(printer->buff, buff);
    }
    ft_strcat(printer->buff, "\n");
}

void print_list_format(char *argv, t_file *file_lst, t_print_opt *print)
{
    // max width
    // for (int i = 0; i < NCOLS; i++)
    // {
    // ft_printf("%i\n", print->col_width[i]);
    // }

    t_file *head = file_lst;
    t_print_buff printer = {0, {0, 0}};
    if (!ISNOTDIR(print->option))
        ft_printf("total %i\n", print->total_blcks_alloc / 2);
    for (; head; head = head->next)
    {
        fill_buffer_list(argv, head->metadata, print, &printer);
        write(1, printer.buff, ft_strlen(printer.buff));
        ft_bzero(printer.buff, 4096);
    }
}