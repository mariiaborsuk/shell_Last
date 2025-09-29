/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 22:28:24 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/26 13:51:28 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/signals.h"

void	exec_and_exit(char **argv, t_minishell *sh, char **envp)
{
	use_execve(argv, envp, sh);
	free_minishell(sh);
	exit(126);
}

int	check_quote(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	while (cur)
	{
		if (cur->squote || cur->dquote)
			return (1);
		cur = cur->next;
	}
	return (0);
}

void	perror_exit(char *er, t_minishell *sh, int num)
{
	write(2, er, ft_strlen(er));
	free_minishell(sh);
	exit(num);
}

void	bash_style_error_exit(char *cmd, char *error_msg, t_minishell *sh, int num)
{
	write(2, "bash: ", 6);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	write(2, error_msg, ft_strlen(error_msg));
	write(2, "\n", 1);
	free_minishell(sh);
	exit(num);
}

void	install_child_signals(void)
{
	struct sigaction	dfl;

	ft_memset(&dfl, 0, sizeof(dfl));
	dfl.sa_handler = SIG_DFL;
	sigemptyset(&dfl.sa_mask);
	dfl.sa_flags = 0;
	sigaction(SIGINT, &dfl, NULL);
	sigaction(SIGQUIT, &dfl, NULL);
}

void	put_line(char *line, int fd)
{
	size_t	len;

	len = ft_strlen(line);
	write(fd, line, len);
	write(fd, "\n", 1);
	free(line);
}
