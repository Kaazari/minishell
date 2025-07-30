/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_redirection_token(t_token_data *data, char *input, int *i)
{
	if (*(data->word_pos) > 0)
	{
		save_word(data->words, data->word_count, data->current_word,
			*(data->word_pos));
		*(data->word_pos) = 0;
	}
	if (input[*i] == input[*i + 1])
	{
		if (input[*i] == '>')
			strcpy(data->current_word, ">>");
		else
			strcpy(data->current_word, "<<");
		save_word(data->words, data->word_count, data->current_word, 2);
		*i += 2;
	}
	else
	{
		data->current_word[0] = input[*i];
		data->current_word[1] = '\0';
		save_word(data->words, data->word_count, data->current_word, 1);
		(*i)++;
	}
}

void	handle_pipe_token(t_token_data *data, int *i)
{
	if (*(data->word_pos) > 0)
	{
		save_word(data->words, data->word_count, data->current_word,
			*(data->word_pos));
		*(data->word_pos) = 0;
	}
	strcpy(data->current_word, "|");
	save_word(data->words, data->word_count, data->current_word, 1);
	(*i)++;
}

void	handle_logical_token(t_token_data *data, char *input, int *i)
{
	if (*(data->word_pos) > 0)
	{
		save_word(data->words, data->word_count, data->current_word,
			*(data->word_pos));
		*(data->word_pos) = 0;
	}
	if (input[*i] == '|' && input[*i + 1] == '|')
	{
		ft_strlcpy(data->current_word, "||", 3);
		save_word(data->words, data->word_count, data->current_word, 2);
		*i += 2;
	}
	else if (input[*i] == '&' && input[*i + 1] == '&')
	{
		ft_strlcpy(data->current_word, "&&", 3);
		save_word(data->words, data->word_count, data->current_word, 2);
		*i += 2;
	}
}

void	handle_separator_token(t_token_data *data, int *i)
{
	if (*(data->word_pos) > 0)
		save_word(data->words, data->word_count, data->current_word,
			*(data->word_pos));
	*(data->word_pos) = 0;
	memset(data->current_word, 0, 1000);
	(*i)++;
}
