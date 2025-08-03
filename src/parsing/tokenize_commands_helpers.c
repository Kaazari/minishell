/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_commands_helpers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Traite une commande séparée par un pipe (|)
 *
 * Cette fonction :
 * - Extrait les mots de la commande entre start et i
 * - Crée une nouvelle structure de commande
 * - Traite les mots pour séparer arguments et redirections
 * - Met à jour les indices pour la prochaine commande
 *
 * @param proc: Structure contenant les données de traitement
 * @param i: Index du token pipe dans le tableau de mots
 *
 * Note: Cette fonction gère la libération mémoire en cas d'échec d'allocation
 */
void	process_pipe_command(t_cmd_process *proc, int i)
{
	char	**cmd_words;

	cmd_words = create_cmd_words(proc->words, *(proc->start), i);
	if (!cmd_words)
		return ;
	proc->commands[*(proc->cmd_idx)] = create_cmd();
	if (!proc->commands[*(proc->cmd_idx)])
	{
		free_args(cmd_words);
		free_partial_cmds(proc->commands, *(proc->cmd_idx));
		proc->commands = NULL;
		return ;
	}
	process_command_words(proc->commands[*(proc->cmd_idx)], cmd_words);
	free_args(cmd_words);
	*(proc->start) = i + 1;
	(*(proc->cmd_idx))++;
}

/**
 * Traite une commande avec opérateur logique (|| ou &&)
 *
 * Cette fonction :
 * - Extrait les mots de la commande entre start et i
 * - Crée une nouvelle structure de commande
 * - Définit l'opérateur logique (1 pour ||, 2 pour &&)
 * - Traite les mots pour séparer arguments et redirections
 * - Met à jour les indices pour la prochaine commande
 *
 * @param proc: Structure contenant les données de traitement
 * @param i: Index du token logique dans le tableau de mots
 * @param operator: Type d'opérateur (1 pour ||, 2 pour &&)
 *
 * Note: Cette fonction gère la libération mémoire en cas d'échec d'allocation
 */
void	process_logical_command(t_cmd_process *proc, int i, int operator)
{
	char	**cmd_words;

	cmd_words = create_cmd_words(proc->words, *(proc->start), i);
	if (!cmd_words)
		return ;
	proc->commands[*(proc->cmd_idx)] = create_cmd();
	if (!proc->commands[*(proc->cmd_idx)])
	{
		free_args(cmd_words);
		free_partial_cmds(proc->commands, *(proc->cmd_idx));
		proc->commands = NULL;
		return ;
	}
	process_command_words(proc->commands[*(proc->cmd_idx)], cmd_words);
	proc->commands[*(proc->cmd_idx)]->logical_operator = operator;
	free_args(cmd_words);
	*(proc->start) = i + 1;
	(*(proc->cmd_idx))++;
}

/**
 * Traite un token pipe (|)
 *
 * @param proc: Structure contenant les données de traitement
 * @param i: Index du token pipe
 */
void	handle_pipe_token_cmd(t_cmd_process *proc, int i)
{
	process_pipe_command(proc, i);
	(*(proc->cmd_count))++;
}

/**
 * Traite un token opérateur logique (|| ou &&)
 *
 * @param proc: Structure contenant les données de traitement
 * @param i: Index du token logique
 * @param operator: Type d'opérateur (1 pour ||, 2 pour &&)
 */
void	handle_logical_token_cmd(t_cmd_process *proc, int i, int operator)
{
	process_logical_command(proc, i, operator);
	(*(proc->cmd_count))++;
}
