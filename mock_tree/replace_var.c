/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:12:48 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/26 20:39:52 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

void	replace_var(t_ast **node, t_var **head_var)
{
	int		i;
	char	*ptr;

	i = 0;
	ptr = NULL;
	while (*node && (*node)->cmd && (*node)->cmd->argv[i])
	{
		ptr = ft_strchr((*node)->cmd->argv[i], '$');
		if (ptr)
		{
			if (ptr[1] == ' ' || ptr[1] == '\0')
				return ;
			get_val(&(*node)->cmd->argv[i], *head_var);
		}
		i++;
	}
}

char	*get_replace(const char *name)
{
	char		*replace;
	int			i;
	const char	*dollar_pos;

	dollar_pos = ft_strchr(name, '$');
	i = 0;
	while (dollar_pos[i] != '}' && dollar_pos[i] != '\0' && dollar_pos[i] != 39 && dollar_pos[i] !=' '
		&& dollar_pos[i] != 34)
		i++;
	replace = malloc(i + 2);
	ft_strlcpy(replace, dollar_pos, i + 1);
	return (replace);
}

char	*get_var_name(const char *name)
{
	const char	*dollar = ft_strchr(name, '$');
	const char	*start;
	size_t		len;
	char		*var;

	len = 0;
	if (dollar[0] == '\0')
		return (NULL);
	start = dollar + 1;
	if (*start == '\0')
		return (NULL);
	len = ft_strlen(start);
	var = malloc(len + 1);
	if (!var)
		return (NULL);
	ft_strlcpy(var, start, len + 1);
	return (var);
}

void	get_cur_val(t_var *head, char *var_name, char **name, char *replace)
{
	t_var	*cur;
	int		found;

	cur = head;
	found = 0;
	if (!var_name)
		return ;
	while (cur != NULL)
	{
		if (cur->key != NULL && ft_strcmp(cur->key, var_name) == 0)
		{
			found = 1;
			break ;
		}
		cur = cur->next;
	}
	if (found)
	{
		free_and_replace(name, replace, cur->val, var_name);
		found = 0;
	}
	else
		free_and_replace(name, replace, "", var_name);
}

char	*remove_braces(char *name)
{
	char	*filtered_name;
	int		length;
	int		i;
	int		m;

	length = ft_strlen(name);
	filtered_name = calloc(length + 1, 1);
	if (!filtered_name)
		return (NULL);
	i = 0;
	m = 0;
	while (name[i])
	{
		if (name[i] != '{' && name[i] != '}')
		{
			filtered_name[m] = name[i];
			++m;
		}
		++i;
	}
	filtered_name[m] = '\0';
	return (filtered_name);
}
