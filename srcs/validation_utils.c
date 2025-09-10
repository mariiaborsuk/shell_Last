/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lyssa <Lyssa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:37:27 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/12 19:53:11 by Lyssa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/errors.h"

bool	is_redirect_at_end(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current && current ->next)
		current = current->next;
	if (current && is_redirect(current->type))
		return (syntax_error(NEWLINE_ERR), true);
	return (false);
}

void	redirect_error(t_token **tokens)
{
	if ((*tokens)->type == token_output)
		syntax_error(OUTPUT_ERR);
	else if ((*tokens)->type == token_append)
		syntax_error(APPEND_ERR);
	else if ((*tokens)->type == token_input)
		syntax_error(INPUT_ERR);
	else
		syntax_error(HEREDOC_ERR);
}

bool	correct_redirect(char *str, int *i)
{
	if (str[*i + 2] == '>' && str[*i + 3] != '>')
		return (syntax_error(OUTPUT_ERR), false);
	else if (str[*i + 2] == '>' && str[*i + 3] == '>' && str[*i + 4] != '>')
		return (syntax_error(APPEND_ERR), false);
	else if (str[*i + 2] == '<' && str[*i + 3] != '<')
		return (syntax_error(INPUT_ERR), false);
	else if (str[*i + 2] == '<' && str[*i + 3] == '<' && str[*i + 4] != '<')
		return (syntax_error(HEREDOC_ERR), false);
	return (true);
}

bool	is_wrong_redirect(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current)
	{
		if (is_redirect(current->type))
		{
			current = current->next;
			if (!current || current->type != token_word)
			{
				if (current->type == token_pipe)
				{
					syntax_error(PIPE_ERR);
				}
				else
					redirect_error(tokens);
				return (true);
			}
		}
		else
			current = current->next;
	}
	return (false);
}
