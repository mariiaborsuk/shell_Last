/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_out.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:10:29 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/25 15:24:38 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"

int	open_std_out(t_redirect **f_str)
{
		if (access((*f_str)->file, F_OK) == 0)
	{
		if (access((*f_str)->file, W_OK) != 0)
		{
			perror("Permission denied: file not writable");
			return (1);
		}
	}
	(*f_str)->fd = open((*f_str)->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if ((*f_str)->fd < 0)
	{
		perror("Error opening file");
		return (1);
	}
	if (dup2((*f_str)->fd, STDOUT_FILENO) == -1)
	{
		close((*f_str)->fd);
		perror("error\n DUP2\n");
		(*f_str)->fd = -1;
		return (1);
	}
	(*f_str)->open = 1;
	return (0);
}

int	open_std_in(t_redirect **f_str)
{
	if (!f_str || !*f_str || !(*f_str)->file)
	{
		perror("Invalid input redirection\n");
		return (1);
	}
	(*f_str)->fd = open((*f_str)->file, O_RDONLY);
	if ((*f_str)->fd < 0)
	{
		perror("open");
		return (1);
	}
	if ((*f_str)->fd != STDIN_FILENO)
	{
		if (dup2((*f_str)->fd, STDIN_FILENO) == -1)
		{
			close((*f_str)->fd);
			perror("dup2");
			return (1);
		}
	}
	(*f_str)->open = 1;
	return (0);
}

int	redirect_stdout_append(t_redirect **f_str)
{
			if (f_str == NULL || (*f_str)->file == NULL)
	{
		perror("Invalid file name\n");
		return (1);
	}
	if (access((*f_str)->file, F_OK) == 0)
	{
		if (access((*f_str)->file, W_OK) != 0)
		{
			perror("Permission denied: file not writable");
			return (1);
		}
	}

	(*f_str)->fd = open((*f_str)->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if ((*f_str)->fd < 0)
	{
		perror("Error opening file for append");
		return (1);
	}
	if (dup2((*f_str)->fd, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting stdout");
		close((*f_str)->fd);
		(*f_str)->fd = -1;
		return (1);
	}
	(*f_str)->open = 1;
	return (0);
}
