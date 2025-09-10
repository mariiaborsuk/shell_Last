/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lyssa <Lyssa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:44:49 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/12 21:26:45 by Lyssa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(char *name, char **envp)
{
	int	i;
	int	length;

	i = 0;
	length = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, length) == 0 && envp[i][length] == '=')
			return (envp[i] + length + 1);
		i++;
	}
	return ("");
}

char	*get_next_part(char *str, int *i, char **envp, int exit_status)
{
	if (str[*i] && str[*i] == '$' && str[*i + 1]
		&& (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_'
			|| str[*i + 1] == '?'))
		return (expand_var(str, i, envp, exit_status));
	else
		return (ft_substr(str, (*i)++, 1));
}
