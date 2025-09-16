/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 11:04:13 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/16 16:04:40 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/signals.h"

void	empty_argv(char **argv, t_minishell *sh)
{
	if (argv[0] == NULL && sh->tokens->dquote == false
		&& sh->tokens->squote == false)
	{
		free_minishell(sh);
		exit(0);
	}
	if (argv[0] == NULL && (sh->tokens->dquote == true
			|| sh->tokens->squote == true))
	{
		ft_putendl_fd("Command '' not found", 2);
		free_minishell(sh);
		exit(127);
	}
}

int	starts_with_tilde(const char *s)
{
	return (s && s[0] == '~' && (s[1] == '\0' || s[1] == '/'));
}

void	check_str(char *new, t_minishell *sh)
{
	if (!new)
	{
		free_minishell(sh);
		exit(0);
	}
}

void	replace_relative(t_ast *node, t_minishell *sh)
{
	char		*old;
	char		*new;
	int			i;
	const char	*home = get_env_value("HOME", sh->envp);

	if (!home)
		ft_putendl_fd("home is not found\n", 2);
	if (!node || !node->cmd || !node->cmd->argv)
		return ;
	i = 0;
	while (node->cmd->argv[i])
	{
		old = node->cmd->argv[i];
		if (starts_with_tilde(old))
		{
			new = replace_substring(old, "~", home);
			check_str(new, sh);
			free(old);
			node->cmd->argv[i] = new;
		}
		i++;
	}
}

int	extrern_exit(int status, t_minishell *sh)
{

	if (WIFSIGNALED(status))
		sh->exit_status = 128 + WTERMSIG(status);
	else
		sh->exit_status = WEXITSTATUS(status);
	return (sh->exit_status);
}
