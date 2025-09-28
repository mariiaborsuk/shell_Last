/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:53:56 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/25 21:17:59 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>

bool	init_var_list_from_envp(t_minishell *shell, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!add_env_var(&shell->var, envp[i]))
		{
			free_var_list(&shell->var);
			return (false);
		}
		++i;
	}
	return (true);
}

bool	init_minishell(t_minishell *shell, char **envp)
{
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->var = NULL;
	shell->exit_status = 0;
	shell->stdin = -1;
	shell->stdout = -1;
	shell->close_parent_stdout=0;
	shell->envp = NULL;
	shell->line = NULL;
	if (!init_var_list_from_envp(shell, envp))
		return (false);
	return (true);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	shell;

	(void)argc;
	(void)argv;
	if (!init_minishell(&shell, envp))
	{
		free_minishell(&shell);
		rl_clear_history();
		return (EXIT_FAILURE);
	}
	if (!shell_read_loop(&shell))
	{
		int exit_status = shell.exit_status;
		free_minishell(&shell);
		rl_clear_history();
		return (exit_status);
	}
	free_minishell(&shell);
	rl_clear_history();
	return (shell.exit_status);
}

bool	handle_envp(t_minishell *shell)
{
	if (shell->envp)
		free_argv(&shell->envp);
	shell->envp = convert_var_to_envp(&shell->var);
	if (!shell->envp)
		return (false);
	return (true);
}
