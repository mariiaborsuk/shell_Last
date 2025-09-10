/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalch <akovalch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:45:44 by akovalch          #+#    #+#             */
/*   Updated: 2025/08/23 17:23:44 by akovalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/Libft/libft.h"
# include "signals.h"
# include "types.h"
# include <stdbool.h>
# include <termios.h>

bool		shell_read_loop(t_minishell *shell);
void		add_node(t_token **list, t_token *new_node);
t_token		*create_node(t_token_type type, char *value);
t_redirect	*add_redirect(t_redirect *redirects, t_redirect *new_redirect);
t_ast		*create_ast_node(t_node_type type);
t_redirect	*create_redirect(t_redir_type type, char *file);
void		init_token_info(t_token_info *info);
bool		token_constructor(enum e_token_type type, t_token **token,
				char *str, t_token_info info);
bool		is_space(char c);
int			count_word_tokens(t_token *tokens);
bool		is_redirect(t_token_type type);
bool		is_wrong_redirect(t_token **tokens);
bool		is_redirect_at_end(t_token **tokens);
bool		create_token_pipe(t_token **tokens, char *str, int *i);
bool		create_token_word(t_token **tokens, char *str, int *i);
bool		create_token_dquote(t_token **tokens, char *str, int *i);
bool		create_token_squote(t_token **tokens, char *str, int *i);
bool		create_token_redirect(t_token **tokens, char *str, int *i);
bool		join_str(t_token **tokens);
bool		lexing(t_minishell *shell, char *line, char **envp);
char		*get_env_value(char *name, char **envp);
char		*get_next_part(char *str, int *i, char **envp, int exit_status);
char		*expand_var(char *str, int *i, char **envp, int exit_status);
bool		expand_tokens(t_token *tokens, char **envp, int exit_status);
bool		is_valid(t_token **tokens);
bool		parsing(t_minishell *shell);
t_cmd		*create_cmd(t_token **token);
bool		build_cmd(t_token **tokens, char ***argv_copy,
				t_redirect **redirect_copy);
bool		correct_redirect(char *str, int *i);
void		syntax_error(char *message);
void		free_and_null(void **ptr);
void		free_list(t_token **token);
void		free_redirect(t_redirect **redirects);
void		free_argv(char ***argv);
void		free_cmd(t_cmd **cmd);
void		free_ast(t_ast **ast);
void		free_minishell(t_minishell *shell);
bool		free_fun(t_minishell *shell, char **new_line);
void		reset_shell(t_minishell *shell);
bool		add_env_var(t_var **head, char *envp_row);
bool		handle_envp(t_minishell *shell);
// void		handle_start(t_minishell *shell);

#endif
