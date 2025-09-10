/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lyssa <Lyssa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:18:37 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/12 19:52:47 by Lyssa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/errors.h"

bool	is_space(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (true);
	return (false);
}

bool	is_redirect(t_token_type type)
{
	if (type == token_input || type == token_output
		|| type == token_append || type == token_heredoc)
		return (true);
	return (false);
}

int	count_word_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	if (!tokens)
		return (0);
	while (tokens && tokens->type != token_pipe)
	{
		if (is_redirect(tokens->type))
		{
			tokens = tokens->next;
			if (tokens && tokens->type == token_word)
				tokens = tokens->next;
		}
		if (tokens && tokens->type == token_word)
			count++;
		if (tokens)
			tokens = tokens->next;
	}
	return (count);
}

void	syntax_error(char *message)
{
	ft_putstr_fd(SYNTAX_ERR, 2);
	ft_putstr_fd(message, 2);
	ft_putendl_fd("'", 2);
}
