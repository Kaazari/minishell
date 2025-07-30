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

static void	child_exit(char *path, int status)
{
	if (path)
		free(path);
	exit(status);
}

void	execute_external_commands(char **args, char **envp, t_shell *shell)
{
	char	*path;

	path = validate_and_get_path(args, shell);
	if (!path)
	{
		/* In pipe context, we need to exit with error code */
		if (shell->pipex && shell->pipex->cmd_count > 1)
			exit(127);
		return ;
	}

	/* In pipe context, just exec directly */
	if (shell->pipex && shell->pipex->cmd_count > 1)
	{
		setup_child_signals();
		execve(path, args, envp);
		perror("execve");
		child_exit(path, 127);
	}

	/* In single command context, use fork */
	shell->state = 3;
	g_signal_exit_status = 999; /* Prevent signal handler from redisplaying */
	pid_t	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		execve(path, args, envp);
		perror("execve");
		child_exit(path, 1);
	}
	else if (pid > 0)
	{
		int	status;
		handle_parent_process(pid, &status, path, shell);
	}
	else
	{
		perror("fork");
		free(path);
		shell->state = 0;
	}
	g_signal_exit_status = 0; /* Reset signal status */
}

static void	handle_signal_termination(int status, t_shell *shell)
{
	if (WTERMSIG(status) == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		shell->exit_status = 128 + WTERMSIG(status);
	}
	else if (WTERMSIG(status) == SIGINT)
	{
		shell->exit_status = 128 + WTERMSIG(status);
	}
}

void	handle_parent_process(pid_t pid, int *status, char *path,
		t_shell *shell)
{
	waitpid(pid, status, 0);
	if (WIFSIGNALED(*status))
		handle_signal_termination(*status, shell);
	else if (WIFEXITED(*status))
		shell->exit_status = WEXITSTATUS(*status);
	free(path);
	shell->state = 0;
}
