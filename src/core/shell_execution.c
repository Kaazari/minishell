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

static void	handle_exit_command(char **args, t_shell *shell)
{
	int	result;

	result = builtin_exit(args, shell);
	if (result >= 1000)
	{
		shell->exit_status = result - 1000;
		exit(shell->exit_status);
	}
}

static void	execute_builtin_command(char **args, t_shell *shell)
{
	g_signal_exit_status = 999; /* Prevent signal handler from redisplaying */
	if (ft_strncmp(args[0], "cd", 3) == 0)
		shell->exit_status = builtin_cd(args, shell);
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		shell->exit_status = builtin_pwd(shell);
	else if (ft_strncmp(args[0], "echo", 5) == 0)
		shell->exit_status = builtin_echo(args, shell);
	else if (ft_strncmp(args[0], "env", 4) == 0)
		shell->exit_status = builtin_env(shell);
	else if (ft_strncmp(args[0], "export", 7) == 0)
		shell->exit_status = builtin_export(args, shell);
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		shell->exit_status = builtin_unset(args, shell);
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		handle_exit_command(args, shell);
	else if (ft_strncmp(args[0], ":", 2) == 0)
		shell->exit_status = 0;
	else
		execute_external_commands(args, shell->envp, shell);
	g_signal_exit_status = 0; /* Reset signal status */
}

void	execute_shell_command(char **args, t_shell *shell, int cmd_count)
{
	int	old_state;
	int	in_pipe_context;

	old_state = shell->state;
	shell->state = 3;

	// Detect pipe context by checking cmd_count
	in_pipe_context = (cmd_count > 1);

	// Appliquer les redirections seulement si pas dans un pipe
	// Dans les pipes, les redirections sont déjà appliquées dans le processus enfant
	if (shell->cmd && (!in_pipe_context))
		handle_redirections(shell->cmd, shell);

	if (args && args[0] && ft_strlen(args[0]) > 0)
		execute_builtin_command(args, shell);
	else if (args && args[0] && ft_strlen(args[0]) == 0)
	{
		printf("minishell: : command not found\n");
		shell->exit_status = 127;
	}

	// Restaurer les redirections seulement si pas dans un pipe
	if (shell->cmd && (!in_pipe_context))
		restore_redirections(shell->cmd);

	if (in_pipe_context)
	{
		fflush(stdout);
		exit(shell->exit_status);
	}

	shell->state = old_state;
}
