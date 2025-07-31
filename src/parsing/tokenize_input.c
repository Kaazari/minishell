/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_variable_expansion(char *input, int *i, t_token_data *data,
		t_shell *shell)
{
	t_var_expansion	var_exp;

	var_exp.input = input;
	var_exp.i = *i;
	var_exp.word = data->current_word;
	var_exp.pos = data->word_pos;
	*i = add_variable_to_word(&var_exp, shell);
	(*i)++;
}

static void	handle_special_chars(char *input, int *i, t_token_data *data,
		char quote_state)
{
	if ((input[*i] == '>' || input[*i] == '<') && !quote_state)
		handle_redirection_token(data, input, i);
	else if (input[*i] == '|' && input[*i + 1] == '|' && !quote_state)
		handle_logical_token(data, input, i);
	else if (input[*i] == '&' && input[*i + 1] == '&' && !quote_state)
		handle_logical_token(data, input, i);
	else if (input[*i] == '|' && !quote_state)
		handle_pipe_token(data, i);
	else if (is_separator(input[*i]) && !quote_state)
		handle_separator_token(data, i);
	else if (!is_quote(input[*i]))
	{
		add_char_to_word(data->current_word, data->word_pos, input[*i]);
		(*i)++;
	}
	else
		(*i)++;
}

void	process_input_chars(char *input, t_token_data *data, char *quote_state,
		t_shell *shell)
{
	int	i;
	int	quote_start;

	i = 0;
	quote_start = -1;
	while (input[i])
	{
		if (input[i] == '$' && *quote_state != '\'')
		{
			handle_variable_expansion(input, &i, data, shell);
			continue ;
		}
		if (is_quote(input[i]))
		{
			if (*quote_state == 0)
			{
				*quote_state = input[i];
				quote_start = i;
				i++;
			}
			else if (*quote_state == input[i])
			{
				/* Check if we have empty quotes */
				if (quote_start == i - 1)
				{
					/* Empty quotes, ignore them completely */
					*quote_state = 0;
					quote_start = -1;
					i++;
					continue ;
				}
				*quote_state = 0;
				quote_start = -1;
				i++;
			}
			else
			{
				/* Add the character to the word if it's inside quotes */
				add_char_to_word(data->current_word, data->word_pos, input[i]);
				i++;
			}
			continue ;
		}
		/* Handle backslash escaping inside quotes */
		if (input[i] == '\\' && *quote_state != 0)
		{
			i++;
			if (input[i])
			{
				/* Add the escaped character */
				add_char_to_word(data->current_word, data->word_pos, input[i]);
				i++;
			}
			continue ;
		}
		handle_special_chars(input, &i, data, *quote_state);
	}
	/* If we end with an open quote, display error message */
	if (*quote_state != 0)
	{
		if (*quote_state == '\'')
			write(STDERR_FILENO, "minishell: Error: Unmatched single quotes\n", 42);
		else
			write(STDERR_FILENO, "minishell: Error: Unmatched double quotes\n", 42);
		*(data->word_pos) = 0;
		/* Don't reset quote_state, let tokenize_words handle it */
	}
}
