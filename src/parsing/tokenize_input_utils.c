/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Extrait le nom d'une variable d'environnement depuis l'entrée
 *
 * Cette fonction :
 * - Avance l'index après le caractère '$'
 * - Lit tous les caractères alphanumériques et underscores
 * - S'arrête au premier caractère non valide
 *
 * @param input: Chaîne d'entrée à traiter
 * @param i: Pointeur vers l'index de position dans l'entrée
 *
 * Note: Cette fonction modifie directement l'index i
 */
void	extract_var_name(char *input, int *i)
{
	(*i)++;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
}

/**
 * Gère le cas d'une variable vide ($)
 *
 * Cette fonction ajoute simplement le caractère '$' au mot courant
 * quand aucune variable n'est spécifiée après le '$'
 *
 * @param data: Structure de données de tokenization
 */
void	handle_empty_var(t_token_data *data)
{
	add_char_to_word(data->current_word, data->word_pos, '$');
}

/**
 * Développe la valeur d'une variable dans le mot courant
 *
 * Cette fonction :
 * - Parcourt chaque caractère de la valeur de la variable
 * - L'ajoute au mot courant via add_char_to_word
 *
 * @param var_value: Valeur de la variable à développer
 * @param data: Structure de données de tokenization
 */
void	expand_var_value(char *var_value, t_token_data *data)
{
	while (*var_value)
	{
		add_char_to_word(data->current_word, data->word_pos, *var_value);
		var_value++;
	}
}

/**
 * Traite l'expansion des variables d'environnement
 *
 * Cette fonction utilise le contexte pour traiter l'expansion
 * des variables d'environnement dans l'entrée
 *
 * @param ctx: Contexte d'expansion des variables
 */
void	process_variable_expansion(t_var_expansion_context *ctx)
{
	if (ctx->input[*ctx->i] == '$' && *ctx->quote_state != '\'')
	{
		handle_variable_expansion(ctx->input, ctx->i, ctx->data, ctx->shell);
	}
}

/**
 * Traite l'échappement des backslashes
 *
 * Cette fonction gère l'échappement des backslashes
 * dans les quotes
 *
 * @param input: Chaîne d'entrée à traiter
 * @param i: Pointeur vers l'index de position dans l'entrée
 * @param data: Structure de données de tokenization
 * @param quote_state: État des quotes
 */
void	process_backslash_handling(char *input, int *i, t_token_data *data,
		char *quote_state)
{
	if (input[*i] == '\\' && *quote_state != 0)
	{
		handle_backslash(input, i, data);
	}
}
