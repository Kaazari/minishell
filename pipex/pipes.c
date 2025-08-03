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

static void	init_pipe_execution(int cmd_count, pid_t **pids, int *prev_fd)
{
	*pids = malloc(sizeof(pid_t) * cmd_count);
	if (!*pids)
		return ;
	*prev_fd = STDIN_FILENO;
}

void	execute_piped_commands(t_shell *shell, t_cmd **commands, int cmd_count)
{
	t_pipe_loop_params	params;
	int					prev_fd;
	pid_t				*pids;

	init_pipe_execution(cmd_count, &pids, &prev_fd);
	if (!pids)
		return ;
	params.shell = shell;
	params.commands = commands;
	params.cmd_count = cmd_count;
	params.prev_fd = &prev_fd;
	params.pids = pids;
	execute_pipe_loop(&params);
	finalize_pipe_execution(commands, cmd_count, prev_fd, pids);
	free(pids);
}
