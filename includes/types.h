/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:47:32 by mborsuk           #+#    #+#             */
/*   Updated: 2025/08/25 21:17:16 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H
# include <stdbool.h>

typedef enum e_token_type
{
	token_word,
	token_whitespace,
	token_pipe,
	token_input,
	token_output,
	token_append,
	token_heredoc
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	bool				squote;
	bool				dquote;
	bool				whitespace;
	struct s_token		*next;
}						t_token;

typedef struct s_token_info
{
	int					start;
	int					end;
	int					quote;
	bool				whitespace;
}						t_token_info;

typedef enum e_node_type
{
	node_command,
	node_pipe
}						t_node_type;

typedef enum e_redir_type
{
	input,
	output,
	append,
	heredoc
}						t_redir_type;

typedef struct s_redirect
{
	t_redir_type		type;
	char				*file;
	int					fd;
	int					open;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_cmd
{
	char				**argv;
	t_redirect			*redirects;
}						t_cmd;

typedef struct s_ast
{
	t_node_type			type;
	struct s_ast		*left;
	struct s_ast		*right;
	t_cmd				*cmd;
}						t_ast;

typedef struct s_var
{
	char				*key;
	char				*val;
	bool				exported;
	struct s_var		*next;
}						t_var;

typedef struct s_minishell
{
	t_token				*tokens;
	t_ast				*ast;
	t_var				*var;
	char				**envp;
	char				*line;
	int					exit_status;
	int					stdin;
	int					stdout;
	int close_parent_stdout;
}						t_minishell;

typedef struct s_buffer
{
	char				*content;
	int					pos;
	int					size;
}						t_buffer;

typedef struct s_line
{
	char				*content;
	int					size;
}						t_line;

typedef struct s_process
{
	pid_t				pid1;
	pid_t				pid2;
	int					status1;
	int					status2;
	int					pipefd[2];
}						t_process;
#endif
