/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalch <akovalch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:54:59 by akovalch          #+#    #+#             */
/*   Updated: 2025/02/17 18:40:37 by akovalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	set_compare(char c, const char *set)
{
	while (*set)
	{
		if (c == *set)
		{
			return (1);
		}
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	int		length;
	char	*newstr;

	start = 0;
	end = ft_strlen(s1);
	if (end - start == 0)
		return (ft_strdup(""));
	if (!s1 || !set)
		return (0);
	while (s1[start] && set_compare(s1[start], set))
		start++;
	while (end > start && set_compare(s1[end - 1], set))
		end--;
	length = end - start;
	newstr = malloc((length + 1) * sizeof(char));
	if (newstr == NULL)
		return (NULL);
	ft_strlcpy(newstr, s1 + start, length + 1);
	newstr[length] = '\0';
	return (newstr);
}
