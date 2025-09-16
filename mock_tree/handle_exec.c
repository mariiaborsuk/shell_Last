/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:07:20 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/16 20:25:50 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"


int	execute_built_in(char **cmd, t_var **var, t_minishell *shell)
{
	char	**local_envp;
	int		result;

	if (ft_strcmp(cmd[0], "cd") == 0)
		return (cd_fns(cmd));
	if (ft_strcmp(cmd[0], "env") == 0)
	{
		local_envp = convert_var_to_envp(var);
		result = print_env(local_envp);
		free_argv(&local_envp);
		return (result);
	}
	if (ft_strcmp(cmd[0], "pwd") == 0)
		return (cmd_pwd());
	if (ft_strcmp(cmd[0], "echo") == 0)
		return (exec_echo(cmd));
	if (ft_strcmp(cmd[0], "export") == 0)
		{return (exec_export(cmd, var));}
	if (ft_strcmp(cmd[0], "unset") == 0)
		return (exec_unset(cmd, var));
	if (ft_strcmp(cmd[0], "exit") == 0)
		return (handle_exit(cmd, shell));
	return (0);
}

int	is_built_in(char **cmd)
{
	if (!cmd || !cmd[0])
		return (0);
	if (ft_strcmp(cmd[0], "cd") == 0 || ft_strcmp(cmd[0], "env") == 0)
		return (1);
	else if (ft_strcmp(cmd[0], "pwd") == 0 || ft_strcmp(cmd[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd[0], "unset") == 0 || ft_strcmp(cmd[0], "exit") == 0)
		return (1);
	else if (ft_strcmp(cmd[0], "export") == 0)
		return (1);
	else
		return (0);
}

int	handle_commands(t_var *var, t_minishell *shell, t_redirect *original_head,
		t_ast *node)
{
	int	rc;

	if (is_built_in(node->cmd->argv) == 0)
	{	rc = extern_cmd(node->cmd->argv, shell->envp, shell);
		printf("Seagflt 5&&& %s\n", original_head->file);

	}
	else
		{
			printf("handle_cmd\n");
			rc = execute_built_in(node->cmd->argv, &var, shell);
				printf("handle_command\n");
		}
	if (original_head != NULL)
		close_files(original_head);

	return (rc);
}


void	execute_cmd(t_ast *node, t_var **var, t_minishell *shell)
{
	if (is_built_in(node->cmd->argv))
		shell->exit_status = execute_built_in(node->cmd->argv, var, shell);
	else
		use_execve(node->cmd->argv, shell->envp, shell);
}

void	manage_cmd(t_ast *node, t_var *var, t_minishell *shell)
{
	if (!node)
		return ;
			if (node->type == node_command && node->cmd != NULL
		&& node->cmd->redirects != NULL)
	{
		shell->exit_status = handle_redir(node, var, shell);
		return ;
	}
	if (node->type == node_command && node->cmd)
		execute_cmd(node, &var, shell);
	else if (node->type == node_pipe)
	{
		execute_pipe(node, var, shell);
	}
}
