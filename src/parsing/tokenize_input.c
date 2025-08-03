/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:00:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_logical_operators(char *input, int *i, t_token_data *data)
{
	if (input[*i] == '|' && input[*i + 1] == '|')
	{
		handle_logical_token(data, input, i);
		return ;
	}
	else if (input[*i] == '&' && input[*i + 1] == '&')
	{
		handle_logical_token(data, input, i);
		return ;
	}
}

static void	handle_pipe_and_redir(char *input, int *i, t_token_data *data)
{
	if (input[*i] == '|')
	{
		handle_pipe_token(data, i);
		return ;
	}
	else if (input[*i] == '>' || input[*i] == '<')
	{
		handle_redirection_token(data, input, i);
		return ;
	}
}

static void	handle_special_chars(char *input, int *i, t_token_data *data,
		char quote_state)
{
	if (quote_state == 0)
	{
		handle_logical_operators(input, i, data);
		handle_pipe_and_redir(input, i, data);
		if (is_separator(input[*i]) && !quote_state)
			handle_separator_token(data, i);
		else if (!is_quote(input[*i]))
		{
			add_char_to_word(data->current_word, data->word_pos, input[*i]);
			(*i)++;
		}
		else
			(*i)++;
	}
	else
	{
		add_char_to_word(data->current_word, data->word_pos, input[*i]);
		(*i)++;
	}
}

/**
 * Traite chaque caractère de l'entrée pour la tokenization
 *
 * Cette fonction est le cœur de la tokenization et gère :
 * - L'expansion des variables d'environnement ($VAR)
 * - Les quotes simples et doubles avec leur contenu
 * - Les quotes vides (ignorées complètement)
 * - L'échappement des backslashes dans les quotes
 * - Les caractères spéciaux (redirections, pipes, opérateurs logiques)
 * - Les erreurs de quotes non fermées
 *
 * Comportement des quotes :
 * - Les quotes vides sont complètement ignorées
 * - Les variables sont expansées dans les quotes doubles, pas dans les simples
 * - Les backslashes échappent les caractères dans les quotes
 *
 * @param input: Ligne de commande à traiter
 * @param data: Structure de données de tokenization
 * @param quote_state: État des quotes (mis à jour)
 * @param shell: Structure shell pour l'expansion des variables
 *
 * Note: Cette fonction affiche des messages d'erreur pour les quotes non fermées
 * et réinitialise la position du mot pour éviter les tokens invalides
 */
static void	process_char_iteration(t_char_iteration_context *ctx)
{
	int	i;

	i = *ctx->i;
	if (ctx->input[i] == '$' && *ctx->quote_state != '\'')
	{
		handle_variable_in_iteration(ctx, &i);
		return ;
	}
	if (is_quote(ctx->input[i]))
	{
		handle_quote_in_iteration(ctx, &i);
		return ;
	}
	if (ctx->input[i] == '\\' && *ctx->quote_state != 0)
	{
		handle_backslash_in_iteration(ctx, &i);
		return ;
	}
	handle_special_chars(ctx->input, &i, ctx->data, *ctx->quote_state);
	*ctx->i = i;
}

void	process_input_chars(char *input, t_token_data *data, char *quote_state,
		t_shell *shell)
{
	t_char_iteration_context	iter_ctx;
	int							i;
	int							quote_start;

	i = 0;
	quote_start = -1;
	while (input[i])
	{
		iter_ctx.input = input;
		iter_ctx.data = data;
		iter_ctx.quote_state = quote_state;
		iter_ctx.shell = shell;
		iter_ctx.i = &i;
		iter_ctx.quote_start = &quote_start;
		process_char_iteration(&iter_ctx);
	}
	if (*quote_state != 0)
		handle_quote_error_input(*quote_state, data);
}
