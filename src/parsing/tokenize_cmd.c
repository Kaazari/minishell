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
 * Note: En cas d'échec d'allocation, cette fonction libère les
 * ressources déjà allouées pour éviter les fuites mémoire
 */
static int	init_cmd_struct(t_cmd *cmd)
{
	if (init_cmd_args(cmd) == -1)
		return (-1);
	if (init_cmd_redirs(cmd) == -1)
		return (-1);
	init_cmd_defaults(cmd);
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
 * @return Pointeur vers la nouvelle
 structure de commande ou NULL en cas d'erreur
 *
 * Note: Cette fonction gère la libération mémoire en cas d'échec
 * d'initialisation
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

	if (ft_strlen(input) > 1000)
	{
		write(2, "minishell: input too large\n", 26);
		write(2, "\n", 1);
		return (NULL);
	}
	if (init_tokenize_struct(&init, current_word, counts) == -1)
		return (NULL);
	quote_state = 0;
	init_tokenize_data(&data, &init);
	process_input_chars(input, &data, &quote_state, shell);
	if (quote_state != 0)
		return (handle_quote_error(init.words));
	save_word(init.words, init.word_count, init.current_word,
		*(init.word_pos));
	init.words[*(init.word_count)] = NULL;
	return (init.words);
}
