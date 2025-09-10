/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_var.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 22:24:39 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/26 22:16:44 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

void	free_var_list(t_var **head)
{
	t_var	*temp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		temp = (*head)->next;
		free_and_null((void **)&(*head)->key);
		free_and_null((void **)&(*head)->val);
		free_and_null((void **)head);
		*head = temp;
	}
	*head = NULL;
}

void	free_var(t_var *var)
{
	if (!var)
		return ;
	free(var->key);
	free(var->val);
	free(var);
}

void	free_and_replace(char **name, char *replace, char *val, char *var_name)
{
	char	*new_str;

	new_str = replace_substring(*name, replace, val);
	free(var_name);
	free(*name);
	*name = new_str;
}

void	free_ar(char **ar)
{
	int	i;

	i = 0;
	if (!ar)
		return ;
	while (ar[i] != NULL)
	{
		free(ar[i]);
		ar[i] = NULL;
		i++;
	}
	free(ar);
	ar = NULL;
}

void	free_execve(char *ar, t_minishell *shell)
{
	write(2, "bash: ", 6);
	write(2, ar, ft_strlen(ar));
	write(2, ": command not found\n", 20);
	if (shell->envp)
		free_argv(&shell->envp);
	free_minishell(shell);
	exit(127);
}
