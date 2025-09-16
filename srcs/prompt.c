/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:56:21 by akovalch          #+#    #+#             */
/*   Updated: 2025/09/16 22:23:55 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/minishell.h"
#include "../includes/signals.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

void	install_parent_signals(void)
{
	struct sigaction	sa;
	struct termios		t;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = parent_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
	if (tcgetattr(STDIN_FILENO, &t) == 0)
	{
		t.c_lflag &= ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &t);
	}
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
                return false;
            }
            current->collected = true;
        }
        current = current->next;
    }
    return true;
}
bool	check_heredoc(t_redirect *head, t_minishell *shell)
{
	int	res;

	res = 0;
	while (head)
	{
		if (head->type == heredoc)
		{
			head->fd = get_temp_fd(head->file, shell, &res);
			// if (res == 130)
			// {
			// 	free_minishell(shell);
			// 	_exit(130);
			// }
			if (res != 0)
			{
				free_minishell(shell);
				_exit(1);
			}
		}
		head = head->next;
	}
	return true;
}

bool collect_all_heredocs(t_ast *root, t_minishell *shell)
{
    if (!root)
       {return true;}
    // Collect heredocs in left-to-right, depth-first order
    if (root->left && !collect_all_heredocs(root->left, shell))
       {return false;}


    if (root->type == node_command && root->cmd && root->cmd->redirects)
    {

        if (!collect_heredocs_for_command(root->cmd->redirects, shell))
           {
			return false;}

    }

    if (root->right && !collect_all_heredocs(root->right, shell))
        return false;

    return true;
}



bool	shell_read_loop(t_minishell *shell)
{
	rl_catch_signals = 0;
	install_parent_signals();
	while (1)
	{
		if (!handle_envp(shell))
			return (reset_shell(shell), false);
		if_g_state(shell);
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
			 collect_all_heredocs(shell->ast, shell);

    // {
    //     free_minishell(shell);
    //     return 130;  // Interrupted
    // }
				execute_ast(shell->ast, &shell->var, shell);}
		// if (shell->exit_status == 130)
		// {
		// 	printf("DEBUG: Handling 130 status\n");
		// 	shell->exit_status = 0;
		// 	rl_replace_line("", 0);
		// 	rl_redisplay();
		// }
		reset_shell(shell);
	}
	return (true);
}
