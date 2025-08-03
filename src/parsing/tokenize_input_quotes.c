/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Traite les erreurs de quotes non fermées
 *
 * Cette fonction affiche un message d'erreur approprié selon le type
 * de quote non fermée et réinitialise la position du mot
 *
 * @param quote_state: Type de quote non fermée (' ou ")
 * @param data: Structure de données de tokenization
 */
void	handle_quote_error_input(char quote_state, t_token_data *data)
{
	if (quote_state == '\'')
		write(STDERR_FILENO, "minishell: Error: Unmatched single quotes\n", 42);
	else
		write(STDERR_FILENO, "minishell: Error: Unmatched double quotes\n", 42);
	*(data->word_pos) = 0;
}

/**
 * Traite la gestion des quotes
 *
 * Cette fonction utilise le contexte pour traiter les quotes
 * simples et doubles dans l'entrée
 *
 * @param ctx: Contexte de gestion des quotes
 */
void	process_quote_handling(t_quote_context *ctx)
{
	if (is_quote(ctx->input[*ctx->i]))
	{
		handle_quotes(ctx);
	}
}

/**
 * Gère le début d'une quote
 *
 * Cette fonction initialise l'état des quotes
 * quand une quote est rencontrée
 *
 * @param ctx: Contexte de gestion des quotes
 */
void	handle_quote_start(t_quote_context *ctx)
{
	*ctx->quote_state = ctx->input[*ctx->i];
	*ctx->quote_start = *ctx->i;
	(*ctx->i)++;
}

/**
 * Gère la fin d'une quote
 *
 * Cette fonction finalise l'état des quotes
 * quand une quote de fermeture est rencontrée
 *
 * @param ctx: Contexte de gestion des quotes
 */
void	handle_quote_end(t_quote_context *ctx)
{
	if (*ctx->quote_start == *ctx->i - 1)
	{
		*ctx->quote_state = 0;
		*ctx->quote_start = -1;
		(*ctx->i)++;
		return ;
	}
	*ctx->quote_state = 0;
	*ctx->quote_start = -1;
	(*ctx->i)++;
}

/**
 * Gère les quotes simples et doubles
 *
 * Cette fonction :
 * - Détecte le début d'une quote (simple ou double)
 * - Gère la fin d'une quote
 * - Ajoute les caractères dans les quotes au mot courant
 *
 * @param ctx: Contexte de gestion des quotes
 */
void	handle_quotes(t_quote_context *ctx)
{
	if (*ctx->quote_state == 0)
		handle_quote_start(ctx);
	else if (*ctx->quote_state == ctx->input[*ctx->i])
		handle_quote_end(ctx);
	else
	{
		add_char_to_word(ctx->data->current_word,
			ctx->data->word_pos, ctx->input[*ctx->i]);
		(*ctx->i)++;
	}
}
