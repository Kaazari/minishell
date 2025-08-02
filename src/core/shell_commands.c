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
		shell->cmd = commands[i];
		execute_shell_command(commands[i]->args, shell, 1);
		prev_exit_status = shell->exit_status;

		if (i + 1 < count && commands[i + 1])
		{
			if (commands[i]->logical_operator == 1)
			{
				if (prev_exit_status == 0)
					break ;
			}
			else if (commands[i]->logical_operator == 2)
			{
				if (prev_exit_status != 0)
					break ;
			}
		}
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
 * Note: Les heredocs sont prétraités pour éviter les problèmes avec les entrées
 * non-interactives. La structure pipex est allouée et libérée ici.
 */
void	handle_piped_commands(t_shell *shell, t_cmd **commands, int count)
{
	if (preprocess_all_heredocs(shell, commands, count) == -1)
	{
		free_partial_cmds(commands, count);
		return ;
	}

	shell->pipex = malloc(sizeof(t_pipex));
	if (!shell->pipex)
	{
		free_partial_cmds(commands, count);
		return ;
	}
	shell->pipex->cmd_count = count;
	shell->pipex->pipe_count = count - 1;

	execute_piped_commands(shell, commands, count);

	if (shell->pipex)
	{
		free(shell->pipex);
		shell->pipex = NULL;
	}

	free_partial_cmds(commands, count);
}

/**
 * Vérifie si des opérateurs logiques sont présents dans les commandes
 *
 * @param commands: Tableau de commandes à vérifier
 * @param cmd_count: Nombre de commandes dans le tableau
 * @return 1 si des opérateurs logiques sont trouvés, 0 sinon
 *
 * Note: logical_operator != 0 indique la présence d'un opérateur || ou &&
 */
static int	has_logical_operators(t_cmd **commands, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count && commands[i])
	{
		if (commands[i]->logical_operator != 0)
			return (1);
		i++;
	}
	return (0);
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
static void	handle_commands_execution(t_shell *shell, t_cmd **commands,
				int cmd_count)
{
	shell->current_commands = commands;
	shell->current_cmd_count = cmd_count;
	if (cmd_count == 1)
	{
		handle_single_command(shell, commands);
		shell->current_commands = NULL;
		shell->current_cmd_count = 0;
	}
	else if (has_logical_operators(commands, cmd_count))
	{
		handle_logical_commands(shell, commands, cmd_count);
		shell->current_commands = NULL;
		shell->current_cmd_count = 0;
	}
	else
	{
		handle_piped_commands(shell, commands, cmd_count);
		shell->current_commands = NULL;
		shell->current_cmd_count = 0;
	}
}

/**
 * Traite et exécute une ligne de commande saisie par l'utilisateur
 *
 * Cette fonction est le point d'entrée principal pour l'exécution des commandes :
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
 * Les commandes avec des arguments vides génèrent une erreur "command not found"
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

	if (!commands[0] || !commands[0]->args || !commands[0]->args[0] || ft_strlen(commands[0]->args[0]) == 0)
	{
		printf("minishell: : command not found\n");
		shell->exit_status = 127;
		free_partial_cmds(commands, cmd_count);
		return ;
	}

	handle_commands_execution(shell, commands, cmd_count);
}
