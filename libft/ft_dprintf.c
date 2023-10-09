/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 11:38:31 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/26 21:35:36 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int manage_arg_p_fd(unsigned long nb, int count, int fd)
{
    if (nb == 0)
        count = putstr_cnt_fd("(nil)", count, fd);
    else
    {
        count = putstr_cnt_fd("0x", count, fd);
        count = putunbrbase_cnt_fd(nb, B_HEXL, count, fd);
    }
    return (count);
}

int to_convert_fd(char f, va_list args, int count, int fd)
{
    if (f == 'c')
        count = putchar_cnt_fd(va_arg(args, int), count, fd);
    else if (f == 's')
        count = putstr_cnt_fd(va_arg(args, char *), count, fd);
    else if (f == 'p')
        count = manage_arg_p_fd(va_arg(args, unsigned long long), count, fd);
    else if (f == 'd' || f == 'i')
        count = putnbrbase_cnt_fd(va_arg(args, int), B_DEC, count, fd);
    else if (f == 'u')
        count = putunbrbase_cnt_fd(va_arg(args, unsigned int), B_DEC, count, fd);
    else if (f == 'x')
        count = putnbrbase_cnt_fd(va_arg(args, unsigned int), B_HEXL, count, fd);
    else if (f == 'X')
        count = putnbrbase_cnt_fd(va_arg(args, unsigned int), B_HEXU, count, fd);
    else if (f == '%')
        count = putchar_cnt_fd('%', count, fd);
    else
    {
        count = putchar_cnt_fd('%', count, fd);
        count = putchar_cnt_fd(f, count, fd);
    }
    return (count);
}

int ft_dprintf(int fd, const char *format, ...)
{
    va_list args;
    int count;

    va_start(args, format);
    count = 0;
    if (!format)
        return (-1);
    while (*format)
    {
        if (*format == '%')
        {
            count = to_convert_fd(*(format + 1), args, count, fd);
            format++;
        }
        else
            count = putchar_cnt_fd(*format, count, fd);
        format++;
    }
    va_end(args);
    return (count);
}
