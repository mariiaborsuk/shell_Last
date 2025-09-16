/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:11:35 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/16 22:28:43 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

void	manage_switch_2(int *ret, t_redirect *head, t_minishell *shell)
{
	if (head->type == append)
	{
		if (redirect_stdout_append(&head) != 0)
		{
			*ret = 1;
			shell->exit_status = *ret;
		}
	}
	if (head->type == heredoc)
	{
		if (head->fd != -1)
		{
			if (dup2(head->fd, STDIN_FILENO) == -1)
			{
				free_minishell(shell);
				exit(1);
			}
			close(head->fd);
			// unlink("temp.temp");
		}
	}
}

void	manage_switch(int *ret, t_redirect *head, t_minishell *shell)
{
	if (head->type == input)
	{
		if (open_std_in(&head) != 0)
		{
			*ret = 1;
			shell->exit_status=*ret;
		}
	}
	if (head->type == output)
	{
		if (open_std_out(&head) != 0)
		{
			*ret = 1;
			shell->exit_status=*ret;
		}
	}
	else
		manage_switch_2(ret, head, shell);
}

// int	check_heredoc(t_redirect *head, t_minishell *shell)
// {
// 	int	res;

// 	res = 0;
// 	while (head)
// 	{
// 		if (head->type == heredoc)
// 		{
// 			head->fd = get_temp_fd(head->file, shell, &res);
// 			// if (res == 130)
// 			// {
// 			// 	free_minishell(shell);
// 			// 	_exit(130);
// 			// }
// 			if (res != 0)
// 			{
// 				free_minishell(shell);
// 				_exit(1);
// 			}
// 		}
// 		head = head->next;
// 	}
// 	return (0);
// }
int	handle_redir(t_ast *node, t_var *var, t_minishell *shell)
{
	t_redirect	*head;
	t_redirect	*original_head;
	// t_redirect	*heredoc;
	int			ret;

	// heredoc = node->cmd->redirects;
	ret = 0;
	if (!node || !node->cmd)
	{
		return (0);
	}
	head = node->cmd->redirects;
	original_head = head;
	// check_heredoc(heredoc, shell);
	while (head)
	{
		if (!head)
			break ;
		manage_switch(&ret, head, shell);
		if (ret == 1)
			{
				free_minishell(shell);
				exit(1);
			}
		head = head->next;
	}
	return (handle_commands(var, shell, original_head, node));
}

void	node_with_redir(t_minishell *shell, t_ast *node, t_var **var)
{
	pid_t	child_id;
	int		num;
	int		status;

	status = 0;
	if (check_quote(shell->tokens) == 0)
		replace_var(&node, var);
	child_id = fork();
	if (child_id == 0)
	{
		num = handle_redir(node, *var, shell);
		shell->exit_status = num;
		free_minishell(shell);
		exit(num);
	}
	else if (child_id > 0)
		handle_status(status, child_id, shell);
}
