/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_child.c                                      :+:      :+:    :+:   */
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
 * Crée un pipe pour une commande
 *
 * @param pipe_fd: Descripteurs de pipe
 * @param i: Index de la commande
 * @param cmd_count: Nombre de commandes
 * @return 1 si succès, 0 sinon
 */
int	create_pipe_for_command(int *pipe_fd, int i, int cmd_count)
{
	if (i < cmd_count - 1)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (0);
		}
	}
	return (1);
}

/**
 * Crée un processus enfant
 *
 * @param pids: Tableau des PIDs
 * @param i: Index de la commande
 * @return 1 si succès, 0 sinon
 */
int	fork_child_process(pid_t *pids, int i)
{
	pids[i] = fork();
	if (pids[i] == -1)
	{
		perror("fork");
		return (0);
	}
	return (1);
}

/**
 * Exécute un processus enfant avec les paramètres donnés
 *
 * @param params: Structure contenant les paramètres d'exécution
 */
void	execute_child_with_params(t_child_exec_params *params)
{
	if (params->i > 0)
	{
		dup2(params->prev_fd, STDIN_FILENO);
		close(params->prev_fd);
	}
	if (params->i < params->cmd_count - 1)
	{
		dup2(params->pipe_fd[1], STDOUT_FILENO);
		close(params->pipe_fd[0]);
		close(params->pipe_fd[1]);
	}
	setup_child_redirections(params->commands, params->i);
	setup_child_signals();
	execute_external_commands(params->commands[params->i]->args,
		params->shell->envp, params->shell);
	exit(1);
}
