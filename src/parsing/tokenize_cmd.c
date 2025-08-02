/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:00:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Initialise la structure de commande avec les valeurs par défaut
 *
 * Cette fonction :
 * - Alloue le tableau d'arguments (64 éléments maximum)
 * - Alloue le tableau de redirections (10 éléments maximum)
 * - Initialise tous les pointeurs à NULL
 * - Initialise les compteurs et états à 0
 * - Définit les sauvegardes de descripteurs à -1
 *
 * @param cmd: Structure de commande à initialiser
 * @return 0 en cas de succès, -1 en cas d'échec d'allocation
 *
 * Note: En cas d'échec d'allocation, cette fonction libère les ressources
 * déjà allouées pour éviter les fuites mémoire
 */
static int	init_cmd_struct(t_cmd *cmd)
{
	int	i;

	cmd->args = malloc(sizeof(char *) * 64);
	if (!cmd->args)
	{
		cmd->redirs = NULL;
		return (-1);
	}
	i = 0;
	while (i < 64)
	{
		cmd->args[i] = NULL;
		i++;
	}
	cmd->redirs = malloc(sizeof(t_redir) * 10);
	if (!cmd->redirs)
	{
		free(cmd->args);
		cmd->args = NULL;
		return (-1);
	}
	cmd->redir_count = 0;
	cmd->stdin_backup = -1;
	cmd->stdout_backup = -1;
	cmd->logical_operator = 0;
	return (0);
}

/**
 * Crée et initialise une nouvelle structure de commande
 *
 * Cette fonction :
 * - Alloue la mémoire pour la structure t_cmd
 * - Initialise tous les champs avec init_cmd_struct
 * - Gère les erreurs d'allocation
 *
 * @return Pointeur vers la nouvelle structure de commande ou NULL en cas d'erreur
 *
 * Note: Cette fonction gère la libération mémoire en cas d'échec d'initialisation
 */
t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	if (init_cmd_struct(cmd) == -1)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

/**
 * Initialise les données de tokenization pour le traitement des mots
 *
 * Cette fonction :
 * - Configure les pointeurs vers les tableaux et compteurs
 * - Initialise les compteurs à 0
 * - Nettoie le buffer de mot courant
 *
 * @param data: Structure de données de tokenization à initialiser
 * @param init: Structure d'initialisation contenant les pointeurs
 *
 * Note: Cette fonction utilise memset pour nettoyer le buffer de mot
 */
static void	init_tokenize_data(t_token_data *data, t_tokenize_init *init)
{
	data->words = init->words;
	data->word_count = init->word_count;
	data->current_word = init->current_word;
	data->word_pos = init->word_pos;
	*(init->word_count) = 0;
	*(init->word_pos) = 0;
	memset(init->current_word, 0, 1000);
}

/**
 * Tokenise une ligne d'entrée en mots individuels
 *
 * Cette fonction est le point d'entrée principal pour la tokenization :
 * - Alloue le tableau de mots (1000 éléments maximum)
 * - Traite chaque caractère de l'entrée avec process_input_chars
 * - Gère les quotes et l'expansion des variables
 * - Sauvegarde le dernier mot si nécessaire
 * - Vérifie que toutes les quotes sont fermées
 *
 * @param input: Ligne de commande saisie par l'utilisateur
 * @param shell: Structure shell pour l'expansion des variables
 * @return Tableau de mots tokenisés ou NULL en cas d'erreur
 *
 * Note: Cette fonction gère la libération mémoire en cas d'erreur de quotes
 * non fermées. Le tableau retourné doit être libéré par l'appelant.
 */
char	**tokenize_words(char *input, t_shell *shell)
{
	char			current_word[1000];
	int				counts[2];
	char			quote_state;
	t_token_data	data;
	t_tokenize_init	init;

	init.words = malloc(sizeof(char *) * 1000);
	if (!init.words)
		return (NULL);
	quote_state = 0;
	init.current_word = current_word;
	init.word_count = &counts[0];
	init.word_pos = &counts[1];
	init_tokenize_data(&data, &init);
	process_input_chars(input, &data, &quote_state, shell);

	if (quote_state != 0)
	{
		free(init.words);
		return (NULL);
	}

	if (counts[1] > 0)
		save_word(init.words, &counts[0], current_word, counts[1]);
	init.words[counts[0]] = NULL;
	return (init.words);
}
