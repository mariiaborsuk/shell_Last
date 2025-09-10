/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalch <akovalch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:36:04 by akovalch          #+#    #+#             */
/*   Updated: 2025/06/12 10:19:15 by akovalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# define SYNTAX_ERR "minishell: syntax error near unexpected token `"
# define DQUOTE_ERR "minishell: syntax error: unclosed double quote"
# define SQUOTE_ERR "minishell: syntax error: unclosed single quote"
# define PIPE_ERR "|"
# define INPUT_ERR "<"
# define OUTPUT_ERR ">"
# define APPEND_ERR ">>"
# define HEREDOC_ERR "<<"
# define NEWLINE_ERR "newline"

#endif