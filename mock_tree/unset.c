/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:13:46 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/26 22:17:41 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

bool	is_valid_unset_var_name(const char *var_name)
{
	int	i;

	if (!var_name)
		return (false);
	if (!ft_isalpha(var_name[0]) && var_name[0] != '_')
	{
		return (false);
	}
	i = 1;
	while (var_name[i])
	{
		if (!ft_isalnum(var_name[i]) && var_name[i] != '_'
			&& var_name[i] != '=')
		{
			return (false);
		}
		++i;
	}
	return (true);
}

int	handle_unset_var(char *var_name, t_var **var)
{
	if (!is_valid_unset_var_name(var_name))
	{
		return (0);
	}
	remove_var(var_name, var);
	return (0);
}

int	exec_unset(char **cmd, t_var **var)
{
	int	exit_status;
	int	i;

	exit_status = 0;
	i = 0;
	while (cmd[i])
		++i;
	if (i == 1)
		return (exit_status);
	i = 1;
	while (cmd[i])
	{
		if (handle_unset_var(cmd[i], var) == 1)
			exit_status = 1;
		++i;
	}
	return (exit_status);
}

void	remove_var(char *key, t_var **head)
{
	t_var	*current;
	t_var	*next;

	if (head == NULL || *head == NULL || key == NULL)
		return ;
	current = *head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			*head = current->next;
			free_var(current);
			return ;
		}
		if (current->next && ft_strcmp(current->next->key, key) == 0)
		{
			next = current->next;
			current->next = next->next;
			free_var(next);
			return ;
		}
		current = current->next;
	}
}
