/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Variable globale pour gérer les signaux reçus par le shell */
volatile sig_atomic_t	g_signal = 0;
/* Variable globale pour stocker le statut de sortie lors de l'interruption par signal */
int	g_signal_exit_status = 0;

/**
 * Initialise la structure shell avec les valeurs par défaut et l'environnement
 *
 * Cette fonction :
 * - Duplique l'environnement système dans shell->envp
 * - Initialise les pointeurs à NULL (local_envp, cmd, current_commands)
 * - Alloue la mémoire pour la structure pipex
 * - Récupère le répertoire de travail actuel
 * - Initialise les compteurs et états à 0
 *
 * @param shell: Structure shell à initialiser
 * @param envp: Environnement système à dupliquer
 *
 * Note: En cas d'échec d'allocation de pipex, le programme se termine avec exit(1)
 */
static void	init_shell(t_shell *shell, char **envp)
{
	shell->envp = dup_envp(envp);
	shell->local_envp = NULL;
	shell->state = 0;
	shell->exit_status = 0;
	shell->pipex = malloc(sizeof(t_pipex));
	if (!shell->pipex)
	{
		printf("Error: Failed to allocate memory for pipex\n");
		exit(1);
	}
	shell->pipex->pipe_fds = NULL;
	shell->cmd = NULL;
	shell->current_commands = NULL;
	shell->current_cmd_count = 0;
	shell->pwd = getcwd(NULL, 0);
	shell->oldpwd = NULL;
}

/**
 * Point d'entrée principal du minishell
 *
 * Cette fonction :
 * - Configure les gestionnaires de signaux (Ctrl+C, Ctrl-D, Ctrl-\)
 * - Initialise la structure shell avec l'environnement
 * - Lance la boucle principale du shell
 * - Nettoie les ressources avant de terminer
 *
 * @param ac: Nombre d'arguments (non utilisé)
 * @param av: Tableau d'arguments (non utilisé)
 * @param envp: Environnement système
 * @return 0 en cas de succès
 *
 * Note: Les paramètres ac et av sont ignorés car le shell ne prend pas d'arguments
 */
int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	setup_signals();
	init_shell(&shell, envp);
	main_shell_loop(&shell);
	clean_exit(&shell, 0);
	return (0);
}
