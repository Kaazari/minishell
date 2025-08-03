/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helpers_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:00:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Vérifie si un mot est vide (contient seulement des espaces)
 *
 * @param word: Le mot à vérifier
 * @param len: La longueur du mot
 * @return 1 si le mot est vide, 0 sinon
 */
int	is_word_empty(char *word, int len)
{
	int	i;

	i = 0;
	while (i < len && (word[i] == ' ' || word[i] == '\t'
			|| word[i] == '\n' || word[i] == '\r'))
		i++;
	return (i == len);
}

/**
 * Vérifie si un mot est juste des guillemets vides
 *
 * @param word: Le mot à vérifier
 * @param len: La longueur du mot
 * @return 1 si le mot est juste des guillemets vides, 0 sinon
 */
int	is_empty_quotes(char *word, int len)
{
	return ((len == 2 && word[0] == '"' && word[1] == '"')
		|| (len == 2 && word[0] == '\'' && word[1] == '\''));
}

/**
 * Crée une copie du mot et l'ajoute au tableau
 *
 * @param words: Tableau de mots
 * @param count: Compteur de mots
 * @param word: Le mot à copier
 * @param len: La longueur du mot
 */
void	add_word_to_array(char **words, int *count, char *word, int len)
{
	char	*word_copy;

	word_copy = malloc(len + 1);
	if (!word_copy)
		return ;
	ft_strlcpy(word_copy, word, len + 1);
	words[*count] = word_copy;
	(*count)++;
}
