/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:30:17 by akovalch          #+#    #+#             */
/*   Updated: 2025/09/28 16:47:58 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <unistd.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

typedef struct s_buffer
{
	char	*content;
	int		pos;
	int		size;
}			t_buffer;

typedef struct s_line
{
	char	*content;
	int		size;
}			t_line;

typedef struct s_read_buffer
{
	char	*line_content;
	int		should_return;
}			t_read_buffer;

char	*get_line_loop(int fd, t_line *line, t_buffer *buffer);
void	free_buffer(t_buffer *buffer)
{
	if (buffer->content)
		free(buffer->content);
	buffer->content = NULL;
	buffer->pos = 0;
	buffer->size = 0;
}

void	free_line(t_line *line)
{
	if (line->content)
		free(line->content);
	line->content = NULL;
}

void	free_at_the_end(t_buffer *buffer, t_line *line)
{
	if (buffer->size < 0)
	{
		free_buffer(buffer);
		free_line(line);
	}
	else if (buffer->size == 0)
	{
		free_buffer(buffer);
		if (line->content[0] == '\0')
			free_line(line);
	}
}

char	*start_new_line(t_line *line, t_buffer *buffer, int i)
{
	buffer->pos = i + 1;
	return (line->content);
}
char	*get_next_line(int fd)
{
	static t_buffer	buffer;
	t_line			line;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		if (buffer.content)
			free_buffer(&buffer);
		return (NULL);
	}
	if (!buffer.content)
	{
		buffer.content = (char *) malloc((BUFFER_SIZE + 1) * sizeof(char));
		if (!buffer.content)
			return (NULL);
	}
	line.size = 0;
	line.content = malloc(2 * sizeof(char));
	if (!line.content)
	{
		free_buffer(&buffer);
		return (NULL);
	}
	line.content[0] = '\0';
	return (get_line_loop(fd, &line, &buffer));
}

char	*make_new_line(t_buffer *buffer, t_line *line)
{
	char	*new_line;
	int		j;

	new_line = malloc(line->size + 2);
	if (!new_line)
	{
		free_buffer(buffer);
		free_line(line);
		return (NULL);
	}
	j = 0;
	while (j < line->size)
	{
		new_line[j] = line->content[j];
		j++;
	}
	return (new_line);
}

int	read_new_buffer(int fd, t_buffer *buffer, t_line *line)
{
	int	should_return;

	should_return = 0;
	if (buffer->pos >= buffer->size)
	{
		buffer->size = read(fd, buffer->content, BUFFER_SIZE);
		if (buffer->size > 0)
			buffer->content[buffer->size] = '\0';
		buffer->pos = 0;
		if (buffer->size <= 0)
		{
			free_at_the_end(buffer, line);
			should_return = 1;
		}
	}
	return (should_return);
}

void	replace_line(t_line *line, t_buffer *buffer, int i, char *new_line)
{
	free_line(line);
	line->content = new_line;
	line->content[line->size++] = buffer->content[i];
	line->content[line->size] = '\0';
}

char	*get_line_loop(int fd, t_line *line, t_buffer *buffer)
{
	int			i;
	char		*new_line;
	int			should_return_line_content;

	should_return_line_content = 0;
	while (1)
	{
		should_return_line_content = read_new_buffer(fd, buffer, line);
		if (should_return_line_content)
			return (line->content);
		i = buffer->pos;
		while (i < buffer->size)
		{
			new_line = make_new_line(buffer, line);
			if (new_line == NULL)
				return (new_line);
			replace_line(line, buffer, i, new_line);
			if (buffer->content[i] == '\n')
				return (start_new_line(line, buffer, i));
			i++;
		}
		buffer->pos = buffer->size;
	}
}
