/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalch <akovalch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 17:07:32 by akovalch          #+#    #+#             */
/*   Updated: 2025/07/06 20:01:31 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_countword(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != c && *s)
			s++;
	}
	return (count);
}

static void	free_all(char **lst, int i)
{
	while (i > 0)
	{
		i--;
		free(lst[i]);
	}
	free(lst);
}

static char	*extract_word(char const *s, char c)
{
	char	*word;
	int		word_len;

	if (!ft_strchr(s, c))
		word_len = ft_strlen(s);
	else
		word_len = ft_strchr(s, c) - s;
	word = ft_substr(s, 0, word_len);
	if (!word)
		return (NULL);
	return (word);
}

static char	**parse_string(char const *s, char **lst, char c)
{
	int		i;
	char	*temp_string;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			temp_string = extract_word(s, c);
			if (!temp_string)
			{
				free_all(lst, i);
				return (NULL);
			}
			lst[i++] = temp_string;
			s += ft_strlen(temp_string);
		}
	}
	lst[i] = NULL;
	return (lst);
}

char	**ft_split(char const *s, char c)
{
	char	**lst;

	if (!s)
		return (NULL);
	lst = malloc((ft_countword(s, c) + 1) * sizeof(char *));
	if (lst == NULL)
		return (NULL);
	return (parse_string(s, lst, c));
}
