/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:30:54 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_redirection_token(t_token_data *data, char *input, int *i)
{
	if (*(data->word_pos) > 0)
	{
		save_word(data->words, data->word_count, data->current_word,
			*(data->word_pos));
		*(data->word_pos) = 0;
	}
	data->current_word[0] = input[*i];
	if (input[*i + 1] == '>' || input[*i + 1] == '<')
	{
		data->current_word[1] = input[*i + 1];
		data->current_word[2] = '\0';
		(*i) += 2;
	}
	else
	{
		data->current_word[1] = '\0';
		(*i)++;
	}
	save_word(data->words, data->word_count, data->current_word,
		strlen(data->current_word));
}

static void	handle_pipe_token(t_token_data *data, int *i)
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

static void	handle_separator_token(t_token_data *data, int *i)
{
	if (*(data->word_pos) > 0)
		save_word(data->words, data->word_count, data->current_word,
			*(data->word_pos));
	*(data->word_pos) = 0;
	memset(data->current_word, 0, 1000);
	(*i)++;
}

void	process_input_chars(char *input, t_token_data *data, char *quote_state)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && *quote_state != '\'')
		{
			i = add_variable_to_word(input, i, data->current_word,
					data->word_pos);
			i++;
			continue ;
		}
		if (is_quote(input[i]))
		{
			*quote_state = update_quote_state(input[i], *quote_state);
			i++;
			continue ;
		}
		if ((input[i] == '>' || input[i] == '<') && !*quote_state)
		{
			handle_redirection_token(data, input, &i);
			continue ;
		}
		if (input[i] == '|' && !*quote_state)
		{
			handle_pipe_token(data, &i);
			continue ;
		}
		if (is_separator(input[i]) && !*quote_state)
		{
			handle_separator_token(data, &i);
			continue ;
		}
		add_char_to_word(data->current_word, data->word_pos, input[i]);
		i++;
	}
}

int	check_syntax_errors(char **words)
{
	int i;

	i = 0;
	while (words[i])
	{
		if ((strcmp(words[i], ">") == 0 || strcmp(words[i], "<") == 0
				|| strcmp(words[i], ">>") == 0 || strcmp(words[i], "<<") == 0)
			&& (!words[i + 1] || words[i + 1][0] == '>' || words[i
				+ 1][0] == '<' || words[i + 1][0] == '|'))
		{
			if (!words[i + 1])
				printf("minishell: syntax error near unexpected token "
						"`newline'\n");
			else
				printf("minishell: syntax error near unexpected token `%s'\n",
					words[i + 1]);
			return (1);
		}
		if ((words[i][0] == '>' || words[i][0] == '<') && strlen(words[i]) > 2)
		{
			printf("minishell: syntax error near unexpected token `%c'\n",
				words[i][2]);
			return (1);
		}
		i++;
	}
	return (0);
}