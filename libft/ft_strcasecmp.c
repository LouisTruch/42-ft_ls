#include "libft.h"

int ft_strcasecmp(const char *s1, const char *s2)
{
	const char *p1 = s1;
	const char *p2 = s2;

	if (p1 == p2)
		return 0;

	int result = 0;
	while (*p1 || *p2)
	{
		if (*p1 != *p2)
		{
			while ((!ft_isalnum(*p1) && *p1) || (!ft_isalnum(*p2) && *p2))
			{
				p1++;
				p2++;
			}
			while ((ft_tolower(*p1)) == ft_tolower(*p2))
			{
				p1++;
				p2++;
			}
			// result = 1;
			result = ft_tolower(*p1) - ft_tolower(*p2);
			break;
		}
		p1++;
		p2++;
	}
	return result;
}