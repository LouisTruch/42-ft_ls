#include "../inc/ft_ls.h"

void get_complete_path(char *str1, size_t len, char *str2, char *str3)
{
    ft_bzero(str1, len);
    ft_strcat(str1, str2);
    ft_strcat(str1, "/");
    ft_strcat(str1, str3);
}

int max(int a, int b)
{
    return a > b ? a : b;
}

void lltoa_no_alloc(char *str, long long nb)
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
        str[i++] = '-';
    str[i] = '\0';
    reverse_str(str);
}

#define FLAG_PERCENT '%'
#define FLAG_STR 's'
#define FLAG_INT 'd'

static void convert_flag(char *buff, const char flag, va_list arg, int *count)
{
    if (flag == FLAG_PERCENT)
    {
        buff[*count] = '%';
        (*count)++;
    }
    else if (flag == FLAG_STR)
    {
        char *str = va_arg(arg, char *);
        for (size_t idx = 0; str[idx]; idx++)
        {
            buff[*count] = str[idx];
            (*count)++;
        }
    }
    else if (flag == FLAG_INT)
    {
        long i = va_arg(arg, long);
        if (i == 0)
        {
            buff[*count] = 0 + '0';
            (*count)++;
            return;
        }
        int i_width = 0;
        if (i < 0)
        {
            buff[*count] = '-';
            i = -i;
            i_width++;
        }
        // Should use reverse instead of this for perf
        long i_cpy = i;
        while (i_cpy)
        {
            i_width++;
            i_cpy /= 10;
        }
        int i_width_cpy = i_width;
        while (i)
        {
            buff[(*count) + i_width - 1] = i % 10 + '0';
            i_width--;
            i /= 10;
        }
        (*count) += i_width_cpy;
    }
}

void reverse_str(char *str)
{
    char c;
    for (int i = 0, j = ft_strlen(str) - 1; i < j; i++, j--)
    {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

static void convert_long(char *buff, long nb, int *count)
{
    char str[20] = {0};
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
    for (i = 0; str[i]; i++, (*count)++)
        buff[*count] = str[i];
}

// DOES NOT HANDLE INT MIN + HANDLE ONLY '%d %s %li %%'
int mini_sprintf(char *buff, const char *format, ...)
{
    if (!format)
        return -1;

    va_list arg;
    va_start(arg, format);

    int count = 0;
    for (int i = 0; format[i]; i++)
    {
        if (format[i] == '%' && format[i + 1] == 'l' && format[i + 2] == 'i')
        {
            convert_long(buff, va_arg(arg, long), &count);
            i += 2;
        }
        else if (format[i] == '%')
        {
            convert_flag(buff, format[i + 1], arg, &count);
            i++;
        }
        else
            buff[count++] = format[i];
    }
    buff[count] = '\0';
    return count;
}