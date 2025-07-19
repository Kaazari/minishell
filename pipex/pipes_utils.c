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
	if (i > 0 && shell->pipex->pipe_fds)
		dup2(shell->pipex->pipe_fds[2 * (i - 1)], STDIN_FILENO);
	if (i < cmd_count - 1 && shell->pipex->pipe_fds)
		dup2(shell->pipex->pipe_fds[2 * i + 1], STDOUT_FILENO);
	close_pipes(shell->pipex);
}

void	execute_child_command(t_shell *shell, t_cmd *command)
{
	shell->cmd = command;
	handle_redirections(command, shell);
	execute_shell_command(command->args, shell);
	restore_redirections(command);
	exit(0);
}
