/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:23:46 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/21 02:47:16 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libs/Libft/libft.h"

t_redir_type	get_redirect_type(t_token_type type)
{
	if (type == token_input)
		return (input);
	else if (type == token_output)
		return (output);
	else if (type == token_append)
		return (append);
	else
		return (heredoc);
}

bool	handle_redirect(t_token **tokens, t_redirect **redirects)
{
	t_redirect		*new;
	t_redir_type	type;
	char			*file;

	if (!*tokens || !(*tokens)->next)
		return (false);
	type = get_redirect_type((*tokens)->type);
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != token_word)
		return (false);
	file = ft_strdup((*tokens)->value);
	if (!file)
		return (false);
	new = create_redirect(type, file);
	if (!new)
		return (free_and_null((void **)&file), false);
	*redirects = add_redirect(*redirects, new);
	*tokens = (*tokens)->next;
	return (true);
}

bool	handle_cmd(t_token **tokens, t_redirect **redirects,
	char **argv, int *i)
{
	if (is_redirect((*tokens)->type))
	{
		if (!handle_redirect(tokens, redirects))
			return (false);
	}
	else if ((*tokens)->type == token_word && *(*tokens)->value != '\0')
	{
		argv[*i] = ft_strdup((*tokens)->value);
		if (!argv[*i])
			return (false);
		(*i)++;
		*tokens = (*tokens)->next;
	}
	else
		*tokens = (*tokens)->next;
	return (true);
}

bool	build_cmd(t_token **tokens, char ***argv_copy,
	t_redirect **redirect_copy)
{
	t_redirect	*redirect;
	char		**argv;
	int			size;
	int			i;

	i = 0;
	redirect = NULL;
	size = count_word_tokens(*tokens);
	argv = ft_calloc(size + 1, sizeof(char *));
	if (!argv)
		return (false);
	while (*tokens && (*tokens)->type != token_pipe)
	{
		if (!handle_cmd(tokens, &redirect, argv, &i))
		{
			free_argv(&argv);
			return (false);
		}
	}
	argv[i] = NULL;
	*argv_copy = argv;
	*redirect_copy = redirect;
	return (true);
}
