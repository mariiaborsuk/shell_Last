/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:06:44 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/26 11:06:21 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

void	restore_fds(int save_stdin, int save_stdout)
{
	if (save_stdin != -1)
	{
		dup2(save_stdin, STDIN_FILENO);
		close(save_stdin);
	}
	if (save_stdout != -1)
	{
		dup2(save_stdout, STDOUT_FILENO);
		close(save_stdout);
	}
}

void	close_files(t_redirect *head)
{
	t_redirect	*cur;

	cur = head;
	while (cur)
	{
		if (cur->fd != -1 && cur->open == 1 && cur->fd > 2)
		{
			close(cur->fd);
			cur->open = 0;
			cur->fd = -1;
		}
		cur = cur->next;
	}
}

void	unlink_close(int rfd)
{
	if (rfd > 2)
	{
		close(rfd);
		rfd = -1;
		unlink("temp.temp");
	}
}

void	cmd_not_found(char *str, t_minishell *shell, char *er)
{
	ft_putstr_fd(str, 2);
	if (er != NULL)
		write(2, er, ft_strlen(er));
	free_minishell(shell);
	exit(127);
}

// void	dup_files(t_minishell *shell, t_ast *node)
// {
// 	shell->stdin = dup(STDIN_FILENO);
// 	shell->stdout = dup(STDOUT_FILENO);
// 	if (shell->stdin == -1 || shell->stdout == -1)
// 	{
// 		perror("dup failed");
// 		shell->exit_status = 1;
// 		return ;
// 	}
// 	if (node->cmd->redirects)
// 	{
// 		if (handle_redir(node->cmd, shell, node->cmd->redirects) != 0)
// 		{
// 			restore_fds(shell->stdin, shell->stdout);
// 			shell->exit_status = 1;
// 			return ;
// 		}
// 	}
// }
