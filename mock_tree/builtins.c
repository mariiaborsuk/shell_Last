/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:53:30 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/23 14:16:45 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

int	handle_exit(char **cmd, t_minishell *shell)
{
	int	exit_status;

	exit_status = 0;
	if (argv_len(cmd) == 2)
	{
		if (cmd[1] && ft_atoi(cmd[1]))
			exit_status = ft_atoi(cmd[1]) & 0xFF;
	}
	if (argv_len(cmd) >= 2 && !ft_atoi(cmd[1]))
	{
		ft_putendl_fd(" numeric argument required", 2);
		free_minishell(shell);
		exit(2);
	}
	if (argv_len(cmd) > 2)
	{
		ft_putendl_fd(" too many arguments\n", 2);
		return (1);
	}
	free_minishell(shell);
	exit(exit_status);
}

int	skip_n(char **cmds, int i)
{
	while (cmds[i])
	{
		if ((cmds[i][0] && cmds[i][0] != '-') || check_n(cmds[i]) == 0)
		{
			break ;
		}
		i++;
	}
	return (i);
}

int	print_echo(int index, char **cmds)
{
	int	i;
	int	len;

	len = argv_len(cmds);
	i = skip_n(cmds, index);
	while (cmds[i])
	{
		printf("%s", cmds[i]);
		if (i < len - 1)
		{
			printf(" ");
		}
		i++;
	}
	return (0);
}

int	exec_echo(char **argv)
{
	int	i;

	if (!argv[1])
	{
		if (printf("\n") < 0)
			return (1);
		return (0);
	}
	if (argv[1][0] == '-' && check_n(&argv[1][0]))
	{
		i = 2;
		print_echo(i, argv);
		return (0);
	}
	else
	{
		i = 1;
		if (print_echo(i, argv) != 0)
			return (1);
		if (printf("\n") < 0)
			return (1);
		return (0);
	}
}

int	print_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "COLUMNS=", 8) != 0 && ft_strncmp(envp[i],
				"LINES=", 6) != 0)
		{
			if (printf("%s\n", envp[i]) < 0)
				return (1);
		}
		i++;
	}
	return (0);
}
