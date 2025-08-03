/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input_iteration.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_quote_start_iteration(t_char_iteration_context *ctx, int *i)
{
	*ctx->quote_state = ctx->input[*i];
	(*i)++;
}

static void	handle_quote_end_iteration(t_char_iteration_context *ctx, int *i)
{
	*ctx->quote_state = 0;
	(*i)++;
}

static void	handle_quote_content_iteration(t_char_iteration_context *ctx,
		int *i)
{
	add_char_to_word(ctx->data->current_word, ctx->data->word_pos,
		ctx->input[*i]);
	(*i)++;
}

void	handle_quote_in_iteration(t_char_iteration_context *ctx, int *i)
{
	if (*ctx->quote_state == 0)
		handle_quote_start_iteration(ctx, i);
	else if (*ctx->quote_state == ctx->input[*i])
		handle_quote_end_iteration(ctx, i);
	else
		handle_quote_content_iteration(ctx, i);
	*ctx->i = *i;
}

void	handle_backslash_in_iteration(t_char_iteration_context *ctx, int *i)
{
	add_char_to_word(ctx->data->current_word, ctx->data->word_pos, '\\');
	(*i)++;
	if (ctx->input[*i])
	{
		add_char_to_word(ctx->data->current_word, ctx->data->word_pos,
			ctx->input[*i]);
		(*i)++;
	}
	*ctx->i = *i;
}
