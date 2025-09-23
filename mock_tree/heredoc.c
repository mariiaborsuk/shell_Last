/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:10:14 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/23 22:07:40 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/signals.h"
#include <readline/history.h>
#include <readline/readline.h>


// void heredoc_sigint(int signum)
// {
//     (void)signum;
//     write(STDOUT_FILENO, "\n", 1);
//     exit(130);  // Exit immediately, don't just set flag
// }


// void	setup_heredoc_signals(void)
// {
// 	struct sigaction	sa;

// 	ft_memset(&sa, 0, sizeof(sa));
// 	sa.sa_handler = heredoc_sigint;
// 	sa.sa_flags = 0;
// 	sigaction(SIGINT, &sa, NULL);
// 	sa.sa_handler = SIG_IGN;
// 	sigaction(SIGQUIT, &sa, NULL);
// }


// // int manage_heredoc(int fd, char *delimiter, t_minishell *shell, int *status)
// // {
// //     char *line;

// //     setup_heredoc_signals();
// //     // Remove g_state check since signal handler exits immediately

// //     while (1)
// //     {
// //         write(STDOUT_FILENO, "> ", 2);  // Use STDOUT_FILENO
// //         line = get_next_line(STDIN_FILENO);

// //         if (!line)
// //         {
// //             // EOF (Ctrl+D)
// //             shell->exit_status = 0;
// //             break;
// //         }

// //         if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
// //             && line[ft_strlen(delimiter)] == '\n')
// //         {
// //             free(line);
// //             break;
// //         }

// //         write(fd, line, ft_strlen(line));
// //         free(line);
// //     }
// // *status=0;
// //     shell->exit_status = 0;
// //     return 0;
// // }

// // int manage_heredoc(int fd, char *delimiter, t_minishell *shell, int *status)
// // {
// //     char *line;
// //     int tty_fd;

// //     setup_heredoc_signals();

// //     // Open terminal directly to bypass pipe redirection
// //     tty_fd = open("/dev/tty", O_RDWR);
// //     if (tty_fd == -1)
// //     {
// //         perror("open /dev/tty");
// //         *status = 1;
// //         return 1;
// //     }

// //     while (1)
// //     {
// //         write(tty_fd, "> ", 2);  // Write prompt to terminal
// //         line = get_next_line(tty_fd);  // Read from terminal

// //         if (!line)
// //         {
// //             // EOF (Ctrl+D)
// //             write(tty_fd, "\n", 1);  // Add newline after Ctrl+D
// //             shell->exit_status = 0;
// //             break;
// //         }

// //         if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
// //             && line[ft_strlen(delimiter)] == '\n')
// //         {
// //             free(line);
// //             break;
// //         }

// //         write(fd, line, ft_strlen(line));  // Write to pipe
// //         free(line);
// //     }

// //     close(tty_fd);
// //     *status = 0;
// //     shell->exit_status = 0;
// //     return 0;
// // }

// static char *read_line_from_tty(int tty_fd)
// {
//     char *line = NULL;
//     char c;
//     int len = 0;
//     int capacity = 128;

//     line = malloc(capacity);
//     if (!line)
//         return NULL;

//     while (read(tty_fd, &c, 1) > 0)
//     {
//         if (c == '\n')
//             break;

//         if (len >= capacity - 1)
//         {
//             capacity *= 2;
//             char *new_line = realloc(line, capacity);
//             if (!new_line)
//             {
//                 free(line);
//                 return NULL;
//             }
//             line = new_line;
//         }

//         line[len++] = c;
//     }

//     line[len] = '\0';

//     if (len == 0 && c != '\n')  // EOF without newline
//     {
//         free(line);
//         return NULL;
//     }

//     return line;
// }

// int manage_heredoc(int fd, char *delimiter, int *status)
// {
//     char *line;
//     int tty_fd;

//     // setup_heredoc_signals();

//     tty_fd = open("/dev/tty", O_RDWR);
//     if (tty_fd == -1)
//     {
//         *status = 1;
//         return 1;
//     }

//     while (1)
//     {
//         write(tty_fd, "> ", 2);
//         line = read_line_from_tty(tty_fd);

//         if (!line)  // EOF
//         {
//             write(tty_fd, "\n", 1);
//             break;
//         }

//         if (ft_strcmp(line, delimiter) == 0)
//         {
//             free(line);
//             break;
//         }

//         write(fd, line, ft_strlen(line));
//         write(fd, "\n", 1);
//         free(line);
//     }

//     close(tty_fd);
//     *status = 0;
//     // DON'T modify shell->exit_status in child!
//     // shell->exit_status = 0;  ← REMOVE THIS
//     return 0;
// }

// int create_heredoc_pipe(char *delimiter, t_minishell *shell, int *status)
// {
//     int pipefd[2];
//     pid_t pid;
//     int child_status;
//     // struct sigaction old_sigint;

//     if (pipe(pipefd) == -1)
//         return -1;

//     // // Disable parent's SIGINT handler during heredoc
//     // struct sigaction ignore_sigint;
//     // ignore_sigint.sa_handler = SIG_IGN;
//     // ignore_sigint.sa_flags = 0;
//     // sigemptyset(&ignore_sigint.sa_mask);
//     // sigaction(SIGINT, &ignore_sigint, &old_sigint);

//     pid = fork();
//     if (pid == 0) {
//         // Child: restore and setup heredoc signals
//         close(pipefd[0]);
//         int exit_code = manage_heredoc(pipefd[1], delimiter,  status);
// 		printf("exit code %d\n", exit_code);
//         close(pipefd[1]);
//         exit(exit_code);
//     } else {
//         // Parent: wait for child
//         close(pipefd[1]);
//         waitpid(pid, &child_status, 0);

//         // RESTORE parent's original SIGINT handler
//         // sigaction(SIGINT, &old_sigint, NULL);

//         // // Clear any signal state contamination
//         // g_state &= ~GOT_SIGINT;

//         // if (WIFEXITED(child_status)) {
//         //     int exit_code = WEXITSTATUS(child_status);
//         //     if (exit_code == 130) {
//         //         *status = 130;
// 		// 		global=130;
// 		// 		printf("status %d, global %d\n", *status, global);
//         //         close(pipefd[0]);
//         //         return -1;
//         //     }
//         // }

//         *status = 0;
// 		 shell->exit_status=0;
// 	// 		child_running(&child_status, pid);
// 	// extrern_exit(child_status, shell);
//         return pipefd[0];
//     }
// }


void heredoc_sigint(int signum)
{
    (void)signum;
    write(STDOUT_FILENO, "\n", 1);
    // Don't print prompt here - let parent handle it
    exit(130);  // Exit with 130 to indicate SIGINT termination
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

// static char *read_line_from_tty(int tty_fd)
// {
//     char *line = NULL;
//     char c;
//     int len = 0;
//     int capacity = 128;

//     line = malloc(capacity);
//     if (!line)
//         return NULL;

//     while (read(tty_fd, &c, 1) > 0)
//     {
//         if (c == '\n')
//             break;

//         if (len >= capacity - 1)
//         {
//             capacity *= 2;
//             char *new_line = realloc(line, capacity);
//             if (!new_line)
//             {
//                 free(line);
//                 return NULL;
//             }
//             line = new_line;
//         }

//         line[len++] = c;
//     }

//     line[len] = '\0';

//     if (len == 0 && c != '\n')  // EOF without newline
//     {
//         free(line);
//         return NULL;
//     }

//     return line;
// }

// int manage_heredoc(int fd, char *delimiter, int *status)
// {
//     char *line;
//     int tty_fd;

//     // UNCOMMENT THIS - Essential for SIGINT detection!
//     setup_heredoc_signals();

//     tty_fd = open("/dev/tty", O_RDWR);
//     if (tty_fd == -1)
//     {
//         *status = 1;
//         return 1;
//     }

//     while (1)
//     {
//         write(tty_fd, "> ", 2);
//         line = read_line_from_tty(tty_fd);

//         if (!line)  // EOF (Ctrl+D)
//         {
//             write(tty_fd, "\n", 1);
//             break;
//         }

//         if (ft_strcmp(line, delimiter) == 0)
//         {
//             free(line);
//             break;
//         }

//         write(fd, line, ft_strlen(line));
//         write(fd, "\n", 1);
//         free(line);
//     }

//     close(tty_fd);
//     *status = 0;
//     return 0;
// }
int manage_heredoc(int fd,  char *delimiter, t_minishell *shell)
{
    int tty = open("/dev/tty", O_RDWR);
	 setup_heredoc_signals();
    if (tty == -1) tty = STDIN_FILENO;

    for (;;) {
        if (isatty(tty)) write(tty, "> ", 2);

        char *line = get_next_line(tty);
        if (!line) {
            // EOF (Ctrl-D) — optional warning:
            // dprintf(STDERR_FILENO, "warning: here-document delimited by end-of-file (wanted '%s')\n", delim);
            break;
        }

        // strip trailing '\n'
        size_t len = ft_strlen(line);
        if (len && line[len - 1] == '\n') line[--len] = '\0';

        // end delimiter?
        if (ft_strcmp(line, delimiter) == 0) { free(line); break; }

        // if (expand) {
        //     // your expansion helper (make sure it can realloc/replace 'line')
        //     get_val(&line, shell->var); // or expand_vars_in_place(&line, env)
        //     len = ft_strlen(line);
        // }
		if (!check_quote(shell->tokens))
			get_val(&line, shell->var);

        // write line + newline to heredoc temp/pipe
        write(fd, line, len);
        write(fd, "\n", 1);
        free(line);
    }

    if (tty != STDIN_FILENO) close(tty);
    return 0; // child will _exit(0) unless SIGINT handler _exit(130) fires
}

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
        // Child process: setup signals and handle heredoc
        close(pipefd[0]);
        int exit_code = manage_heredoc(pipefd[1], delimiter, shell);
        close(pipefd[1]);
        exit(exit_code);  // Child will exit with 130 if SIGINT received
    } else {
        // Parent process: wait for child
        close(pipefd[1]);
        waitpid(pid, &child_status, 0);

        // Restore parent's original SIGINT handler
        sigaction(SIGINT, &old_sigint, NULL);

        // Check how child terminated
        if (WIFEXITED(child_status)) {
            int exit_code = WEXITSTATUS(child_status);

            if (exit_code == 130) {
                // Child was interrupted by Ctrl+C
                *status = 130;
                shell->exit_status = 130;
                close(pipefd[0]);

                // Clear any readline state if using readline
                if (isatty(STDIN_FILENO)) {
                    rl_replace_line("", 0);
                    rl_redisplay();
                }

                return -1;
            }
            // Normal completion
            *status = 0;
            shell->exit_status = 0;
        } else if (WIFSIGNALED(child_status)) {
            // Child was killed by signal
            int sig = WTERMSIG(child_status);

            if (sig == SIGINT) {
                *status = 130;
                shell->exit_status = 130;
                close(pipefd[0]);

                // Clear any readline state if using readline
                if (isatty(STDIN_FILENO)) {
                    rl_replace_line("", 0);
                    rl_redisplay();
                }

                return -1;
            }
            *status = 1;
            shell->exit_status = 1;
        }

        return pipefd[0];
    }
}

// // ********************************************************************************************************
// int manage_heredoc(int wfd, const char *delim /* parsed, unquoted form */, bool expand, t_minishell *shell)
// {
//     int tty = open("/dev/tty", O_RDWR);
//     if (tty == -1) tty = STDIN_FILENO;

//     for (;;) {
//         if (isatty(tty)) write(tty, "> ", 2);

//         char *line = get_next_line(tty);
//         if (!line) {
//             // EOF (Ctrl-D) — optional warning:
//             // dprintf(STDERR_FILENO, "warning: here-document delimited by end-of-file (wanted '%s')\n", delim);
//             break;
//         }

//         // strip trailing '\n'
//         size_t len = ft_strlen(line);
//         if (len && line[len - 1] == '\n') line[--len] = '\0';

//         // end delimiter?
//         if (ft_strcmp(line, delim) == 0) { free(line); break; }

//         if (expand) {
//             // your expansion helper (make sure it can realloc/replace 'line')
//             get_val(&line, shell->var); // or expand_vars_in_place(&line, env)
//             len = ft_strlen(line);
//         }

//         // write line + newline to heredoc temp/pipe
//         write(wfd, line, len);
//         write(wfd, "\n", 1);
//         free(line);
//     }

//     if (tty != STDIN_FILENO) close(tty);
//     return 0; // child will _exit(0) unless SIGINT handler _exit(130) fires
// }
