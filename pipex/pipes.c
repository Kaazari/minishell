/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "pipex.h"

void	init_pipes(t_pipex *pipex, int cmd_count)
{
	pipex->cmd_count = cmd_count;
	pipex->pipe_count = cmd_count - 1;
	if (pipex->pipe_count <= 0)
	{
		pipex->pipe_fds = NULL;
		return ;
	}
	pipex->pipe_fds = allocate_pipe_fds(pipex->pipe_count);
	if (!pipex->pipe_fds)
		return ;
	if (!create_pipes(pipex->pipe_fds, pipex->pipe_count))
		pipex->pipe_fds = NULL;
}

void	close_pipes(t_pipex *pipex)
{
	int	i;

	if (!pipex->pipe_fds)
		return ;
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
	if (cmd_count > 1 && !shell->pipex->pipe_fds)
	{
		perror("Failed to initialize pipes");
		return ;
	}
	i = 0;
	while (i < cmd_count)
	{
		if (commands[i] != NULL)
		{
			shell->pipex->pid = fork();
			if (shell->pipex->pid == 0)
			{
				setup_child_pipes(shell, i, cmd_count);
				execute_child_command(shell, commands[i]);
			}
		}
		i++;
	}
	close_pipes(shell->pipex);
	wait_children(shell->pipex);
}
