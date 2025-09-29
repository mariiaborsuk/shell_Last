/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mborsuk <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:07:20 by mborsuk           #+#    #+#             */
/*   Updated: 2025/09/25 12:09:19 by mborsuk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"


int	execute_built_in(char **cmd, t_var **var, t_minishell *shell)
{
	char	**local_envp;
	int		result;

	if (ft_strcmp(cmd[0], "cd") == 0)
		return(cd_fns(cmd, shell));
	if (ft_strcmp(cmd[0], "env") == 0)
	{
		local_envp = convert_var_to_envp(var);
		if (argv_len(cmd) == 1)
		{
			result = print_env(local_envp);
			free_argv(&local_envp);
			return (result);
		}
		else
		{
			// Execute command with environment
			execve(cmd[1], &cmd[1], local_envp);
			perror("env");
			free_argv(&local_envp);
			return (127);
		}
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

int	is_variable_assignment(char **cmd)
{
	char	*equals_pos;

	if (!cmd || !cmd[0])
		return (0);
	equals_pos = ft_strchr(cmd[0], '=');
	if (equals_pos && equals_pos != cmd[0] && (ft_isalpha(cmd[0][0]) || cmd[0][0] == '_'))
		return (1);
	return (0);
}

int	execute_variable_assignment(char **cmd, t_var **var)
{
	char	*equals_pos;
	char	*key;
	char	*value;
	t_var	new_var;

	equals_pos = ft_strchr(cmd[0], '=');
	key = ft_substr(cmd[0], 0, equals_pos - cmd[0]);
	value = ft_strdup(equals_pos + 1);
	if (!key || !value)
	{
		free(key);
		free(value);
		return (1);
	}
	new_var.key = key;
	new_var.val = value;
	new_var.exported = false;
	add_var(&new_var, var);
	free(key);
	free(value);
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

	if (is_variable_assignment(node->cmd->argv))
		rc = execute_variable_assignment(node->cmd->argv, &var);
	else if (is_built_in(node->cmd->argv) == 0)
	{	rc = extern_cmd(node->cmd->argv, shell->envp, shell);

	}
	else
		{
			rc = execute_built_in(node->cmd->argv, &var, shell);
			shell->exit_status=rc;
		}
	if (original_head != NULL)
		close_files(original_head);

	return (rc);
}


void	execute_cmd(t_ast *node, t_var **var, t_minishell *shell)
{
	if (is_variable_assignment(node->cmd->argv))
		shell->exit_status = execute_variable_assignment(node->cmd->argv, var);
	else if (is_built_in(node->cmd->argv))
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
