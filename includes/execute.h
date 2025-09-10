/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 22:23:59 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/10 17:33:56 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H
# include "../includes/errors.h"
# include "../includes/minishell.h"
# include "../includes/types.h"
# include <ctype.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

void		strjoin1(char **s1, const char *s2);
char		**get_paths(char *envp[]);
char		*find_path(char *envp[]);
void		use_execve(char **ar, char *envp[], t_minishell *shell);
bool		execute_ast(t_ast *node, t_var **var, t_minishell *shell);
// int			manage_heredoc(int fd, char *del, t_minishell *shell);
int			handle_redir(t_ast *node, t_var *var, t_minishell *shell);
bool		execute_pipe(t_ast *node, t_var *var, t_minishell *shell);
int			cmd_len(char **cmd);
t_var		*check_var(t_var **head, char *key);
int			open_std_out(t_redirect **f_str);
char		*replace_substring(const char *str, const char *old_sub,
				const char *new_sub);
t_var		*make_var_from_envp(char *envp_row);
int			open_std_in(t_redirect **f_str);
int			redirect_stdout_append(t_redirect **f_str);
int			get_home_dir(void);
int			cd_fns(char **argv);
int			print_env(char **envp);
int			cmd_pwd(void);
int			argv_len(char **argv);
int			print_echo(int index, char **cmds);
int			exec_echo(char **argv);
void		print_exported_vars(t_var **var);
bool		is_valid_export_var_name(const char *var_name);
int			handle_exported_var(const t_var *exported_var, t_var **var);
int			exec_export(char **cmd, t_var **var);
int			handle_unset_var(char *var_name, t_var **var);
bool		is_valid_unset_var_name(const char *var_name);
int			exec_unset(char **cmd, t_var **var);
int			extern_cmd(char **argv, char **envp, t_minishell *sh);
void		close_files(t_redirect *head);
int			get_var_count(t_var *var);
char		**convert_var_to_envp(t_var **var);
void		free_var(t_var *var);
void		remove_var(char *key, t_var **head);
void		free_var_list(t_var **head);
t_redirect	*get_files(t_redirect *list);
void		free_files(t_redirect *head);
int			handle_exit(char **cmd, t_minishell *shell);
int			execute_built_in(char **cmd, t_var **var, t_minishell *shell);
int			is_built_in(char **cmd);
char		*substring_until_ptr(const char *str, const char *end_ptr);
void		expand_variable(t_var *head, char **v_name);
int			count_dollar(char *cmd);
void		get_val(char **name, t_var *head);
int			count_var(char **cmd);
void		delete_var(t_ast **node);
void		replace_var(t_ast **node, t_var **head_var);
int			handle_commands(t_var *var, t_minishell *shell,
				t_redirect *original_head, t_ast *node);
void		execute_cmd(t_ast *node, t_var **var, t_minishell *shell);
void		manage_cmd(t_ast *node, t_var *var, t_minishell *shell);
int			check_quote(t_token *tokens);
void		handle_status(int status, pid_t pid, t_minishell *shell);
char		*ft_implode(char **array);
void		get_cur_val(t_var *head, char *var_name, char **name,
				char *replace);
void		free_and_replace(char **name, char *replace, char *val,
				char *var_name);
char		*get_replace(const char *name);
char		*get_var_name(const char *name);
void		modify_result(const char *str, const char *old_sub, char *result,
				const char *new_sub);
void		modify_old_var(char *val, t_var *old_var, t_var **head);
t_var		*add_new_var(char *val, char *key, bool exported);
void		free_execve(char *ar, t_minishell *shell);
void		exec_and_exit(char **argv, t_minishell *sh, char **envp);
void		perror_exit(char *er, t_minishell *sh, int num);
char		*remove_braces(char *name);
void		add_var(const t_var *exported_var, t_var **head);
void		empty_argv(char **argv, t_minishell *sh);
int			starts_with_tilde(const char *s);
void		check_str(char *new, t_minishell *sh);
void		replace_relative(t_ast *node, t_minishell *sh);
char		*if_heredoc(t_cmd *cmd);
void		rfd_invalid(t_minishell *shell);
void		replace_heredoc_val(t_var *var, char *str);
void		put_line(char *line, int fd);
int			extrern_exit(int status, t_minishell *sh);
void		her_exit(int fd, t_minishell *shell);
void		unlink_close(int rfd);
int			get_temp_fd(char *heredoc_del, t_minishell *shell, int *st);
void		restore_saved_fds(t_minishell *shell);
void		check_files(t_cmd *cmd, t_minishell *sh, t_redirect *redirect);
void		dup_file(int rfd);
void		restore_fds(int save_stdin, int save_stdout);
void		dup_files(t_minishell *shell, t_ast *node);
int			check_n(char *str);
void		cmd_not_found(char *str, t_minishell *shell, char *er);
void		node_with_redir(t_minishell *shell, t_ast *node, t_var **var);
// int	prepare_heredocs(t_redirect *rlist, t_minishell *sh, int *out_fd);
int			prepare_heredocs(t_redirect *rlist, t_minishell *sh);
int create_heredoc_pipe(char *delimiter, t_minishell *shell, int *status);
int	check_heredoc(t_redirect *head, t_minishell *shell);

#endif
