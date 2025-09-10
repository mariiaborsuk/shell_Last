/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 22:09:44 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/26 12:35:04 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

int	get_old_dir(void)
{
	char	*old_value;

	old_value = getenv("OLDPWD");
	if (old_value)
	{
		printf("Old pwd %s\n", old_value);
		if (chdir(old_value) != 0)
			return (1);
		return (0);
	}
	else
	{
		perror("Old pwd is not found\n");
		return (1);
	}
}

int	get_dir(char **argv)
{
	struct stat	info;

	if (stat(argv[1], &info) == 0)
	{
		if (S_ISDIR(info.st_mode))
		{
			if (chdir(argv[1]) != 0)
				return (1);
			return (0);
		}
		else
		{
			ft_putendl_fd(" No such file or directory", 2);
			return (1);
		}
	}
	else
	{
		ft_putendl_fd(" No such file or directory", 2);
		return (1);
	}
}

int	cd_fns(char **argv)
{
	int	result;

	result = 0;
	if (argv_len(argv) == 1)
	{
		result = 0;
	}
	else if (argv_len(argv) > 2)
		result = (ft_putendl_fd(" too many arguments", 2), 1);
	else if (ft_strcmp("-", argv[1]) == 0)
		result = (get_old_dir());
	else if (ft_strcmp(".", argv[1]) == 0)
		result = 0;
	else if (ft_strlen(argv[1]) > 1)
		result = (get_dir(argv));
	return (result);
}

int	cmd_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd\n");
		return (1);
	}
}

int	check_n(char *str)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
		{
			return (0);
		}
		i++;
	}
	return (1);
}
