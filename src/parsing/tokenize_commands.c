/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_commands.c                                :+:      :+:    :+:   */
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
static void	process_pipe_command(t_cmd_process *proc, int i)
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
static void	process_logical_command(t_cmd_process *proc, int i, int operator)
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
 * Traite tous les tokens pour séparer les commandes selon les pipes et opérateurs logiques
 *
 * Cette fonction parcourt tous les mots et :
 * - Détecte les pipes (|) pour séparer les commandes
 * - Détecte les opérateurs logiques (|| et &&)
 * - Crée des structures de commande pour chaque partie
 * - Traite la commande finale après le dernier séparateur
 *
 * @param words: Tableau de mots tokenisés
 * @param commands: Tableau de commandes à remplir
 * @param cmd_count: Compteur de commandes (mis à jour)
 *
 * Note: Cette fonction initialise les indices et la structure de traitement
 * puis traite chaque token pour construire les commandes
 */
static void	process_pipe_tokens(char **words, t_cmd **commands, int *cmd_count)
{
	int				i;
	int				start;
	int				cmd_idx;
	t_cmd_process	proc;

	i = 0;
	start = 0;
	cmd_idx = 0;
	proc.commands = commands;
	proc.words = words;
	proc.cmd_idx = &cmd_idx;
	proc.start = &start;
	proc.cmd_count = cmd_count;
	while (words[i])
	{
		if (words[i][0] == '|' && words[i][1] == '\0')
		{
			process_pipe_command(&proc, i);
			(*cmd_count)++;
		}
		else if (ft_strncmp(words[i], "||", 2) == 0)
		{
			process_logical_command(&proc, i, 1);
			(*cmd_count)++;
		}
		else if (ft_strncmp(words[i], "&&", 2) == 0)
		{
			process_logical_command(&proc, i, 2);
			(*cmd_count)++;
		}
		i++;
	}
	process_final_command(&proc, start, i);
	commands[cmd_idx] = NULL;
}

/**
 * Tokenise une ligne d'entrée en commandes séparées par des pipes et opérateurs logiques
 *
 * Cette fonction est le point d'entrée principal pour la tokenization :
 * - Tokenise d'abord l'entrée en mots individuels
 * - Vérifie la syntaxe des redirections et opérateurs
 * - Alloue le tableau de commandes
 * - Traite les tokens pour séparer les commandes
 * - Libère les ressources temporaires
 *
 * @param input: Ligne de commande saisie par l'utilisateur
 * @param cmd_count: Pointeur vers le compteur de commandes (mis à jour)
 * @param shell: Structure shell pour l'expansion des variables
 * @return Tableau de commandes ou NULL en cas d'erreur
 *
 * Note: Cette fonction gère la libération mémoire en cas d'erreur de syntaxe
 * ou d'allocation. Le tableau retourné doit être libéré par l'appelant.
 */
t_cmd	**tokenize_piped_commands(char *input, int *cmd_count, t_shell *shell)
{
	char	**words;
	t_cmd	**commands;

	words = tokenize_words(input, shell);
	if (!words)
		return (NULL);
	if (check_syntax_errors(words))
	{
		free_args(words);
		return (NULL);
	}
	commands = init_commands_array(words);
	if (!commands)
	{
		free_args(words);
		return (NULL);
	}
	*cmd_count = 1;
	process_pipe_tokens(words, commands, cmd_count);
	free_args(words);
	return (commands);
}
