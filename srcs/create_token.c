/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lyssa <Lyssa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:50:24 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/12 20:05:47 by Lyssa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/errors.h"

void	init_token_info(t_token_info *info)
{
	info->start = 0;
	info->end = 0;
	info->quote = 0;
	info->whitespace = false;
}

bool	create_token_pipe(t_token **tokens, char *str, int *i)
{
	t_token_info	info;

	init_token_info(&info);
	info.start = *i;
	if (str[*i] && str[*i] == '|')
		(*i)++;
	info.end = *i;
	return (token_constructor(token_pipe, tokens, str, info));
}

bool	create_token_word(t_token **tokens, char *str, int *i)
{
	t_token_info	info;

	init_token_info(&info);
	info.start = *i;
	while (str[*i] && !(is_space(str[*i])) && str[*i] != '|'
		&& str[*i] != '\'' && str[*i] != '\"'
		&& str[*i] != '>' && str[*i] != '<')
		(*i)++;
	if (is_space(str[*i]))
		info.whitespace = true;
	info.end = *i;
	return (token_constructor(token_word, tokens, str, info));
}

bool	create_token_dquote(t_token **tokens, char *str, int *i)
{
	t_token_info	info;

	init_token_info(&info);
	(*i)++;
	info.start = *i;
	while (str[*i] && str[*i] != '\"')
		(*i)++;
	if (!str[*i])
	{
		ft_putendl_fd(DQUOTE_ERR, 2);
		return (false);
	}
	if (is_space(str[*i + 1]))
		info.whitespace = true;
	info.end = (*i)++;
	info.quote = 2;
	return (token_constructor(token_word, tokens, str, info));
}

bool	create_token_squote(t_token **tokens, char *str, int *i)
{
	t_token_info	info;

	init_token_info(&info);
	(*i)++;
	info.start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (!str[*i])
	{
		ft_putendl_fd(SQUOTE_ERR, 2);
		return (false);
	}
	if (is_space(str[*i + 1]))
		info.whitespace = true;
	info.end = (*i)++;
	info.quote = 1;
	return (token_constructor(token_word, tokens, str, info));
}
