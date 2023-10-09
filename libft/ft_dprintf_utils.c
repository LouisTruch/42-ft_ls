
#include "libft.h"

int putchar_cnt_fd(char c, int count, int fd)
{
    write(fd, &c, 1);
    count++;
    return (count);
}

int putstr_cnt_fd(char *s, int count, int fd)
{
    if (!s)
    {
        count = putstr_cnt_fd("(null)", count, fd);
        return (count);
    }
    while (*s)
    {
        write(fd, s, 1);
        s++;
        count++;
    }
    return (count);
}

int basewrite_cnt_fd(long long nb, char *base, int count, int fd)
{
    write(fd, &base[nb], 1);
    count++;
    return (count);
}

int putnbrbase_cnt_fd(long long nb, char *base, int count, int fd)
{
    int base_l;

    base_l = ft_strlen(base);
    if (nb < 0)
    {
        count = putchar_cnt_fd('-', count, fd);
        nb *= -1;
    }
    if (nb < base_l)
        count = basewrite_cnt_fd(nb, base, count, fd);
    else
    {
        count = putnbrbase_cnt_fd(nb / base_l, base, count, fd);
        count = basewrite_cnt_fd(nb % base_l, base, count, fd);
    }
    return (count);
}

int putunbrbase_cnt_fd(unsigned long long nb, char *base, int count, int fd)
{
    unsigned long long base_l;

    base_l = ft_strlen(base);
    if (nb < base_l)
        count = basewrite_cnt_fd(nb, base, count, fd);
    else
    {
        count = putnbrbase_cnt_fd(nb / base_l, base, count, fd);
        count = basewrite_cnt_fd(nb % base_l, base, count, fd);
    }
    return (count);
}
