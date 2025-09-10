/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:14:55 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/27 00:05:26 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

t_var	*make_var_from_envp(char *envp_row)
{
	char	*split_char;
	t_var	*var;
	char	*key;

	if (!envp_row)
		return (NULL);
	var = malloc(sizeof(t_var));
	if (!var)
		return (NULL);
	split_char = ft_strchr(envp_row, '=');
	if (!split_char)
	{
		var->key = ft_strdup(envp_row);
		var->val = NULL;
		var->exported = false;
	}
	else
	{
		key = ft_substr(envp_row, 0, (int)(split_char - envp_row));
		var->key = key;
		var->val = ft_strdup(split_char + 1);
		var->exported = true;
	}
	var->next = NULL;
	return (var);
}

char	**free_envp(char **envp)
{
	if (envp)
	{
		free_argv(&envp);
		envp = NULL;
	}
	return (envp);
}

char	**convert_var_to_envp(t_var **var)
{
	char	**envp;
	int		i;
	t_var	*current;

	envp = malloc(sizeof(char *) * (get_var_count(*var) + 1));
	if (!envp)
		return (NULL);
	current = *var;
	i = 0;
	while (current)
	{
		if (current->exported == false)
		{
			current = current->next;
			continue ;
		}
		envp[i] = ft_implode((char *[]){current->key, "=", current->val, NULL});
		if (!envp[i])
			return (free_argv(&envp), NULL);
		current = current->next;
		++i;
	}
	envp[i] = NULL;
	return (envp);
}

char	*substring_until_ptr(const char *str, const char *end_ptr)
{
	size_t	length;
	char	*result;

	length = end_ptr - str;
	result = (char *)malloc(length + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, str, length);
	result[length] = '\0';
	return (result);
}

void	expand_variable(t_var *head, char **v_name)
{
	t_var	*cur;

	cur = head;
	while (cur)
	{
		if (ft_strcmp(cur->key, *v_name + 1) == 0)
		{
			*v_name = cur->val;
			return ;
		}
		cur = cur->next;
	}
}
