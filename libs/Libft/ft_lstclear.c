/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalch <akovalch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 15:25:00 by akovalch          #+#    #+#             */
/*   Updated: 2025/02/17 18:36:39 by akovalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*node;
	t_list	*nextnode;

	node = *lst;
	nextnode = *lst;
	while (node != NULL)
	{
		nextnode = nextnode->next;
		del(node->content);
		free(node);
		node = nextnode;
	}
	*lst = NULL;
}
