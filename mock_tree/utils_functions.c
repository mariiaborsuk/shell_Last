/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 14:46:12 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/26 12:00:09 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/minishell.h"
#include "../includes/types.h"

void	dup_file(int rfd)
{
	if (rfd != -1)
	{
		if (dup2(rfd, STDIN_FILENO) == -1)
		{
			perror("dup_file failed");
		}
		close(rfd);
	}
}
