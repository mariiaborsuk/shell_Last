/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:10:14 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/09 22:17:39 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/signals.h"
#include <readline/history.h>
#include <readline/readline.h>

// void	heredoc_sigint(int signum)
// {
// 	(void)signum;
// 	write(STDOUT_FILENO, "\n", 1);
// 	g_state |= GOT_SIGINT;
// }
void heredoc_sigint(int signum)
{
    (void)signum;
    write(STDOUT_FILENO, "\n", 1);
    exit(130);  // Exit immediately, don't just set flag
}

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = heredoc_sigint;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

// int	manage_heredoc(int fd, char *delimiter, t_minishell *shell, int *status)
// {
// 	char	*line;

// 	setup_heredoc_signals();
// 	g_state &= ~GOT_SIGINT;
// 	while (1)
// 	{
// 		write(1, " > ", 4);
// 		line = get_next_line(STDIN_FILENO);
// 		if (g_state & GOT_SIGINT)
// 		{
// 			shell->exit_status = 130;
// 			free(line);
// 			*status=130;
// 			return 130;
// 		}
// 		if (!line)
// 		{
// 			shell->exit_status = 1;
// 			break ;
// 		}
// 		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
// 			&& line[ft_strlen(delimiter)] == '\n')
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(fd, line, ft_strlen(line));
// 		free(line);
// 	}
// 	shell->exit_status=0;
// 	return (0);
// }
int manage_heredoc(int fd, char *delimiter, t_minishell *shell, int *status)
{
    char *line;

    setup_heredoc_signals();
    // Remove g_state check since signal handler exits immediately

    while (1)
    {
        write(STDOUT_FILENO, "> ", 2);  // Use STDOUT_FILENO
        line = get_next_line(STDIN_FILENO);

        if (!line)
        {
            // EOF (Ctrl+D)
            shell->exit_status = 0;
            break;
        }

        if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
            && line[ft_strlen(delimiter)] == '\n')
        {
            free(line);
            break;
        }

        write(fd, line, ft_strlen(line));
        free(line);
    }
*status=0;
    shell->exit_status = 0;
    return 0;
}

// int create_heredoc_pipe(char *delimiter, t_minishell *shell, int *status)
// {
//     int pipefd[2];
//     pid_t pid;

//     if (pipe(pipefd) == -1)
//         return -1;

//     pid = fork();
//     if (pid == 0) {
//         // Child: write to pipe
//         close(pipefd[0]);
//         manage_heredoc(pipefd[1], delimiter, shell, status);
//         close(pipefd[1]);
//         exit(0);
//     } else {
//         // Parent: return read end
//         close(pipefd[1]);
//         wait(NULL);
//         return pipefd[0]; // Use this as stdin for your command
//     }
// }

int create_heredoc_pipe(char *delimiter, t_minishell *shell, int *status)
{
    int pipefd[2];
    pid_t pid;
    int child_status;
    struct sigaction old_sigint;

    if (pipe(pipefd) == -1)
        return -1;

    // Disable parent's SIGINT handler during heredoc
    struct sigaction ignore_sigint;
    ignore_sigint.sa_handler = SIG_IGN;
    ignore_sigint.sa_flags = 0;
    sigemptyset(&ignore_sigint.sa_mask);
    sigaction(SIGINT, &ignore_sigint, &old_sigint);

    pid = fork();
    if (pid == 0) {
        // Child: restore and setup heredoc signals
        close(pipefd[0]);
        int exit_code = manage_heredoc(pipefd[1], delimiter, shell, status);
        close(pipefd[1]);
        exit(exit_code);
    } else {
        // Parent: wait for child
        close(pipefd[1]);
        waitpid(pid, &child_status, 0);

        // RESTORE parent's original SIGINT handler
        sigaction(SIGINT, &old_sigint, NULL);

        // Clear any signal state contamination
        g_state &= ~GOT_SIGINT;

        if (WIFEXITED(child_status)) {
            int exit_code = WEXITSTATUS(child_status);
            if (exit_code == 130) {
                *status = 130;
                close(pipefd[0]);
                return -1;
            }
        }

        *status = 0;
        return pipefd[0];
    }
}


