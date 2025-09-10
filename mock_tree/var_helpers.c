/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:14:12 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/27 00:05:45 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

int	ft_get_total_ar_length(char **array)
{
	int	i;
	int	k;
	int	j;

	if (!array || !*array)
		return (0);
	i = 0;
	k = 0;
	j = 0;
	while (array[i])
	{
		while (array[i][k])
		{
			++j;
			++k;
		}
		k = 0;
		++i;
	}
	return (j);
}

char	*ft_implode(char **array)
{
	char	*result;
	int		i;
	int		k;
	int		j;

	i = 0;
	k = 0;
	j = 0;
	result = (char *)malloc(sizeof(char) * (ft_get_total_ar_length(array) + 1));
	if (!result)
		return (NULL);
	while (array[i])
	{
		while (array[i][k])
		{
			result[j] = array[i][k];
			++k;
			++j;
		}
		k = 0;
		++i;
	}
	result[j] = '\0';
	return (result);
}

int	get_var_count(t_var *var)
{
	t_var	*current;
	int		count;

	if (!var)
		return (0);
	count = 0;
	current = var;
	while (current)
	{
		++count;
		current = current->next;
	}
	return (count);
}

int	count_var(char **cmd)
{
	int	i;
	int	j;
	int	del;

	i = 0;
	while (cmd[i])
		i++;
	j = 0;
	del = 0;
	while (cmd[j])
	{
		if (ft_strchr(cmd[j], '=') != NULL && cmd[j][0] != '=')
			del++;
		j++;
	}
	return (i - del);
}

t_var	*check_var(t_var **head, char *key)
{
	t_var	*cur;

	cur = *head;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			return (cur);
		}
		cur = cur->next;
	}
	return (NULL);
}
