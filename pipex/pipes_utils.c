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

void	execute_child_command(t_shell *shell, t_cmd *command)
{
	shell->cmd = command;
	execute_shell_command(command->args, shell, 1);
	exit(0);
}
