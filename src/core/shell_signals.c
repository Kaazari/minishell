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
 * - SIGINT (Ctrl+C) : Interrompt la commande en cours
 et affiche un nouveau prompt
 * - SIGQUIT (Ctrl-\) : Termine le processus avec un statut d'erreur
 *
 * Comportement :
 * - Met à jour g_signal_exit_status avec le statut du signal
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
	if (signo == SIGINT)
	{
		if (g_signal_exit_status == 999)
			return ;
		g_signal_exit_status = 130;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
	{
		if (g_signal_exit_status != 999)
		{
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		}
		g_signal_exit_status = 131;
	}
}

/**
 * Configure les gestionnaires de signaux pour le minishell
 *
 * Cette fonction :
 * - Configure SIGINT (Ctrl+C) pour utiliser signal_handler
 * - Configure SIGQUIT (Ctrl-\) pour être ignoré dans le shell principal
 * - Utilise signal() comme dans le code de référence
 *
 * Note: SIGQUIT est ignoré dans le shell
 * principal mais sera géré dans les processus enfants
 * pour permettre l'affichage du message "Quit (core dumped)"
 */
void	setup_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}
