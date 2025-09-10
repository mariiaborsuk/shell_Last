/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lyssa <Lyssa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:37:27 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/12 19:53:22 by Lyssa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/errors.h"

bool	is_pipe_at_start_end(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	if (current && current->type == token_pipe)
		return (true);
	while (current && current->next)
		current = current->next;
	if (current && current->type == token_pipe)
		return (true);
	return (false);
}

bool	is_double_pipe(t_token **tokens)
{
	t_token	*current;
	bool	is_pipe;

	current = *tokens;
	is_pipe = false;
	while (current)
	{
		if (current->type == token_pipe)
		{
			if (is_pipe)
				return (true);
			is_pipe = true;
		}
		else
			is_pipe = false;
		current = current->next;
	}
	return (false);
}

bool	is_valid(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (false);
	if (is_pipe_at_start_end(tokens) || is_double_pipe(tokens))
	{
		syntax_error(PIPE_ERR);
		return (false);
	}
	if (is_redirect_at_end(tokens) || is_wrong_redirect(tokens))
		return (false);
	return (true);
}
