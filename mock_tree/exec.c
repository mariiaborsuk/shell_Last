/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:38:00 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/28 19:23:30 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/errors.h"
#include "../includes/execute.h"
#include "../includes/minishell.h"
#include "../libs/Libft/libft.h"

void	strjoin_1(char **s1, const char *s2)
{
	char	*result;
	int		i;
	int		j;
	int		len1;
	int		len2;

	len1 = ft_strlen(*s1);
	len2 = ft_strlen(s2);
	result = malloc((len1 + len2 + 1) * sizeof(char));
	if (!result)
	{
		free(*s1);
		*s1 = NULL;
		return ;
	}
	i = -1;
	while (++i < len1)
		result[i] = (*s1)[i];
	j = -1;
	while (++j < len2)
		result[i + j] = s2[j];
	result[i + j] = '\0';
	free(*s1);
	*s1 = result;
}

char	**get_paths(char *envp[])
{
	char	*paths_dirs;
	char	**paths;

	paths_dirs = find_path(envp);
	paths = ft_split(paths_dirs + 5, ':');
	if (paths == NULL)
		return (NULL);
	return (paths);
}

char	*find_path(char *envp[])
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i]);
		i++;
	}
	return (NULL);
}

char	*check_cmd(char **paths, char *cmd)
{
	int	i;

	i = 0;
	if (cmd == NULL || (cmd[0] == '\0' || cmd[0] == ' '))
		return (NULL);
	if (cmd != NULL && ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0)
			return (cmd);
	}
	while (paths[i] != NULL)
	{
		strjoin_1(&paths[i], "/");
		strjoin_1(&paths[i], cmd);
		if (access(paths[i], F_OK) == 0 && access(paths[i], X_OK) == 0)
			return (paths[i]);
		i++;
	}
	return (NULL);
}

void	use_execve(char **ar, char *envp[], t_minishell *shell)
{
	char	*path_env;
	char	**paths_ar;
	char	*path;

	if (ar && (ar[0][0] == '\0' || ar[0][0] == ' '))
		cmd_not_found(ar[0], shell, ": command not found\n");
	path_env = find_path(envp);
	if (!path_env)
		cmd_not_found(ar[0], shell, ": command not found\n");
	paths_ar = ft_split(path_env, ':');
	if (!paths_ar)
		cmd_not_found(ar[0], shell, ": command not found\n");
	path = check_cmd(paths_ar, ar[0]);
	if (path == NULL)
	{
		free_argv(&paths_ar);
		cmd_not_found(ar[0], shell, ": command not found\n");
	}
	if (execve(path, ar, envp) == -1)
	{
		perror("execve");
		free_minishell(shell);
		exit(126);
	}
}
