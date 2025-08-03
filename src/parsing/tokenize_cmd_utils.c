/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_cmd_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
void	init_tokenize_data(t_token_data *data, t_tokenize_init *init)
{
	data->words = init->words;
	data->word_count = init->word_count;
	data->current_word = init->current_word;
	data->word_pos = init->word_pos;
	data->syntax_error = 0;
	*(init->word_count) = 0;
	*(init->word_pos) = 0;
	memset(init->current_word, 0, 1000);
}

/**
 * Initialise la structure d'initialisation pour la tokenization
 *
 * @param init: Structure à initialiser
 * @param current_word: Buffer pour le mot courant
 * @param counts: Tableau de compteurs
 * @return 0 en cas de succès, -1 en cas d'échec d'allocation
 */
int	init_tokenize_struct(t_tokenize_init *init, char *current_word,
		int *counts)
{
	init->words = malloc(sizeof(char *) * 1000);
	if (!init->words)
		return (-1);
	init->current_word = current_word;
	init->word_count = &counts[0];
	init->word_pos = &counts[1];
	return (0);
}

/**
 * Traite les erreurs de quotes non fermées
 *
 * @param words: Tableau de mots à libérer
 * @return NULL (toujours)
 */
char	**handle_quote_error(char **words)
{
	free(words);
	return (NULL);
}
