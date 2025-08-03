/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "pipex.h"

/**
 * Nettoie les pipes du processus parent
 *
 * @param prev_fd: Descripteur précédent
 * @param pipe_fd: Descripteurs de pipe
 * @param i: Index de la commande
 * @param cmd_count: Nombre de commandes
 */
void	cleanup_parent_pipes(int *prev_fd, int *pipe_fd, int i, int cmd_count)
{
	if (i > 0)
		close(*prev_fd);
	if (i < cmd_count - 1)
	{
		*prev_fd = pipe_fd[0];
		close(pipe_fd[1]);
	}
}

/**
 * Traite une itération de pipe
 *
 * @param params: Structure contenant les paramètres d'itération
 */
void	process_pipe_iteration(t_pipe_iteration_params *params)
{
	t_child_exec_params	child_params;

	if (!create_pipe_for_command(params->pipe_fd,
			*params->i, params->cmd_count))
		return ;
	if (!fork_child_process(params->pids, *params->i))
		return ;
	if (params->pids[*params->i] == 0)
	{
		child_params.commands = params->commands;
		child_params.i = *params->i;
		child_params.prev_fd = *params->prev_fd;
		child_params.pipe_fd = params->pipe_fd;
		child_params.cmd_count = params->cmd_count;
		child_params.shell = params->shell;
		execute_child_with_params(&child_params);
	}
	else
		cleanup_parent_pipes(params->prev_fd, params->pipe_fd, *params->i,
			params->cmd_count);
}

/**
 * Finalise l'exécution des pipes
 *
 * @param commands: Tableau de commandes
 * @param cmd_count: Nombre de commandes
 * @param prev_fd: Descripteur de fichier précédent
 * @param pids: Tableau des PIDs des processus enfants
 */
void	finalize_pipe_execution(t_cmd **commands, int cmd_count, int prev_fd,
		pid_t *pids)
{
	close_parent_heredoc_fds(commands, cmd_count);
	wait_all_children(pids, cmd_count);
	if (cmd_count > 1)
		close(prev_fd);
}

/**
 * Exécute la boucle principale des pipes
 *
 * @param params: Structure contenant les paramètres de la boucle
 */
void	execute_pipe_loop(t_pipe_loop_params *params)
{
	t_pipe_iteration_params	iter_params;
	int						pipe_fd[2];
	int						i;

	i = 0;
	while (i < params->cmd_count)
	{
		iter_params.shell = params->shell;
		iter_params.commands = params->commands;
		iter_params.cmd_count = params->cmd_count;
		iter_params.pipe_fd = pipe_fd;
		iter_params.prev_fd = params->prev_fd;
		iter_params.pids = params->pids;
		iter_params.i = &i;
		process_pipe_iteration(&iter_params);
		i++;
	}
}
