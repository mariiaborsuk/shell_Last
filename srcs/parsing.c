/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lyssa <Lyssa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:24:11 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/12 21:26:12 by Lyssa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/errors.h"

bool	find_redirect(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current && current->type != token_pipe)
	{
		if (is_redirect(current->type))
			return (true);
		current = current->next;
	}
	return (false);
}

t_ast	*parse_cmd(t_token **tokens)
{
	t_cmd	*cmd;
	t_ast	*node;

	cmd = create_cmd(tokens);
	if (!cmd)
		return (NULL);
	node = create_ast_node(node_command);
	if (!node)
	{
		free_cmd(&cmd);
		return (NULL);
	}
	node->cmd = cmd;
	return (node);
}

t_ast	*parse_line(t_token **tokens)
{
	t_ast	*node;
	t_ast	*left;
	t_ast	*right;

	left = parse_cmd(tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == token_pipe)
	{
		*tokens = (*tokens)->next;
		right = parse_cmd(tokens);
		if (!right)
			return (free_ast(&left), NULL);
		node = create_ast_node(node_pipe);
		if (!node)
		{
			free_ast(&left);
			free_ast(&right);
			return (NULL);
		}
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}

bool	parsing(t_minishell *shell)
{
	t_ast	*ast;
	t_token	*temp;

	temp = shell->tokens;
	ast = parse_line(&temp);
	if (!ast)
		return (false);
	shell->ast = ast;
	return (true);
}
