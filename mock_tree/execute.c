/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:29:17 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/28 22:44:59 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/minishell.h"
#include <errno.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>

int	argv_len(char **argv)
{
	int	len;

	len = 0;
	while (argv[len])
	{
		len++;
	}
	return (len);
}

void	is_dir(char **argv, t_minishell *sh, char **envp)
{
	struct stat	sb;

	if (stat(argv[0], &sb) == -1)
	{
		sh->exit_status = 127;
		perror_exit(argv[0], sh, 127);
	}
	if (access(argv[0], F_OK) != 0)
	{
		perror_exit(" No such file or directory\n ", sh, 127);
	}
	if (S_ISDIR(sb.st_mode))
	{
		errno = EISDIR;
		perror_exit(argv[0], sh, 126);
	}
	if (access(argv[0], X_OK) != 0)
	{
		perror_exit(argv[0], sh, 126);
	}
	execve(argv[0], argv, envp);
	perror_exit("execve\n", sh, 126);
}

int	prepare_heredocs(t_redirect *rlist, t_minishell *sh)
{
	int	st;

	st = 0;
	while (rlist)
	{
		if (rlist->type != heredoc)
			continue ;
		(rlist)->fd = get_temp_fd(rlist->file, sh, &st);
		if ((rlist)->fd < 0)
		{
			sh->exit_status = (st == 130) ? 130 : 1;
			return ((st == 130) ? 130 : 1);
		}
		rlist = rlist->next;
	}
	return (0);
}

int	extern_cmd(char **argv, char **envp, t_minishell *sh)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), sh->exit_status = 1);
	if (pid == 0)
	{
		if (argv == NULL)
			exit(0);
		empty_argv(argv, sh);
		if (ft_strchr(argv[0], '/') != NULL)
			is_dir(argv, sh, envp);
		else
			exec_and_exit(argv, sh, envp);
	}
	child_running(&status, pid);
	return (extrern_exit(status, sh));
}

bool	handle_node_no_redir(t_ast *node, t_var **var, t_minishell *shell,
		char **local_envp)
{
	if (check_quote(shell->tokens) == 0)
		replace_var(&node, var);
	if (is_built_in(node->cmd->argv) == 0)
		shell->exit_status = extern_cmd(node->cmd->argv, local_envp, shell);
	else
		shell->exit_status = execute_built_in(node->cmd->argv, var, shell);
	return (true);
}

bool	execute_ast(t_ast *node, t_var **var, t_minishell *shell)
{
	if (node == NULL)
		return (false);
	replace_relative(node, shell);
	if (node->type == node_command && node->cmd && node->cmd->redirects == NULL)
	{
		handle_node_no_redir(node, var, shell, shell->envp);
	}
	if (node->type == node_command && node->cmd && node->cmd->redirects != NULL)
	{
		node_with_redir(shell, node, var);
	}
	if (node->type == node_pipe)
	{
		execute_pipe(node, *var, shell);
	}
	if (node->type != node_pipe && node->left != NULL)
		execute_ast(node->left, var, shell);
	if (node->type != node_pipe && node->right != NULL)
		execute_ast(node->right, var, shell);
	return (true);
}
