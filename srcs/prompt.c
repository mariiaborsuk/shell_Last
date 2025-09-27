/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:56:21 by akovalch          #+#    #+#             */
/*   Updated: 2025/09/27 21:46:08 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/minishell.h"
#include "../includes/signals.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
int global=0;

// void	install_parent_signals(void)
// {
// 	struct sigaction	sa;
// 	struct termios		t;

// 	ft_memset(&sa, 0, sizeof(sa));
// 	sa.sa_handler = parent_sigint;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	sigaction(SIGINT, &sa, NULL);
// 	sa.sa_handler = SIG_IGN;
// 	sigaction(SIGQUIT, &sa, NULL);
// 	if (tcgetattr(STDIN_FILENO, &t) == 0)
// 	{
// 		// t.c_lflag &= ~ECHOCTL;
// 		tcsetattr(STDIN_FILENO, TCSANOW, &t);
// 	}
// }
void	install_parent_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = parent_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
	// Remove the terminal modification - let ECHOCTL remain enabled
}



bool collect_heredocs_for_command(t_redirect *redirects, t_minishell *shell)
{
    t_redirect *current = redirects;

    while (current)
    {
        if (current->type == heredoc && !current->collected)
        {
            int status;

            current->fd = create_heredoc_pipe(current->file, shell, &status);

            if (current->fd == -1 || status != 0)
            {
                // Set global status to indicate interruption
                global = status;
				shell->exit_status=status;
                return false;
            }
            current->collected = true;
			shell->exit_status=status;
            global = status;
        }
        current = current->next;
    }

    return true;
}

bool collect_all_heredocs(t_ast *root, t_minishell *shell)
{
    if (!root)
        return true;

    // Collect heredocs in left-to-right, depth-first order
    if (root->left && !collect_all_heredocs(root->left, shell))
        return false;

    if (root->type == node_command && root->cmd && root->cmd->redirects)
    {
        if (!collect_heredocs_for_command(root->cmd->redirects, shell))
            return false;
    }

    if (root->right && !collect_all_heredocs(root->right, shell))
        return false;

    return true;
}

bool shell_read_loop(t_minishell *shell)
{
    rl_catch_signals = 0;
    install_parent_signals();

    while (1)
    {
        if_g_state(shell);

        if (!handle_envp(shell))
            return (reset_shell(shell), false);

        shell->line = readline("Minishell % ");
        if (!shell->line)
        {
            printf("Exit\n");
            reset_shell(shell);
            return (false);
        }

        if (*shell->line)
            add_history(shell->line);

        if (lexing(shell, shell->line, shell->envp) && parsing(shell))
        {
            // Collect heredocs - if interrupted, handle gracefully
            if (!collect_all_heredocs(shell->ast, shell))
            {
                // Check if it was interrupted by Ctrl+C
                if (shell->exit_status == 130 || shell->exit_status==1)
                {
                    // shell->exit_status = 130;
                    // global = 0;  // Reset global state

                    // Clear readline state to prevent double prompt
                    rl_replace_line("", 0);
                    rl_on_new_line();

                    // Don't print extra newline or prompt here
                    reset_shell(shell);
                    continue;  // Go back to readline prompt
                }
                // Other errors - continue normally
                reset_shell(shell);
                continue;
            }

            // Execute if heredocs collected successfully
            execute_ast(shell->ast, &shell->var, shell);
        }

        reset_shell(shell);
    }
    return (true);
}
