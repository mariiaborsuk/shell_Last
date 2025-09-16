/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:21:43 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/16 20:30:34 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

char	*if_heredoc(t_cmd *cmd)
{
	t_redirect	*temp;
	char		*heredoc_del;

	heredoc_del = NULL;
	if (!cmd)
		return (NULL);
	temp = cmd->redirects;
	while (temp)
	{
		if (temp->type == heredoc)
		{
			heredoc_del = temp->file;
		}
		temp = temp->next;
	}
	return (heredoc_del);
}

// void	rfd_invalid(t_minishell *shell)
// {
// 	free_minishell(shell);
// 	perror("open for read");
// 	unlink("temp.temp");
// 	exit(1);
// }

// void	wfd_invalid(int wfd, t_minishell *shell)
// {
// 	if (wfd == -1)
// 	{
// 		perror("open temp file for writing");
// 		free_minishell(shell);
// 		exit(1);
// 	}
// }

int	get_temp_fd(char *delimiter, t_minishell *shell, int *status)
{
	int	fd;
	// int	result;

	// fd = open("temp.temp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	// if (fd < 0)
	// 	return (-1);
fd = create_heredoc_pipe(delimiter, shell, status);
	// close(fd);
	// install_parent_signals();
	// if (result == 130)
	// {
	// 	*status = 130;
	// 	unlink("temp.temp");
	// 	return (-1);
	// }
	// fd = open("temp.temp", O_RDONLY);
	return (fd);
}
void	replace_heredoc_val(t_var *var, char *str)
{
	char	*ptr;

	ptr = NULL;
	ptr = ft_strchr(str, '$');
	if (ptr)
	{
		if (ptr[1] == ' ' || ptr[1] == '\0')
			return ;
		get_val(&str, var);
	}
}
