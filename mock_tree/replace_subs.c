/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_subs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:12:25 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/26 20:40:32 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

char	*ft_strstr(const char *str, const char *needle)
{
	int	count;
	int	i;
	int	j;

	if (needle[0] == '\0')
		return ((char *)str);
	i = 0;
	while (str[i])
	{
		count = i;
		j = 0;
		while (needle[j] && needle[j] == str[count])
		{
			count++;
			j++;
		}
		if (needle[j] == '\0')
			return ((char *)str + i);
		i++;
	}
	return (NULL);
}

int	get_sub_count(int old_len, const char *str, const char *old_sub)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (ft_strstr(&str[i], old_sub) == &str[i])
		{
			count++;
			i += old_len;
		}
		else
			i++;
	}
	return (count);
}

void	modify_result(const char *str, const char *old_sub, char *result,
		const char *new_sub)
{
	int	i;
	int	old_len;
	int	new_len;

	i = 0;
	old_len = ft_strlen(old_sub);
	new_len = ft_strlen(new_sub);
	while (*str)
	{
		if (ft_strstr(str, old_sub) == str)
		{
			ft_memcpy(result + i, new_sub, new_len);
			i += new_len;
			str += old_len;
		}
		else
			result[i++] = *str++;
	}
	result[i] = '\0';
}

char	*replace_substring(const char *str, const char *old_sub,
		const char *new_sub)
{
	char	*result;
	int		old_len;
	int		new_len;
	int		count;

	old_len = ft_strlen(old_sub);
	new_len = ft_strlen(new_sub);
	count = get_sub_count(old_len, str, old_sub);
	result = malloc(ft_strlen(str) + count * (new_len - old_len) + 1);
	if (!result)
		return (NULL);
	modify_result(str, old_sub, result, new_sub);
	return (result);
}

void	get_val(char **name, t_var *head)
{
	char	*replace;
	char	*var_name;
	char	*filtered_name;

	if (*name == NULL)
		return ;
	if (head == NULL)
		return ;
	if (ft_strchr(*name, '$') != NULL)
	{
		replace = get_replace(*name);
		if (replace == NULL)
			return ;
		filtered_name = remove_braces(*name);
		var_name = get_var_name(replace);
		if (var_name == NULL)
		{
			free(replace);
			free(filtered_name);
			return ;
		}
		get_cur_val(head, var_name, name, replace);
		free(filtered_name);
		free(replace);
	}
}
