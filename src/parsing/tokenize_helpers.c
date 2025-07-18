/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:00:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '|' || c == '>'
		|| c == '<');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

char	update_quote_state(char c, char current_state)
{
	if (!is_quote(c))
		return (current_state);
	if (current_state == c)
		return (0);
	if (!current_state)
		return (c);
	return (current_state);
}

void	add_char_to_word(char *word, int *pos, char c)
{
	word[*pos] = c;
	(*pos)++;
}

void	save_word(char **words, int *count, char *word, int len)
{
	char	*word_copy;

	if (len == 0)
		return ;
	word_copy = malloc(len + 1);
	if (!word_copy)
		return ;
	strncpy(word_copy, word, len);
	word_copy[len] = '\0';
	words[*count] = word_copy;
	(*count)++;
}
