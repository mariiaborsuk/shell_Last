/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_nodes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:53:23 by akovalch          #+#    #+#             */
/*   Updated: 2025/09/28 20:22:50 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>

t_token	*create_node(t_token_type type, char *value)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->value = value;
	new_node->squote = false;
	new_node->dquote = false;
	new_node->whitespace = false;
	new_node->next = NULL;
	return (new_node);
}
// bool first_token_is_comment(t_token *tokens)
// {
//     if (!tokens)
//         return false;

//     // Check if first token is a word token that starts with '#'
//     if (tokens->type == token_word && tokens->value && tokens->value[0] == '#')
//         return true;

//     return false;
// }
bool first_token_is_comment(t_token *tokens)
{
    t_token *current;

    if (!tokens)
        return false;

    current = tokens;

    // Skip to first non-null token if needed
    while (current && !current->value)
        current = current->next;

    if (!current)
        return false;

    // Check if it's a word token starting with '#'
    if (current->type == token_word &&
        current->value &&
        current->value[0] == '#')
        return true;

    return false;
}
t_cmd	*create_cmd(t_token **tokens)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	if (first_token_is_comment(*tokens)) {
    // Don't build command - it's a comment line
    // shell->exit_status = 0;
    return NULL; // or however you indicate no command
}
	if (!build_cmd(tokens, &cmd->argv, &cmd->redirects))
	{
		free_cmd(&cmd);
		return (NULL);
	}
	return (cmd);
}

t_ast	*create_ast_node(t_node_type type)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->cmd = NULL;
	return (node);
}

t_redirect	*create_redirect(t_redir_type type, char *file)
{
	t_redirect	*new_redirect;

	new_redirect = malloc(sizeof(t_redirect));
	if (!new_redirect)
		return (false);
	new_redirect->type = type;
	new_redirect->file = file;
	new_redirect->next = NULL;
	new_redirect->open = 0;
	new_redirect->fd = -1;
	 new_redirect->collected = false;
	return (new_redirect);
}
