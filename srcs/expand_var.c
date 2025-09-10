/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lyssa <Lyssa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:37:52 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/12 21:27:35 by Lyssa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*return_exit_status(int *i, int exit_status)
{
	char	*status;

	(*i) += 2;
	status = ft_itoa(exit_status);
	if (!status)
		return (NULL);
	return (status);
}

char	*expand_existing_var(char *str, int *i, char **envp)
{
	int		start;
	char	*name;
	char	*value;
	char	*temp;

	(*i)++;
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	name = ft_substr(str, start, *i - start);
	if (!name)
		return (NULL);
	temp = get_env_value(name, envp);
	value = ft_strdup(temp);
	if (!value)
		return (NULL);
	free_and_null((void **)&name);
	return (value);
}

char	*expand_var(char *str, int *i, char **envp, int exit_status)
{
	if (str[*i] && str[*i + 1] == '?')
		return (return_exit_status(i, exit_status));
	if (str[*i] && ft_isdigit(str[*i + 1]))
	{
		(*i) += 2;
		return (ft_strdup(""));
	}
	if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
		return (expand_existing_var(str, i, envp));
	(*i)++;
	return (ft_substr(str, *i, 1));
}

char	*expand_string(char *str, char **envp, int exit_status)
{
	int		i;
	char	*result;
	char	*part;
	char	*temp;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[i])
	{
		part = get_next_part(str, &i, envp, exit_status);
		if (!part)
		{
			free_and_null((void **)&result);
			return (NULL);
		}
		temp = result;
		result = ft_strjoin(result, part);
		free_and_null((void **)&temp);
		free_and_null((void **)&part);
		if (!result)
			return (NULL);
	}
	return (result);
}

bool	expand_tokens(t_token *tokens, char **envp, int exit_status)
{
	char	*expanded;

	while (tokens)
	{
		if (tokens->type == token_word && tokens->squote == false
			&& ft_strchr(tokens->value, '$'))
		{
			expanded = expand_string(tokens->value, envp, exit_status);
			if (!expanded)
				return (false);
			if (ft_strcmp(tokens->value, expanded) != 0)
			{
				free_and_null((void **)&tokens->value);
				tokens->value = expanded;
			}
			else
				free_and_null((void **)&expanded);
		}
		tokens = tokens->next;
	}
	return (true);
}
