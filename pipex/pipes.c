/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:31:53 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "pipex.h"

void	init_pipes(t_pipex *pipex, int cmd_count)
{
	int	i;

	pipex->cmd_count = cmd_count;
	pipex->pipe_count = cmd_count - 1;
	if (pipex->pipe_count <= 0)
	{
		pipex->pipe_fds = NULL;
		return ;
	}
	pipex->pipe_fds = malloc(sizeof(int) * 2 * pipex->pipe_count);
	if (!pipex->pipe_fds)
	{
		perror("malloc");
		return ;
	}
	i = 0;
	while (i < pipex->pipe_count)
	{
		if (pipe(pipex->pipe_fds + 2 * i) == -1)
		{
			perror("pipe");
			free(pipex->pipe_fds);
			pipex->pipe_fds = NULL;
			return ;
		}
		i++;
	}
}

void	close_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < 2 * pipex->pipe_count)
	{
		close(pipex->pipe_fds[i]);
		i++;
	}
	free(pipex->pipe_fds);
	pipex->pipe_fds = NULL;
}

void	wait_children(t_pipex *pipex)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipex->cmd_count)
	{
		waitpid(-1, &status, 0);
		i++;
	}
}

void	execute_piped_commands(t_shell *shell, t_cmd **commands, int cmd_count)
{
	int	i;

	init_pipes(shell->pipex, cmd_count);
	i = 0;
	while (i < cmd_count)
	{
		shell->pipex->pid = fork();
		if (shell->pipex->pid == 0)
		{
			if (i > 0)
				dup2(shell->pipex->pipe_fds[2 * (i - 1)], STDIN_FILENO);
			if (i < cmd_count - 1)
				dup2(shell->pipex->pipe_fds[2 * i + 1], STDOUT_FILENO);
			close_pipes(shell->pipex);
			shell->cmd = commands[i];
			handle_redirections(commands[i]);
			execute_shell_command(commands[i]->args, shell);
			restore_redirections(commands[i]);
			exit(0);
		}
		i++;
	}
	close_pipes(shell->pipex);
	wait_children(shell->pipex);
}
