/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_commands_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Vérifie si la condition logique est remplie pour continuer
 *
 * @param commands: Tableau de commandes
 * @param i: Index de la commande actuelle
 * @param prev_exit_status: Statut de sortie de la commande précédente
 * @return 1 si on doit continuer, 0 si on doit arrêter
 */
int	should_continue_logical(t_cmd **commands, int i, int prev_exit_status)
{
	if (i + 1 >= commands[i]->logical_operator || !commands[i + 1])
		return (0);
	if (commands[i]->logical_operator == 1)
		return (prev_exit_status != 0);
	else if (commands[i]->logical_operator == 2)
		return (prev_exit_status == 0);
	return (1);
}

/**
 * Exécute une commande dans le contexte logique
 *
 * @param shell: Structure shell
 * @param commands: Tableau de commandes
 * @param i: Index de la commande à exécuter
 * @return Statut de sortie de la commande
 */
int	execute_logical_command(t_shell *shell, t_cmd **commands, int i)
{
	shell->cmd = commands[i];
	execute_shell_command(commands[i]->args, shell, 1);
	return (shell->exit_status);
}

/**
 * Initialise la structure pipex pour les commandes pipées
 *
 * @param shell: Structure shell contenant l'état
 * @param commands: Tableau de commandes à exécuter
 * @param count: Nombre de commandes dans le tableau
 * @return 1 en cas de succès, 0 en cas d'échec
 */
int	init_pipex_for_commands(t_shell *shell, t_cmd **commands, int count)
{
	if (shell->pipex)
	{
		if (shell->pipex->pipe_fds)
			free(shell->pipex->pipe_fds);
		free(shell->pipex);
	}
	shell->pipex = malloc(sizeof(t_pipex));
	if (!shell->pipex)
	{
		free_partial_cmds(commands, count);
		return (0);
	}
	shell->pipex->cmd_count = count;
	shell->pipex->pipe_count = count - 1;
	shell->pipex->pipe_fds = NULL;
	return (1);
}

/**
 * Nettoie la structure pipex après exécution
 *
 * @param shell: Structure shell contenant l'état
 */
void	cleanup_pipex_after_execution(t_shell *shell)
{
	if (shell->pipex)
	{
		if (shell->pipex->pipe_fds)
			free(shell->pipex->pipe_fds);
		free(shell->pipex);
		shell->pipex = NULL;
	}
}
