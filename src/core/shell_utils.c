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
	(void)shell;
	if (!input || ft_strlen(input) == 0)
	{
		free(input);
		return (1);
	}
	add_history(input);
	return (0);
}

/**
 * Boucle principale du minishell
 *
 * Cette fonction implémente la boucle infinie du shell qui :
 * - Affiche le prompt "minishell> "
 * - Lit l'entrée utilisateur avec readline
 * - Traite l'entrée et exécute les commandes
 * - Gère la sortie propre du shell (Ctrl+D)
 *
 * Gestion des signaux :
 * - Si g_signal_exit_status != 0 et != 999 : met à jour shell->exit_status
 * - Si g_signal_exit_status == 999 : réinitialise
 le flag (processus enfant terminé)
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
		if (!input)
		{
			printf("exit\n");
			exit(0);
		}
		if (handle_input_processing(shell, input))
			continue ;
		process_command(shell, input);
		free(input);
	}
}

/**
 * Détermine et exécute le type de commandes approprié
 *
 * Cette fonction analyse les commandes et choisit la méthode d'exécution :
 * - Commande simple : exécution directe
 * - Commandes avec opérateurs logiques : exécution conditionnelle
 * - Commandes avec pipes : exécution parallèle
 *
 * @param shell: Structure shell contenant l'état
 * @param commands: Tableau de commandes à exécuter
 * @param cmd_count: Nombre de commandes dans le tableau
 *
 * Note: Cette fonction met à jour shell->current_commands pour le suivi
 * et nettoie ces pointeurs après exécution
 */
void	handle_commands_execution(t_shell *shell, t_cmd **commands,
			int cmd_count)
{
	shell->current_commands = commands;
	shell->current_cmd_count = cmd_count;
	if (cmd_count == 1)
		handle_single_command(shell, commands);
	else if (has_logical_operators(commands, cmd_count))
		handle_logical_commands(shell, commands, cmd_count);
	else
		handle_piped_commands(shell, commands, cmd_count);
	shell->current_commands = NULL;
	shell->current_cmd_count = 0;
}
