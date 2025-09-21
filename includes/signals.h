/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:47:08 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/19 23:43:25 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H
# include "../includes/types.h"
# include <errno.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <unistd.h>
// # define CHILD_RUNNING 1u
# define NEED_REDISPLAY 2u
// # define GOT_SIGINT 4u
// # define IN_HEREDOC 1
#define CHILD_RUNNING  (1u << 0)
#define GOT_SIGINT     (1u << 1)

void		parent_sigint(int signum);
void		handle_sigint(int signum);
void		install_parent_signals(void);
void		child_running(int *status, pid_t pid);
void		if_g_state(t_minishell *shell);
void		set_signals_interactive(void);
void		signal_reset_prompt(int signo);
void		signal_print_newline(int signal);
void		ignore_sigquit(void);
extern int	g_state;
extern int global;

#endif
