/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:10:14 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/28 18:43:16 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/signals.h"
#include <readline/history.h>
#include <readline/readline.h>

void heredoc_sigint(int signum)
{
    (void)signum;
    write(STDOUT_FILENO, "\n", 1);
	g_state=3;
    // Don't print prompt here - let parent handle it
    // exit(130);  // Exit with 130 to indicate SIGINT termination
}

void setup_heredoc_signals(void)
{
    struct sigaction sa;

    ft_memset(&sa, 0, sizeof(sa));
    sa.sa_handler = heredoc_sigint;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);  // Initialize signal mask
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

// int manage_heredoc(int fd,  char *delimiter, t_minishell *shell)
// {
//     int tty = open("/dev/tty", O_RDWR);
// 	 setup_heredoc_signals();
//     if (tty == -1) tty = STDIN_FILENO;

//    while (1) {
//         if (isatty(tty)) write(tty, "> ", 2);

//         char *line = get_next_line(tty);
//         // if (!line) {
//         //     shell->exit_status=1;
// 		// 	if (isatty(tty)) write(tty, "\n", 1);
// 		// 	break;

//         // }
// 	     if (!line) {
//             // EOF (^D) was pressed - treat as cancellation
//             shell->exit_status = 1; // Set error status
//             if (isatty(tty)) {
//                 write(tty, "\n", 1);
//                 write(tty, "minishell: warning: here-document delimited by end-of-file\n", 59);
//             }
//             if (tty != STDIN_FILENO) close(tty);
//             return 1; // Return error code
//         }

//         // strip trailing '\n'
//         size_t len = ft_strlen(line);
//         if (len && line[len - 1] == '\n') line[--len] = '\0';

//         // end delimiter?
//         if (ft_strcmp(line, delimiter) == 0) { free(line); break; }
// 		if (!check_quote(shell->tokens))
// 			get_val(&line, shell->var);
//         write(fd, line, len+1);
//         write(fd, "\n", 1);
//         free(line);
//     }

//     if (tty != STDIN_FILENO) close(tty);
//     return 0; // child will _exit(0) unless SIGINT handler _exit(130) fires
// }


// int create_heredoc_pipe(char *delimiter, t_minishell *shell, int *status)
// {
//     int pipefd[2];
//     pid_t pid;
//     int child_status;
//     struct sigaction old_sigint;

//     if (pipe(pipefd) == -1)
//         return -1;

//     // Save parent's SIGINT handler and ignore SIGINT in parent during heredoc
//     struct sigaction ignore_sigint;
//     ignore_sigint.sa_handler = SIG_IGN;
//     ignore_sigint.sa_flags = 0;
//     sigemptyset(&ignore_sigint.sa_mask);
//     sigaction(SIGINT, &ignore_sigint, &old_sigint);

//     pid = fork();
//     if (pid == 0) {
//         // Child process: setup signals and handle heredoc
//         close(pipefd[0]);
//         int exit_code = manage_heredoc(pipefd[1], delimiter, shell);
//         close(pipefd[1]);
//         exit(exit_code);  // Child will exit with 130 if SIGINT received
//     } else {
//         close(pipefd[1]);
//         waitpid(pid, &child_status, 0);
//         sigaction(SIGINT, &old_sigint, NULL);
//         if (WIFEXITED(child_status)) {
//             int exit_code = WEXITSTATUS(child_status);

//             if (exit_code == 130) {
//                 *status = 130;
//                 shell->exit_status = 130;
//                 close(pipefd[0]);
//                 // if (isatty(STDIN_FILENO)) {
//                 //     rl_replace_line("", 0);
//                 //     rl_redisplay();
//                 // }

//                 return -1;
//             }
// 			else if (exit_code == 1) {
//         // EOF case - cancel the command
//         *status = 1;
//         shell->exit_status = 1;
//         close(pipefd[0]);
//         return -1;
//     }
//             *status = 0;
//             shell->exit_status = 0;
//         } else if (WIFSIGNALED(child_status)) {
//             // Child was killed by signal
//             int sig = WTERMSIG(child_status);

//             if (sig == SIGINT) {
//                 *status = 130;
//                 shell->exit_status = 130;
//                 close(pipefd[0]);

//                 // Clear any readline state if using readline
//                 // if (isatty(STDIN_FILENO)) {
//                 //     rl_replace_line("", 0);
//                 //     rl_redisplay();
//                 // }

//                 return -1;
//             }
//             *status = 1;
//             shell->exit_status = 1;
//         }


//         return pipefd[0];
//     }
// }





static char *read_line_from_fd(int fd)
{
    char *line = NULL;
    char *temp;
    size_t capacity = 64;
    size_t size = 0;
    char c;
    ssize_t bytes_read;

    line = malloc(capacity);
    if (!line)
        return (NULL);

    while ((bytes_read = read(fd, &c, 1)) > 0)
    {
        if (c == '\n')
        {
            line[size] = '\0';
            return (line);
        }

        // Expand buffer if needed
        if (size >= capacity - 1)
        {
            capacity *= 2;
            temp = realloc(line, capacity);
            if (!temp)
            {
                free(line);
                return (NULL);
            }
            line = temp;
        }

        line[size++] = c;
    }

    // Handle EOF or error
    if (bytes_read == 0)
    {
        if (size == 0)
        {
            // EOF with no data - true EOF
            free(line);
            return (NULL);
        }
        else
        {
            // EOF but we have some data - return it
            line[size] = '\0';
            return (line);
        }
    }

    // Error occurred (bytes_read < 0)
    free(line);
    return (NULL);
}

int manage_heredoc(int fd, char *delimiter, t_minishell *shell)
{
    int tty = open("/dev/tty", O_RDWR);
    setup_heredoc_signals();
    if (tty == -1) tty = STDIN_FILENO;
    if (isatty(tty)) write(tty, "> ", 2);

    while (1) {
        char *line = read_line_from_fd(tty);
		if(g_state==3)
		{
			free_minishell(shell);
			exit(130);
		}
        if (!line) {
            // EOF (^D) was pressed
            if (isatty(tty)) {
                write(tty, "\n", 1);
                write(STDERR_FILENO, "minishell: warning: here-document delimited by end-of-file (wanted `", 67);
                write(STDERR_FILENO, delimiter, ft_strlen(delimiter));
                write(STDERR_FILENO, "')\n", 3);
            }
            printf("^D IS CALLED\n");
            shell->exit_status = 1;
            if (tty != STDIN_FILENO) close(tty);
            return 1;
        }

        // Check if this is the end delimiter
        if (ft_strcmp(line, delimiter) == 0) {
            free(line);
            shell->exit_status = 0;
            if (tty != STDIN_FILENO) close(tty);
            return 0;
        }

        // Variable expansion
        if (!check_quote(shell->tokens))
            get_val(&line, shell->var);

        // Write the line to the pipe
        size_t len = ft_strlen(line);
        if (len > 0) {
            write(fd, line, len);
        }
        write(fd, "\n", 1);
        free(line);

        if (isatty(tty)) write(tty, "> ", 2);
    }
}
// int manage_heredoc(int fd, char *delimiter, t_minishell *shell)
// {
//     int tty = open("/dev/tty", O_RDWR);
//     setup_heredoc_signals();
//     if (tty == -1) tty = STDIN_FILENO;
// 	if (isatty(tty)) write(tty, "> ", 2);
//     while (1) {


//         char *line = get_next_line(tty);
//         if (!line) {
//             // EOF (^D) was pressed
//             if (isatty(tty)) {
//                 write(tty, "\n", 1);
//                 // Print warning but continue - don't set error status
//                 write(STDERR_FILENO, "minishell: warning: here-document delimited by end-of-file (wanted `", 67);
//                 write(STDERR_FILENO, delimiter, ft_strlen(delimiter));
//                 write(STDERR_FILENO, "')\n", 3);
//             }
// 			printf("^D IS CALLED\n");
//             shell->exit_status = 1;
//             if (tty != STDIN_FILENO) close(tty);
//             return 1;
//         }

//         // Strip trailing '\n'
//         size_t len = ft_strlen(line);
//         if (len > 0 && line[len - 1] == '\n') {
//             line[len - 1] = '\0';
//             len--;
//         }

//         // Check if this is the end delimiter
//         if (ft_strcmp(line, delimiter) == 0) {
//             free(line);
//             shell->exit_status = 0;
//             if (tty != STDIN_FILENO) close(tty);
//             return 0;
//         }

//         // Variable expansion
//         if (!check_quote(shell->tokens))
//             get_val(&line, shell->var);

//         // Write the line to the pipe
//         len = ft_strlen(line); // Recalculate after potential expansion
//         if (len > 0) {
//             write(fd, line, len);
//         }
//         write(fd, "\n", 1);
//         free(line);
// 		 if (isatty(tty)) write(tty, "> ", 2);
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

    // Save parent's SIGINT handler and ignore SIGINT in parent during heredoc
    struct sigaction ignore_sigint;
    ignore_sigint.sa_handler = SIG_IGN;
    ignore_sigint.sa_flags = 0;
    sigemptyset(&ignore_sigint.sa_mask);
    sigaction(SIGINT, &ignore_sigint, &old_sigint);

    pid = fork();
    if (pid == 0) {
        // Child process
        close(pipefd[0]);
        int exit_code = manage_heredoc(pipefd[1], delimiter, shell);
        close(pipefd[1]);
        exit(exit_code);
    } else if (pid > 0) {
        // Parent process
        close(pipefd[1]);
        waitpid(pid, &child_status, 0);
        sigaction(SIGINT, &old_sigint, NULL);

        if (WIFEXITED(child_status)) {
            int exit_code = WEXITSTATUS(child_status);

            if (exit_code == 130) {
                // Only SIGINT should cancel the command
                *status = 130;
                shell->exit_status = 130;
                close(pipefd[0]);
                return -1;
            }

            // For both normal completion AND EOF, return the pipe
            // The warning was already printed in the child
            *status = 0;
            shell->exit_status = 0;
            return pipefd[0]; // Command should execute
        } else if (WIFSIGNALED(child_status)) {
            int sig = WTERMSIG(child_status);

            if (sig == SIGINT) {
                *status = 130;
                shell->exit_status = 130;
                close(pipefd[0]);
                return -1;
            }
            *status = 1;
            shell->exit_status = 1;
            close(pipefd[0]);
            return -1;
        }

        close(pipefd[0]);
        return -1;
    } else {
        // Fork failed
        close(pipefd[0]);
        close(pipefd[1]);
        sigaction(SIGINT, &old_sigint, NULL);
        return -1;
    }
}

