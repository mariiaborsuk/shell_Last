/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:56:21 by akovalch          #+#    #+#             */
/*   Updated: 2025/09/09 23:03:55 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/minishell.h"
#include "../includes/signals.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

void	install_parent_signals(void)
{
	struct sigaction	sa;
	struct termios		t;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = parent_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
	if (tcgetattr(STDIN_FILENO, &t) == 0)
	{
		t.c_lflag &= ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &t);
	}
}

bool	shell_read_loop(t_minishell *shell)
{
	rl_catch_signals = 0;
	install_parent_signals();
	while (1)
	{
		if (!handle_envp(shell))
			return (reset_shell(shell), false);
		if_g_state(shell);
		shell->line = readline("Minishell % ");
		if (!shell->line)
		{
			printf("Exit\n");
			reset_shell(shell);
			return (false);
		}
		if (*shell->line)
			add_history(shell->line);
		if (lexing(shell, shell->line, shell->envp) && parsing(shell))
			execute_ast(shell->ast, &shell->var, shell);
		// if (shell->exit_status == 130)
		// {
		// 	printf("DEBUG: Handling 130 status\n");
		// 	shell->exit_status = 0;
		// 	rl_replace_line("", 0);
		// 	rl_redisplay();
		// }
		reset_shell(shell);
	}
	return (true);
}
