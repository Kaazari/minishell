/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helpers.c                                    :+:      :+:    :+:   */
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
 * Initialise la structure pipex pour gérer les pipes
 *
 * @param pipex: Structure pipex à initialiser
 * @param cmd_count: Nombre de commandes dans le pipeline
 */
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

/**
 * Ferme tous les descripteurs de fichiers des pipes et libère la mémoire
 *
 * @param pipex: Structure pipex contenant les descripteurs à fermer
 */
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

/**
 * Attend tous les processus enfants et récupère leurs statuts
 *
 * @param pids: Tableau des PIDs des processus enfants
 * @param cmd_count: Nombre de commandes
 */
void	wait_all_children(pid_t *pids, int cmd_count)
{
	int	signal_num;
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if ((status & 0x7F) != 0)
			{
				signal_num = status & 0x7F;
				if (signal_num == SIGQUIT)
					write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
				g_signal_exit_status = 128 + signal_num;
			}
			else
				g_signal_exit_status = (status >> 8) & 0xFF;
		}
		i++;
	}
}
