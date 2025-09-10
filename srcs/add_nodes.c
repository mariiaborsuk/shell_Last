/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_nodes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalch <akovalch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:53:23 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/23 13:42:46 by akovalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/execute.h"

void	add_node(t_token **list, t_token *new_node)
{
	t_token	*current;

	if (!(*list))
		*list = new_node;
	else
	{
		current = *list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

t_redirect	*add_redirect(t_redirect *redirects, t_redirect *new_redirect)
{
	t_redirect	*current;

	if (!redirects)
		return (new_redirect);
	current = redirects;
	while (current->next)
		current = current->next;
	current->next = new_redirect;
	return (redirects);
}

bool	add_env_var(t_var **head, char *envp_row)
{
	t_var	*new_var;
	t_var	*current;

	if (!envp_row)
		return (false);
	new_var = make_var_from_envp(envp_row);
	if (!new_var)
		return (false);
	if (!(*head))
		*head = new_var;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_var;
	}
	return (true);
}
