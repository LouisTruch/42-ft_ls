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

#define FLAG_PERCENT '%'
#define FLAG_STR 's'
#define FLAG_INT 'd'
#define ASCII_OFFSET 48

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
            buff[*count] = 0 + ASCII_OFFSET;
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
            buff[(*count) + i_width - 1] = i % 10 + ASCII_OFFSET;
            i_width--;
            i /= 10;
        }
        (*count) += i_width_cpy;
    }
}

// DOES NOT HANDLE INT MIN + HANDLE ONLY '%d %s %%'
int mini_sprintf(char *buff, const char *format, ...)
{
    if (!format)
        return -1;

    va_list arg;
    va_start(arg, format);

    int count = 0;
    while (*format)
    {
        if (*format == '%')
        {
            convert_flag(buff, *(format + 1), arg, &count);
            format++;
        }
        else
            buff[count++] = *format;
        format++;
    }
    buff[count] = '\0';
    return count;
}