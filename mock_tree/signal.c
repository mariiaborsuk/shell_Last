/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:31:27 by akovalch          #+#    #+#             */
/*   Updated: 2025/09/24 14:52:12 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/minishell.h"
#include "../includes/signals.h"
#include <errno.h>
#include <readline/readline.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

int		g_state = 0;
// void	signal_reset_prompt(int signo)
// {
// 	(void)signo;
// 	write(1, "\n", 1);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

void	parent_sigint(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	g_state |= GOT_SIGINT;
	// global=130;
	if (!(g_state & CHILD_RUNNING))
	{
	 rl_replace_line("", 0);
		 rl_on_new_line();
	 rl_redisplay();
	}
}

// void	handle_sigint(int signum)
// {
// 	(void)signum;
// 	g_state = 1;
// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// }

void	child_running(int *status, pid_t pid)
{
	g_state |= CHILD_RUNNING;
	while (waitpid(pid, status, 0) == -1 && errno == EINTR)
		;
	g_state &= ~CHILD_RUNNING;
}

void	if_g_state(t_minishell *shell)
{
	if (g_state & GOT_SIGINT)
	{
		shell->exit_status = 130;
		g_state &= ~GOT_SIGINT;
	}
}
