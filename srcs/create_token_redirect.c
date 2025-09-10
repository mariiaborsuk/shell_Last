/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_redirect.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lyssa <Lyssa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 12:35:15 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/12 19:49:29 by Lyssa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/errors.h"

bool	handle_output(t_token **tokens, char *str, int *i, t_token_info info)
{
	if (str[*i] == '>' && str[*i + 1] != '>')
	{
		(*i)++;
		return (token_constructor(token_output, tokens, str, info));
	}
	return (false);
}

bool	handle_input(t_token **tokens, char *str, int *i, t_token_info info)
{
	if (str[*i] == '<' && str[*i + 1] != '<')
	{
		(*i)++;
		return (token_constructor(token_input, tokens, str, info));
	}
	return (false);
}

bool	handle_append(t_token **tokens, char *str, int *i, t_token_info info)
{
	if (str[*i] == '>' && str[*i + 1] == '>')
	{
		if (correct_redirect(str, i))
		{
			info.end += 2;
			*i += 2;
			return (token_constructor(token_append, tokens, str, info));
		}
	}
	return (false);
}

bool	handle_heredoc(t_token **tokens, char *str, int *i, t_token_info info)
{
	if (str[*i] == '<' && str[*i + 1] == '<')
	{
		if (correct_redirect(str, i))
		{
			info.end += 2;
			*i += 2;
			return (token_constructor(token_heredoc, tokens, str, info));
		}
	}
	return (false);
}

bool	create_token_redirect(t_token **tokens, char *str, int *i)
{
	t_token_info	info;

	init_token_info(&info);
	info.start = *i;
	info.end = *i + 1;
	if (str[*i] == '>')
	{
		if (str[*i + 1] == '>')
			return (handle_append(tokens, str, i, info));
		else
			return (handle_output(tokens, str, i, info));
	}
	else if (str[*i] == '<')
	{
		if (str[*i + 1] == '<')
			return (handle_heredoc(tokens, str, i, info));
		else
			return (handle_input(tokens, str, i, info));
	}
	return (false);
}
