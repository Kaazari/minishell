/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "pipex.h"

int	*allocate_pipe_fds(int pipe_count)
{
	int	*pipe_fds;

	pipe_fds = malloc(sizeof(int) * 2 * pipe_count);
	if (!pipe_fds)
	{
		perror("malloc");
		return (NULL);
	}
	return (pipe_fds);
}

int	create_pipes(int *pipe_fds, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipe_fds + 2 * i) == -1)
		{
			perror("pipe");
			free(pipe_fds);
			return (0);
		}
		i++;
	}
	return (1);
}

void	setup_child_pipes(t_shell *shell, int i, int cmd_count)
{
	int	j;
	int	in = -1;
	int	out = -1;

	if (!shell->pipex)
		return ;
	if (i > 0 && shell->pipex->pipe_fds)
	{
		in = shell->pipex->pipe_fds[2 * (i - 1) + 1];
		dup2(in, STDIN_FILENO);
		close(in);
	}
	if (i < cmd_count - 1 && shell->pipex->pipe_fds)
	{
		out = shell->pipex->pipe_fds[2 * i];
		dup2(out, STDOUT_FILENO);
		close(out);
	}
	if (shell->pipex->pipe_fds)
	{
		j = 0;
		while (j < 2 * shell->pipex->pipe_count)
		{
			close(shell->pipex->pipe_fds[j]);
			j++;
		}
	}
}

void	execute_child_command(t_shell *shell, t_cmd *command)
{
	shell->cmd = command;
	execute_shell_command(command->args, shell, 1);
	exit(0);
}
