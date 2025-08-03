/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_pipe_context(char *path, char **args, char **envp)
{
	setup_child_signals();
	execve(path, args, envp);
	perror("execve");
	child_exit(path, 127);
}

static void	handle_single_context(char *path, char **args,
				char **envp, t_shell *shell)
{
	pid_t	pid;
	int		status;

	shell->state = 3;
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		execve(path, args, envp);
		perror("execve");
		child_exit(path, 1);
	}
	else if (pid > 0)
		handle_parent_process(pid, &status, path, shell);
	else
	{
		perror("fork");
		free(path);
		shell->state = 0;
	}
}

void	execute_external_commands(char **args, char **envp, t_shell *shell)
{
	char	*path;

	path = validate_and_get_path(args, shell);
	if (!path)
	{
		if (shell->pipex && shell->pipex->cmd_count > 1)
			exit(127);
		return ;
	}
	if (shell->pipex && shell->pipex->cmd_count > 1)
		handle_pipe_context(path, args, envp);
	else
		handle_single_context(path, args, envp, shell);
}

void	handle_parent_process(pid_t pid, int *status, char *path,
		t_shell *shell)
{
	waitpid(pid, status, 0);
	if ((*status & 0x7F) != 0)
	{
		handle_signal_termination(*status, shell);
	}
	else
	{
		shell->exit_status = (*status >> 8) & 0xFF;
	}
	free(path);
	shell->state = 0;
}
