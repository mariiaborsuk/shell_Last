/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:05:32 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/27 00:12:21 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

void	print_exported_vars(t_var **var)
{
	t_var	*cur;

	cur = *var;
	while (cur)
	{
		printf("declare -x %s", cur->key);
		if (cur->exported == true)
			printf("=");
		if (cur->exported == true)
		{
			if (cur->val)
				printf("\"%s\"", cur->val);
			else
				printf("\"\"");
		}
		printf("\n");
		cur = cur->next;
	}
}

bool	is_valid_export_var_name(const char *var_name)
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
		if (!ft_isalnum(var_name[i]) && var_name[i] != '_')
		{
			return (false);
		}
		++i;
	}
	return (true);
}

int	handle_exported_var(const t_var *exported_var, t_var **var)
{
	char	*invalid_data[5];
	char	*invalid_message;

	if (!is_valid_export_var_name(exported_var->key))
	{
		invalid_data[0] = exported_var->key;
		invalid_data[1] = "=";
		invalid_data[2] = exported_var->val;
		invalid_data[3] = NULL;
		invalid_message = ft_implode(invalid_data);
		ft_putendl_fd(" not a valid identifier", 2);
		free(invalid_message);
		return (1);
	}
	add_var(exported_var, var);
	return (0);
}

static void	free_exported_var(t_var *exported_var)
{
	free(exported_var->key);
	if (exported_var->val)
		free(exported_var->val);
	free(exported_var);
}

int	exec_export(char **cmd, t_var **var)
{
	int		i;
	t_var	*exported_var;
	int		exec_result;

	exec_result = 0;
	i = 0;
	while (cmd[i])
		++i;
	if (i == 1)
	{
		print_exported_vars(var);
		return (exec_result);
	}
	i = 1;
	while (cmd[i])
	{
		exported_var = make_var_from_envp(cmd[i]);
		if (handle_exported_var(exported_var, var) == 1)
			exec_result = 1;
		++i;
		free_exported_var(exported_var);
	}
	return (exec_result);
}
