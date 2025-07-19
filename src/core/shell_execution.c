/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	execute_builtin_command(char **args, t_shell *shell)
{
	if (strcmp(args[0], "cd") == 0)
		shell->exit_status = builtin_cd(args, shell);
	else if (strcmp(args[0], "pwd") == 0)
		shell->exit_status = builtin_pwd(args);
	else if (strcmp(args[0], "echo") == 0)
		shell->exit_status = builtin_echo(args);
	else if (strcmp(args[0], "env") == 0)
		shell->exit_status = builtin_env(args, shell);
	else if (strcmp(args[0], "export") == 0)
		shell->exit_status = builtin_export(args, shell);
	else if (strcmp(args[0], "unset") == 0)
		shell->exit_status = builtin_unset(args, shell);
	else if (strcmp(args[0], "exit") == 0)
		builtin_exit(args);
	else if (strcmp(args[0], ":") == 0)
		shell->exit_status = builtin_colon(args);
	else
		execute_external_commands(args, shell->envp, shell);
}

void	execute_shell_command(char **args, t_shell *shell)
{
	int	old_state;

	old_state = shell->state;
	shell->state = 3;
	handle_redirections(shell->cmd, shell);
	if (args && args[0])
		execute_builtin_command(args, shell);
	restore_redirections(shell->cmd);
	shell->state = old_state;
}
