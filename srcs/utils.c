#include "../inc/ft_ls.h"

void get_complete_path(char *str1, char *str2, char *str3)
{
    ft_bzero(str1, sizeof(str1));
    ft_strcat(str1, str2);
    ft_strcat(str1, "/");
    ft_strcat(str1, str3);
}

int max(int a, int b)
{
    return a > b ? a : b;
}