/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_redirection_syntax(char **words, int i)
{
	if (!words[i + 1])
		printf("minishell: syntax error near unexpected token `newline'\n");
	else
		printf("minishell: syntax error near unexpected token `%s'\n",
			words[i + 1]);
	return (1);
}

static int	check_redirection_length(char *word)
{
	if ((word[0] == '>' || word[0] == '<') && ft_strlen(word) > 2)
	{
		printf("minishell: syntax error near unexpected token `%c'\n",
			word[2]);
		return (1);
	}
	return (0);
}

int	check_syntax_errors(char **words)
{
	int	i;

	i = 0;
	while (words[i])
	{
		if ((ft_strncmp(words[i], ">", 2) == 0
				|| ft_strncmp(words[i], "<", 2) == 0
				|| ft_strncmp(words[i], ">>", 3) == 0
				|| ft_strncmp(words[i], "<<", 3) == 0)
			&& (!words[i + 1] || words[i + 1][0] == '>'
			|| words[i + 1][0] == '<' || words[i + 1][0] == '|'))
		{
			return (check_redirection_syntax(words, i));
		}
		if (check_redirection_length(words[i]))
			return (1);
		i++;
	}
	return (0);
}
