/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Exécute une commande simple (sans pipes ni opérateurs logiques)
 *
 * Cette fonction :
 * - Définit la commande courante dans shell->cmd
 * - Exécute la commande avec ses arguments
 * - Nettoie la commande après exécution
 * - Libère la mémoire des structures de commande
 *
 * @param shell: Structure shell contenant l'état
 * @param commands: Tableau de commandes (seule la première est utilisée)
 *
 * Note: Cette fonction gère les redirections car shell->cmd est défini
 * avant l'exécution de execute_shell_command
 */
void	handle_single_command(t_shell *shell, t_cmd **commands)
{
	if (preprocess_all_heredocs(shell, commands, 1) == -1)
	{
		free_partial_cmds(commands, 1);
		return ;
	}
	shell->cmd = commands[0];
	execute_shell_command(commands[0]->args, shell, 1);
	shell->cmd = NULL;
	free_partial_cmds(commands, 1);
}

/**
 * Exécute des commandes avec opérateurs logiques (|| et &&)
 *
 * Cette fonction implémente la logique des opérateurs logiques :
 * - || (OR) : exécute la commande suivante seulement si la précédente échoue
 * - && (AND) : exécute la commande suivante seulement si la précédente réussit
 *
 * Comportement :
 * - Exécute chaque commande séquentiellement
 * - Vérifie le statut de sortie après chaque commande
 * - Décide d'exécuter la suivante selon l'opérateur logique
 * - Arrête l'exécution si la condition n'est pas remplie
 *
 * @param shell: Structure shell contenant l'état
 * @param commands: Tableau de commandes à exécuter
 * @param count: Nombre de commandes dans le tableau
 *
 * Note: logical_operator = 1 pour ||, logical_operator = 2 pour &&
 */
void	handle_logical_commands(t_shell *shell, t_cmd **commands, int count)
{
	int	i;
	int	prev_exit_status;

	i = 0;
	while (i < count && commands[i])
	{
		prev_exit_status = execute_logical_command(shell, commands, i);
		if (!should_continue_logical(commands, i, prev_exit_status))
			break ;
		i++;
	}
	shell->cmd = NULL;
	free_partial_cmds(commands, count);
}

/**
 * Exécute des commandes avec des pipes (|)
 *
 * Cette fonction :
 * - Prétraite tous les heredocs avant l'exécution des pipes
 * - Initialise la structure pipex pour gérer les pipes
 * - Exécute les commandes en parallèle avec des pipes
 * - Nettoie les ressources après exécution
 *
 * @param shell: Structure shell contenant l'état
 * @param commands: Tableau de commandes à exécuter
 * @param count: Nombre de commandes dans le tableau
 *
 * Note: Les heredocs sont prétraités pour éviter les problèmes
 * avec les entrées non-interactives.
 */
void	handle_piped_commands(t_shell *shell, t_cmd **commands, int count)
{
	if (preprocess_all_heredocs(shell, commands, count) == -1)
	{
		free_partial_cmds(commands, count);
		return ;
	}
	if (!init_pipex_for_commands(shell, commands, count))
		return ;
	execute_piped_commands(shell, commands, count);
	cleanup_pipex_after_execution(shell);
	free_partial_cmds(commands, count);
}

/**
 * Traite et exécute une ligne de commande saisie par l'utilisateur
 *
 * Cette fonction est le point d'entrée principal
 pour l'exécution des commandes :
 * - Vérifie si l'entrée est valide (non vide)
 * - Tokenise l'entrée en commandes séparées
 * - Vérifie la syntaxe et la validité des commandes
 * - Détermine le type d'exécution approprié
 * - Gère les erreurs de commande (commande vide, commande non trouvée)
 *
 * @param shell: Structure shell contenant l'état
 * @param input: Ligne de commande saisie par l'utilisateur
 *
 * Note: Les commandes vides ou ne contenant que des espaces sont ignorées.
 * Les commandes avec des arguments vides génèrent une erreur.
 */
void	process_command(t_shell *shell, char *input)
{
	int		cmd_count;
	t_cmd	**commands;

	if (!input || ft_strlen(input) == 0)
		return ;
	commands = tokenize_piped_commands(input, &cmd_count, shell);
	if (!commands)
		return ;
	if (!is_valid_command(commands, cmd_count))
	{
		handle_command_not_found(commands, cmd_count, shell);
		return ;
	}
	handle_commands_execution(shell, commands, cmd_count);
}
