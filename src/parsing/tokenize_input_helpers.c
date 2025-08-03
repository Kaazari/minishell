/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:00:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Gère l'expansion des variables d'environnement
 *
 * Cette fonction :
 * - Détecte le caractère '$' pour identifier une variable
 * - Extrait le nom de la variable
 * - Récupère sa valeur depuis l'environnement
 * - Développe la valeur dans le mot courant
 * - Gère le cas d'une variable vide ($)
 *
 * @param input: Chaîne d'entrée à traiter
 * @param i: Pointeur vers l'index de position dans l'entrée
 * @param data: Structure de données de tokenization
 * @param shell: Structure shell contenant l'environnement
 */
void	handle_variable_expansion(char *input, int *i, t_token_data *data,
		t_shell *shell)
{
	t_var_expansion	var_exp;
	static int		recursion_count = 0;

	if (recursion_count > 3)
	{
		write(2, "minishell: too many recursive expansions\n", 39);
		write(2, "\n", 1);
		(*i)++;
		return ;
	}
	recursion_count++;
	var_exp.input = input;
	var_exp.i = *i;
	var_exp.word = data->current_word;
	var_exp.pos = data->word_pos;
	var_exp.depth = 0;
	var_exp.expanding_vars = NULL;
	var_exp.expanding_count = 0;
	*i = add_variable_to_word(&var_exp, shell);
	recursion_count--;
	(*i)++;
}

/**
 * Gère l'échappement des backslashes
 *
 * Cette fonction :
 * - Détecte le caractère backslash '\'
 * - Lit le caractère suivant
 * - L'ajoute au mot courant sans traitement spécial
 *
 * @param input: Chaîne d'entrée à traiter
 * @param i: Pointeur vers l'index de position dans l'entrée
 * @param data: Structure de données de tokenization
 */
void	handle_backslash(char *input, int *i, t_token_data *data)
{
	(*i)++;
	if (input[*i])
	{
		add_char_to_word(data->current_word, data->word_pos, input[*i]);
		(*i)++;
	}
}

void	handle_variable_in_iteration(t_char_iteration_context *ctx, int *i)
{
	handle_variable_expansion(ctx->input, i, ctx->data, ctx->shell);
	*ctx->i = *i;
}
