/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Nettoie les commandes et la structure pipex
 *
 * @param shell: Structure shell à nettoyer
 */
static void	cleanup_commands_and_pipex(t_shell *shell)
{
	if (shell && shell->cmd)
	{
		free_cmd(shell->cmd);
		shell->cmd = NULL;
	}
	if (shell && shell->pipex)
	{
		if (shell->pipex->pipe_fds)
			free(shell->pipex->pipe_fds);
		free(shell->pipex);
		shell->pipex = NULL;
	}
	if (shell->current_commands)
	{
		free_partial_cmds(shell->current_commands, shell->current_cmd_count);
		shell->current_commands = NULL;
		shell->current_cmd_count = 0;
	}
}

/**
 * Nettoie les environnements et les chemins
 *
 * @param shell: Structure shell à nettoyer
 */
static void	cleanup_environments_and_paths(t_shell *shell)
{
	if (shell->envp)
	{
		free_envp(shell->envp);
		shell->envp = NULL;
	}
	if (shell->local_envp)
	{
		free_envp(shell->local_envp);
		shell->local_envp = NULL;
	}
	if (shell->pwd)
	{
		free(shell->pwd);
		shell->pwd = NULL;
	}
	if (shell->oldpwd)
	{
		free(shell->oldpwd);
		shell->oldpwd = NULL;
	}
}

/**
 * Force le nettoyage de toutes les ressources allouées par le shell
 *
 * Cette fonction libère de manière sécurisée :
 * - La commande courante (shell->cmd) et ses ressources
 * - La structure pipex et ses descripteurs de fichiers
 * - Les commandes en cours d'exécution (current_commands)
 * - L'environnement local et les variables
 *
 * @param shell: Structure shell à nettoyer
 *
 * Note: Cette fonction vérifie l'existence des pointeurs avant de les libérer
 * pour éviter les segfaults. Elle met les pointeurs à NULL après libération.
 */
static void	force_cleanup(t_shell *shell)
{
	cleanup_commands_and_pipex(shell);
	cleanup_environments_and_paths(shell);
}

/**
 * Termine proprement le minishell avec nettoyage complet
 *
 * Cette fonction :
 * - Nettoie toutes les ressources allouées par le shell
 * - Vide l'historique des commandes
 * - Restaure les paramètres du terminal
 * - Termine le processus avec le statut spécifié
 *
 * @param shell: Structure shell à nettoyer
 * @param status: Code de sortie du programme
 *
 * Note: Cette fonction est appelée lors de la sortie normale du shell (Ctrl+D)
 * ou lors de la réception d'un signal de terminaison (SIGQUIT)
 */
void	clean_exit(t_shell *shell, int status)
{
	if (shell)
	{
		force_cleanup(shell);
	}
	clear_history();
	cleanup_terminal_resources();
	exit(status);
}
