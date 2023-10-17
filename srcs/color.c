#include "../inc/ft_ls.h"

void get_color(char *color, mode_t mode)
{
    if (S_ISREG(mode))
    {
        if (S_IXUSR & mode && S_IXGRP & mode && S_IXOTH & mode)
            ft_strcpy(color, COLOR_EXEC);
        else
            ft_strcpy(color, COLOR_RESET);
    }
    else if (S_ISDIR(mode))
        ft_strcpy(color, COLOR_DIR);
    else if (S_ISCHR(mode))
        ft_strcpy(color, COLOR_CHR);
    else if (S_ISBLK(mode))
        ft_strcpy(color, COLOR_RESET);
    else if (S_ISFIFO(mode))
        ft_strcpy(color, COLOR_FIFO);
    else if (S_ISLNK(mode))
        ft_strcpy(color, COLOR_LINK);
    else if (S_ISSOCK(mode))
        ft_strcpy(color, COLOR_SOCK);
}