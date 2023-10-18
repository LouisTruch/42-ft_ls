char *ft_strcatindex(char *dest, char *src, int *i)
{
    int j;

    j = 0;
    while (src[j])
    {
        dest[*i] = src[j];
        j++;
        (*i)++;
    }
    dest[*i] = '\0';
    return (dest);
}