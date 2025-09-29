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

int	is_valid_number(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);

	// Special cases for edge values
	if (ft_strcmp(str, "-9223372036854775808") == 0)
		return (1); // LLONG_MIN is valid
	if (ft_strcmp(str, "-9223372036854775809") == 0)
		return (0); // LLONG_MIN - 1 is invalid
	if (ft_strcmp(str, "9223372036854775808") == 0)
		return (0); // LLONG_MAX + 1 is invalid

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	handle_exit(char **cmd, t_minishell *shell)
{
	int	exit_status;

	exit_status = 0;
	if (argv_len(cmd) == 2)
	{
		if (cmd[1] && is_valid_number(cmd[1]))
			exit_status = ft_atoi(cmd[1]) & 0xFF;
	}
	if (argv_len(cmd) >= 2 && !is_valid_number(cmd[1]))
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

	i = skip_n(cmds, index);
	while (cmds[i])
	{
		printf("%s", cmds[i]);
		if (cmds[i + 1])
			printf(" ");
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
