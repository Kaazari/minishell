/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_setup.c                                      :+:      :+:    :+:   */
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
 * Configure le pipe d'entrée pour un processus enfant
 *
 * @param shell: Structure shell
 * @param i: Index de la commande
 */
void	setup_input_pipe(t_shell *shell, int i)
{
	int	in;

	if (i > 0 && shell->pipex->pipe_fds)
	{
		in = shell->pipex->pipe_fds[2 * (i - 1) + 1];
		dup2(in, STDIN_FILENO);
		close(in);
	}
}

/**
 * Configure le pipe de sortie pour un processus enfant
 *
 * @param shell: Structure shell
 * @param i: Index de la commande
 * @param cmd_count: Nombre de commandes
 */
void	setup_output_pipe(t_shell *shell, int i, int cmd_count)
{
	int	out;

	if (i < cmd_count - 1 && shell->pipex->pipe_fds)
	{
		out = shell->pipex->pipe_fds[2 * i];
		dup2(out, STDOUT_FILENO);
		close(out);
	}
}

/**
 * Ferme tous les descripteurs de fichiers des pipes
 *
 * @param shell: Structure shell
 */
void	close_all_pipe_fds(t_shell *shell)
{
	int	j;

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

/**
 * Configure tous les pipes pour un processus enfant
 *
 * @param shell: Structure shell
 * @param i: Index de la commande
 * @param cmd_count: Nombre de commandes
 */
void	setup_child_pipes(t_shell *shell, int i, int cmd_count)
{
	if (!shell->pipex)
		return ;
	setup_input_pipe(shell, i);
	setup_output_pipe(shell, i, cmd_count);
	close_all_pipe_fds(shell);
}
