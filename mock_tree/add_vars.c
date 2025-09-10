/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:53:00 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/27 00:11:32 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

static t_var	*free_func(t_var *new_var)
{
	free(new_var->key);
	free(new_var);
	return (NULL);
}

t_var	*add_new_var(char *val, char *key, bool exported)
{
	t_var	*new_var;

	new_var = malloc(sizeof(t_var));
	if (new_var == NULL)
		return (NULL);
	new_var->key = ft_strdup(key);
	if (!new_var->key)
	{
		free_var(new_var);
		return (NULL);
	}
	if (val == NULL)
		new_var->val = NULL;
	else
	{
		new_var->val = ft_strdup(val);
		if (new_var->val == NULL)
			return (free_func(new_var));
	}
	new_var->exported = exported;
	new_var->next = NULL;
	return (new_var);
}

void	modify_old_var(char *val, t_var *old_var, t_var **head)
{
	char	*new_val;

	new_val = NULL;
	if (val)
	{
		new_val = ft_strdup(val);
		if (!new_val)
		{
			free_var_list(head);
			return ;
		}
	}
	free(old_var->val);
	old_var->val = new_val;
	old_var->exported = true;
}

void	add_var(const t_var *exported_var, t_var **head)
{
	t_var	*new_var;
	t_var	*cur;
	t_var	*old_var;

	if (!exported_var->key)
		return ;
	old_var = check_var(head, exported_var->key);
	if (old_var == NULL)
	{
		new_var = add_new_var(exported_var->val, exported_var->key,
				exported_var->exported);
		if (new_var == NULL)
			return ;
		if (*head == NULL)
			*head = new_var;
		else
		{
			cur = *head;
			while (cur->next != NULL)
				cur = cur->next;
			cur->next = new_var;
		}
	}
	else
		modify_old_var(exported_var->val, old_var, head);
}
