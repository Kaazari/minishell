/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <termios.h>
#include <unistd.h>

/**
 * Nettoie la structure pipex et ses ressources
 *
 * Cette fonction :
 * - Libère le tableau de descripteurs de fichiers des pipes
 * - Libère la structure pipex elle-même
 * - Met le pointeur à NULL pour éviter les doubles libérations
 *
 * @param shell: Structure shell contenant la structure pipex à nettoyer
 *
 * Note: Cette fonction vérifie l'existence des pointeurs avant de les libérer
 * pour éviter les segfaults
 */
void	cleanup_pipex(t_shell *shell)
{
	if (shell && shell->pipex)
	{
		if (shell->pipex->pipe_fds)
			free(shell->pipex->pipe_fds);
		free(shell->pipex);
		shell->pipex = NULL;
	}
}

/**
 * Nettoie la commande courante et ses ressources
 *
 * Cette fonction :
 * - Libère la structure de commande et tous ses champs
 * - Met le pointeur à NULL pour éviter les doubles libérations
 *
 * @param shell: Structure shell contenant la commande à nettoyer
 *
 * Note: Cette fonction utilise free_cmd qui libère récursivement tous les champs
 * de la structure de commande (arguments, redirections, etc.)
 */
void	cleanup_cmd(t_shell *shell)
{
	if (shell && shell->cmd)
	{
		free_cmd(shell->cmd);
		shell->cmd = NULL;
	}
}

/**
 * Nettoie toutes les variables d'environnement et les chemins
 *
 * Cette fonction libère de manière sécurisée :
 * - L'environnement local (local_envp)
 * - L'environnement système (envp)
 * - Le répertoire de travail actuel (pwd)
 * - L'ancien répertoire de travail (oldpwd)
 *
 * @param shell: Structure shell contenant les variables à nettoyer
 *
 * Note: Cette fonction libère d'abord chaque chaîne du tableau, puis le tableau
 * lui-même. Elle met tous les pointeurs à NULL après libération.
 */
void	cleanup_envp(t_shell *shell)
{
	char	**tmp;

	if (shell && shell->local_envp)
	{
		tmp = shell->local_envp;
		while (*tmp)
			free(*tmp++);
		free(shell->local_envp);
		shell->local_envp = NULL;
	}
	if (shell && shell->envp)
	{
		tmp = shell->envp;
		while (*tmp)
			free(*tmp++);
		free(shell->envp);
		shell->envp = NULL;
	}
	if (shell && shell->pwd)
	{
		free(shell->pwd);
		shell->pwd = NULL;
	}
	if (shell && shell->oldpwd)
	{
		free(shell->oldpwd);
		shell->oldpwd = NULL;
	}
}

/**
 * Restaure les paramètres du terminal à leur état normal
 *
 * Cette fonction :
 * - Vide l'historique des commandes
 * - Restaure l'affichage du terminal
 * - Remet les flags du terminal en mode canonique
 * - Active l'écho des caractères
 *
 * Note: Cette fonction est appelée lors de la sortie propre du shell
 * pour restaurer l'état du terminal tel qu'il était avant l'exécution
 */
void	cleanup_terminal_resources(void)
{
	struct termios	term;

	rl_clear_history();
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	if (tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_lflag |= ECHO | ICANON;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
}
