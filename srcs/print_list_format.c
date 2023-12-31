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

static void add_padding_column(t_printer *printer, t_col_width width, t_col_width max_width)
{
    // + 1 is space between each col
    int padding_size = max_width - width + 1;
    char padding[padding_size + 1];
    padding[padding_size] = '\0';
    ft_memset(padding, ' ', padding_size);
    ft_strcatindex(printer->buff, padding, &printer->i);
}

static void fill_buffer_list(char *argv, t_metadata *metadata, t_print_opt *print, t_printer *printer)
{
    ft_strcatindex(printer->buff, metadata->str_file_info.perms, &printer->i);
    add_padding_column(printer, ft_strlen(metadata->str_file_info.perms), print->col_width[PERMS] - 1);
    add_padding_column(printer, ft_strlen(metadata->str_file_info.nlink), print->col_width[NB_LINKS]);
    ft_strcatindex(printer->buff, metadata->str_file_info.nlink, &printer->i);

    if (!OPT_IS_NO_USERLIST(print->option))
    {
        if (metadata->str_file_info.size[0] == '?')
        {
            add_padding_column(printer, 1, print->col_width[OWNER]);
            ft_strcatindex(printer->buff, "?", &printer->i);
        }
        else
        {
            struct passwd *pw = getpwuid(metadata->owner);
            add_padding_column(printer, ft_strlen(pw->pw_name), print->col_width[OWNER]);
            ft_strcatindex(printer->buff, pw->pw_name, &printer->i);
        }
    }
    if (!OPT_IS_NO_GRPLIST(print->option))
    {
        if (metadata->str_file_info.size[0] == '?')
        {
            add_padding_column(printer, 1, print->col_width[GROUP]);
            ft_strcatindex(printer->buff, "?", &printer->i);
        }
        else
        {
            struct group *gr = getgrgid(metadata->group);
            add_padding_column(printer, ft_strlen(gr->gr_name), print->col_width[GROUP]);
            ft_strcatindex(printer->buff, gr->gr_name, &printer->i);
        }
    }

    add_padding_column(printer, ft_strlen(metadata->str_file_info.size), print->col_width[SIZE]);
    ft_strcatindex(printer->buff, metadata->str_file_info.size, &printer->i);
    add_padding_column(printer, ft_strlen(metadata->str_file_info.month), print->col_width[MONTH]);
    ft_strcatindex(printer->buff, metadata->str_file_info.month, &printer->i);
    add_padding_column(printer, ft_strlen(metadata->str_file_info.day), print->col_width[DAY]);
    ft_strcatindex(printer->buff, metadata->str_file_info.day, &printer->i);
    add_padding_column(printer, ft_strlen(metadata->str_file_info.hour), print->col_width[HOUR]);
    ft_strcatindex(printer->buff, metadata->str_file_info.hour, &printer->i);
    if (OPT_IS_COLOR(print->option))
    {
        ft_strcatindex(printer->buff, metadata->str_file_info.color, &printer->i);
        ft_strcatindex(printer->buff, metadata->name, &printer->i);
        ft_strcatindex(printer->buff, COLOR_RESET, &printer->i);
    }
    else
        ft_strcatindex(printer->buff, metadata->name, &printer->i);

    // Look at link mode
    if (metadata->str_file_info.size[0] != '?' && S_ISLNK(metadata->mode))
    {
        ft_strcatindex(printer->buff, " -> ", &printer->i);
        char link[1000] = {0};
        char link_path[1000] = {0};
        ft_strcat(link_path, argv);
        ft_strcat(link_path, "/");
        ft_strcat(link_path, metadata->name);
        if (readlink(link_path, link, 100) < 0)
            perror("readlink");
        else
        {
            if (OPT_IS_COLOR(print->option))
            {
                struct stat sb;
                if (lstat(link, &sb) == -1)
                    ft_strcatindex(printer->buff, link, &printer->i);
                else
                {
                    char color[11];
                    get_color(color, sb.st_mode);
                    ft_strcatindex(printer->buff, color, &printer->i);
                    ft_strcatindex(printer->buff, link, &printer->i);
                    ft_strcatindex(printer->buff, COLOR_RESET, &printer->i);
                }
            }
            else
                ft_strcatindex(printer->buff, link, &printer->i);
        }
    }
    ft_strcatindex(printer->buff, "\n", &printer->i);
}

void print_list_format(char *argv, t_file *file_lst, t_print_opt *print)
{
    t_file *head = file_lst;
    t_printer printer = {0, {0, 0}};
    if (!ISNOTDIR(print->option))
        ft_printf("total %i\n", print->total_blcks_alloc / 2);
    for (; head; head = head->next)
    {
        fill_buffer_list(argv, head->metadata, print, &printer);
        write(1, printer.buff, printer.i);
        ft_bzero(&printer, sizeof(printer));
    }
    // When recursive call need to reset to 0;
    print->total_blcks_alloc = 0;
}