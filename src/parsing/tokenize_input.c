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

static void	handle_quote_processing(char input_char, char *quote_state, int *i)
{
	*quote_state = update_quote_state(input_char, *quote_state);
	(*i)++;
}

static void	handle_special_chars(char *input, int *i, t_token_data *data,
		char quote_state)
{
	if ((input[*i] == '>' || input[*i] == '<') && !quote_state)
		handle_redirection_token(data, input, i);
	else if (input[*i] == '|' && !quote_state)
		handle_pipe_token(data, i);
	else if (is_separator(input[*i]) && !quote_state)
		handle_separator_token(data, i);
	else
	{
		add_char_to_word(data->current_word, data->word_pos, input[*i]);
		(*i)++;
	}
}

void	process_input_chars(char *input, t_token_data *data, char *quote_state,
		t_shell *shell)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && *quote_state != '\'')
		{
			handle_variable_expansion(input, &i, data, shell);
			continue ;
		}
		if (is_quote(input[i]))
		{
			handle_quote_processing(input[i], quote_state, &i);
			continue ;
		}
		handle_special_chars(input, &i, data, *quote_state);
	}
}
