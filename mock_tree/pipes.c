/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:11:10 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/10 17:31:55 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

void	handle_sig_p(int sig)
{
	(void)sig;
	g_state = 2;
}
void	handle_status(int status, pid_t pid, t_minishell *shell)
{
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

t_ast	*get_leftmost_command(t_ast *node)
{
	if (!node)
		return (NULL);
	if (node->type == node_command)
		return (node);
	if (node->type == node_pipe && node->left)
		return (get_leftmost_command(node->left));
	return (NULL);
}

void	install_pipe_sig(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sig_p;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}
bool	right_error(t_process p)
{
	close(p.pipefd[0]);
	close(p.pipefd[1]);
	waitpid(p.pid1, &p.status1, 0);
	return (false);
}
bool	handle_right_child(t_process p, t_ast *node, t_minishell *shell,
		t_var *var)
{
	if (p.pid2 == -1)
	{
		right_error(p);
	}
	if (p.pid2 == 0)
	{
		install_pipe_sig();
		close(p.pipefd[1]);
		dup2(p.pipefd[0], STDIN_FILENO);
		close(p.pipefd[0]);
		if (node->type == node_pipe)
			execute_pipe(node, var, shell);
		else
			manage_cmd(node, var, shell);
		if (g_state == 2)
			shell->exit_status = 130;
		free_minishell(shell);
		exit(shell->exit_status);
	}
	return (true);
}
void	left_errror(t_process p, t_minishell *shell)
{
	close(p.pipefd[0]);
	close(p.pipefd[1]);
	perror("Fork failed for left process");
	free_minishell(shell);
	exit(1);
}
void	manage_left_child(t_process p, int rfd, t_ast *node, t_minishell *shell)
{
	if (p.pid1 == -1)
		left_errror(p, shell);
	if (p.pid1 == 0)
	{
		install_pipe_sig();
		if (rfd != -1)
		{
			if (dup2(rfd, STDIN_FILENO) == -1)
				_exit(1);
			close(rfd);
		}
		close(p.pipefd[0]);
		dup2(p.pipefd[1], STDOUT_FILENO);
		close(p.pipefd[1]);
		if (node->type == node_pipe)
		{
			shell->close_parent_stdout = 1;
			execute_pipe(node, shell->var, shell);
			shell->close_parent_stdout = 0;
		}
		else
			manage_cmd(node, shell->var, shell);
		if (g_state == 2)
			shell->exit_status = 130;
		free_minishell(shell);
		exit(shell->exit_status);
	}
}

bool	execute_pipe(t_ast *node, t_var *var, t_minishell *shell)
{
	t_process	p;
	int			rfd;

	rfd = -1;
	ft_memset(&p, 0, sizeof p);
	if (pipe(p.pipefd) == -1)
	{
		perror("pipe failed");
		return (false);
	}
	p.pid1 = fork();
	manage_left_child(p, rfd, node->left, shell);
	p.pid2 = fork();
	handle_right_child(p, node->right, shell, var);
	close(p.pipefd[0]);
	close(p.pipefd[1]);
	if (shell->close_parent_stdout)
		close(STDOUT_FILENO);
	child_running(&p.status1, p.pid1);
	child_running(&p.status2, p.pid2);
	unlink_close(rfd);
	return (true);
}
