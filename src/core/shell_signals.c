/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Gestionnaire de signaux pour le minishell
 *
 * Cette fonction gère les signaux reçus par le shell :
 * - SIGINT (Ctrl+C) : Interrompt la commande en cours et affiche un nouveau prompt
 * - SIGQUIT (Ctrl-\) : Termine le processus avec un statut d'erreur
 *
 * Comportement :
 * - Met à jour g_signal avec le numéro du signal reçu
 * - Pour SIGINT : affiche un retour à la ligne, vide la ligne de commande
 *   et redémarre le prompt (sauf si g_signal_exit_status == 999)
 * - Pour SIGQUIT : met à jour le statut de sortie à 131
 *
 * @param signo: Numéro du signal reçu
 *
 * Note: g_signal_exit_status == 999 indique qu'on est dans un processus enfant
 * et qu'il ne faut pas redémarrage le prompt
 */
void	signal_handler(int signo)
{
	g_signal = signo;
	if (signo == SIGINT)
	{
		if (g_signal_exit_status == 999)
			return ;
		g_signal_exit_status = 130;
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		if (g_signal_exit_status != 999)
			rl_redisplay();
	}
	else if (signo == SIGQUIT)
	{
		if (g_signal_exit_status == 999)
			return ;
		g_signal_exit_status = 131;
	}
}

/**
 * Configure les gestionnaires de signaux pour le minishell
 *
 * Cette fonction :
 * - Configure SIGINT (Ctrl+C) pour utiliser signal_handler
 * - Configure SIGQUIT (Ctrl-\) pour être ignoré dans le shell principal
 * - Utilise sigaction pour une gestion plus robuste que signal()
 *
 * Note: SIGQUIT est ignoré dans le shell principal mais sera géré dans les processus enfants
 * pour permettre l'affichage du message "Quit (core dumped)"
 */
void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}


