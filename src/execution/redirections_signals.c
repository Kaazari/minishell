/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_signals.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Gestionnaire de signal pour les heredocs
 *
 * Cette fonction :
 * - Met à jour g_signal_exit_status avec le code 130 (SIGINT)
 * - Affiche un retour à la ligne
 * - Ferme l'entrée standard pour interrompre la lecture
 *
 * @param signo: Numéro du signal reçu
 */
static void	heredoc_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal_exit_status = 130;
		write(STDOUT_FILENO, "\n", 1);
		close(STDIN_FILENO);
	}
}

/**
 * Configure les signaux pour les heredocs
 *
 * Cette fonction :
 * - Configure le gestionnaire de signal pour SIGINT
 * - Ignore SIGQUIT pendant les heredocs
 * - Utilise SA_RESTART pour éviter l'interruption des appels système
 */
void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = heredoc_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * Restaure les signaux principaux après un heredoc
 *
 * Cette fonction :
 * - Remet le gestionnaire de signal principal pour SIGINT
 * - Ignore SIGQUIT comme dans le shell principal
 */
void	restore_main_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
