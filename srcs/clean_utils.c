/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:37:17 by akovalch          #+#    #+#             */
/*   Updated: 2025/09/16 20:48:53 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/minishell.h"

void	free_list(t_token **token)
{
	t_token	*temp;
	t_token	*next_node;

	if (!token || !*token)
		return ;
	temp = *token;
	while (temp)
	{
		next_node = temp->next;
		free_and_null((void **)&temp->value);
		free_and_null((void **)&temp);
		temp = next_node;
	}
	*token = NULL;
}

// void	free_redirect(t_redirect **redirects)
// {
// 	t_redirect	*temp;
// 	t_redirect	*cur;

// 	if (!redirects || !*redirects)
// 		return ;
// 	cur = *redirects;
// 	while (cur)
// 	{
// 		temp = cur->next;
// 		if (cur->fd != -1 && cur->open == 1 && cur->fd > 2)
// 		{
// 			close(cur->fd);
// 			cur->open = 0;
// 			cur->fd = -1;
// 		}
// 		if (cur->file)
// 			free_and_null((void **)&cur->file);
// 		free_and_null((void **)&cur);
// 		cur = temp;
// 	}
// }
void free_redirect(t_redirect **redirect)
{
    t_redirect *current = *redirect;  // Get the actual list head
    t_redirect *next;

    // printf("DEBUG: free_redirect called with redirect=%p, *redirect=%p\n",
    //        (void*)redirect, (void*)*redirect);

    while (current)
    {
        // // printf("DEBUG: Freeing redirect %p, next=%p\n",
        //        (void*)current, (void*)current->next);

        // Add bounds checking
        if ((uintptr_t)current < 0x1000)
        {
            // printf("ERROR: Corrupted redirect pointer detected: %p\n", (void*)current);
            break;
        }

        if (current->next && (uintptr_t)current->next < 0x1000)
        {
            // printf("ERROR: Corrupted next pointer detected: %p\n", (void*)current->next);
            break;
        }

        next = current->next;     // Now this works correctly
        if (current->file)
            free(current->file);
        free(current);
        current = next;
    }

    *redirect = NULL;  // Clear the original pointer
}

void	free_argv(char ***argv)
{
	int		i;
	char	**temp;

	i = 0;
	if (!argv)
		return ;
	temp = *argv;
	while (temp[i])
	{
		free_and_null((void **)&temp[i]);
		i++;
	}
	free(temp);
	temp = NULL;
	*argv = NULL;
}

void	free_cmd(t_cmd **cmd)
{
	t_cmd	*temp;

	if (!cmd || !*cmd)
		return ;
	temp = *cmd;
	if (temp->argv)
		free_argv(&temp->argv);
	if (temp->redirects)
		free_redirect(&temp->redirects);
	free_and_null((void **)cmd);
}

void	free_ast(t_ast **ast)
{
	t_ast	*node;

	if (!ast || !*ast)
		return ;
	node = *ast;
	if (node->type == node_command)
		free_cmd(&node->cmd);
	if (node->left)
		free_ast(&node->left);
	if (node->right)
		free_ast(&node->right);
	free_and_null((void **)&node);
	*ast = NULL;
}
