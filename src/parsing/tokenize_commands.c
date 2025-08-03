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
 * Traite tous les tokens pour séparer les commandes selon les pipes
 * et opérateurs logiques
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
			handle_pipe_token_cmd(&proc, i);
		else if (ft_strncmp(words[i], "||", 2) == 0)
			handle_logical_token_cmd(&proc, i, 1);
		else if (ft_strncmp(words[i], "&&", 2) == 0)
			handle_logical_token_cmd(&proc, i, 2);
		i++;
	}
	process_final_command(&proc, start, i);
	commands[cmd_idx] = NULL;
}

/**
 * Tokenise une ligne d'entrée en commandes séparées par des pipes
 * et opérateurs logiques
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
