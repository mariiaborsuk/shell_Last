/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:06:14 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/13 13:57:59 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	token_constructor(enum e_token_type type, t_token **token,
		char *str, t_token_info info)
{
	char	*value;
	t_token	*new_node;

	value = ft_substr(str, info.start, info.end - info.start);
	if (!value)
		return (false);
	new_node = create_node(type, value);
	if (!new_node)
	{
		free_and_null((void **)&value);
		return (false);
	}
	if (info.quote == 1)
		new_node->squote = true;
	else if (info.quote == 2)
		new_node->dquote = true;
	if (info.whitespace == true)
		new_node->whitespace = true;
	add_node(token, new_node);
	return (true);
}

bool	join_str(t_token **tokens)
{
	t_token	*current;
	t_token	*next;
	char	*temp;

	current = *tokens;
	while (current && current->next)
	{
		if (current->type == token_word && current->next->type == token_word
			&& !current->whitespace)
		{
			temp = current->value;
			next = current->next;
			current->value = ft_strjoin(current->value, next->value);
			if (!current->value)
				return (false);
			current->next = next->next;
			free_and_null((void **)&next->value);
			free_and_null((void **)&next);
			free_and_null((void **)&temp);
		}
		else
			current = current->next;
	}
	return (true);
}

bool	handle_token(t_token **tokens, char *str, int *i)
{
	if (is_space(str[*i]))
	{
		(*i)++;
		return (true);
	}
	else if (str[*i] == '|')
		return (create_token_pipe(tokens, str, i));
	else if (str[*i] == '\"')
		return (create_token_dquote(tokens, str, i));
	else if (str[*i] == '\'')
		return (create_token_squote(tokens, str, i));
	else if (str[*i] == '>' || str[*i] == '<')
		return (create_token_redirect(tokens, str, i));
	else
		return (create_token_word(tokens, str, i));
}

bool	lexing(t_minishell *shell, char *line, char **envp)
{
	int		i;

	i = 0;
	if (!line)
		return (false);
	while (line[i])
	{
		if (!(handle_token(&shell->tokens, line, &i)))
			return (false);
	}
	if (!expand_tokens(shell->tokens, envp, shell->exit_status)
		|| !join_str(&shell->tokens))
	{
		free_list(&shell->tokens);
		return (false);
	}
	if (!is_valid(&shell->tokens))
	{
		free_list(&shell->tokens);
		return (false);
	}
	return (true);
}
