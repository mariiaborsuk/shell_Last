/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:37:17 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/26 12:00:21 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/minishell.h"

void	free_and_null(void **ptr)
{
	if (!ptr || !*ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

bool	free_fun(t_minishell *shell, char **new_line)
{
	free_list(&shell->tokens);
	free_and_null((void **)new_line);
	return (false);
}

void	reset_shell(t_minishell *shell)
{
	if (!shell)
		return ;
	if (shell->line)
		free_and_null((void **)&shell->line);
	if (shell->tokens)
		free_list(&shell->tokens);
	if (shell->envp)
		free_argv(&shell->envp);
	if (shell->ast)
		free_ast(&shell->ast);
	shell->tokens = NULL;
	shell->envp = NULL;
	shell->ast = NULL;
}

void	free_minishell(t_minishell *shell)
{
	if (!shell)
		return ;
	if (shell->line)
		free_and_null((void **)&shell->line);
	if (shell->var)
		free_var_list(&shell->var);
	if (shell->tokens)
		free_list(&shell->tokens);
	if (shell->envp)
		free_argv(&shell->envp);
	if (shell->ast)
		free_ast(&shell->ast);
	shell->var = NULL;
	shell->tokens = NULL;
	shell->envp = NULL;
	shell->ast = NULL;
}
