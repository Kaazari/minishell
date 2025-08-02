/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Traite l'entrée utilisateur avant l'exécution de la commande
 *
 * Cette fonction :
 * - Vérifie si un signal d'interruption (SIGINT) a été reçu
 * - Ajoute l'entrée à l'historique si elle n'est pas vide
 * - Vérifie si l'entrée est vide ou ne contient que des espaces
 * - Libère la mémoire de l'entrée si elle doit être ignorée
 *
 * @param shell: Structure shell contenant l'état
 * @param input: Ligne de commande saisie par l'utilisateur
 * @return 1 si l'entrée doit être ignorée, 0 sinon
 *
 * Note: Cette fonction gère la libération mémoire de input en cas de retour 1
 */
static int	handle_input_processing(t_shell *shell, char *input)
{
	if (g_signal_exit_status == 130)
	{
		shell->exit_status = 130;
		g_signal_exit_status = 0;
		free(input);
		return (1);
	}
	if (ft_strlen(input) > 0)
		add_history(input);
	if (process_empty_input(input))
	{
		free(input);
		return (1);
	}
	return (0);
}

/**
 * Boucle principale du minishell
 *
 * Cette fonction implémente la boucle infinie du shell qui :
 * - Affiche le prompt "minishell> "
 * - Lit l'entrée utilisateur avec readline
 * - Gère les signaux reçus pendant la lecture
 * - Traite l'entrée et exécute les commandes
 * - Gère la sortie propre du shell (Ctrl+D)
 *
 * Gestion des signaux :
 * - Si g_signal_exit_status != 0 et != 999 : met à jour shell->exit_status
 * - Si g_signal_exit_status == 999 : réinitialise le flag (processus enfant terminé)
 *
 * @param shell: Structure shell contenant l'état global
 *
 * Note: La fonction se termine proprement avec clean_exit en cas de Ctrl+D
 * ou si shell->state == 2 (signal SIGQUIT)
 */
void	main_shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (g_signal_exit_status != 0 && g_signal_exit_status != 999)
		{
			shell->exit_status = g_signal_exit_status;
			g_signal_exit_status = 0;
		}
		else if (g_signal_exit_status == 999)
		{
			g_signal_exit_status = 0;
		}
		if (!input)
			clean_exit(shell, 0);
		if (handle_input_processing(shell, input))
			continue ;
		process_command(shell, input);
		free(input);
		if (shell->state == 2)
			clean_exit(shell, 131);
	}
}
