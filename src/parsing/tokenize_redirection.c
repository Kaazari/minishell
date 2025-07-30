/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:00:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_redirection(t_cmd *cmd, int type, char *file)
{
	char	*file_copy;

	file_copy = ft_strdup(file);
	if (!file_copy)
		return ;
	(cmd->redirs + cmd->redir_count)->type = type;
	(cmd->redirs + cmd->redir_count)->file = file_copy;
	(cmd->redirs + cmd->redir_count)->fd = -1;
	(cmd->redirs + cmd->redir_count)->is_heredoc_fd = -1;
	cmd->redir_count++;
}

int	get_redirection_type(char *token)
{
	if (token[0] == '>')
	{
		if (token[1] == '>')
			return (REDIR_APPEND);
		else
			return (REDIR_OUT);
	}
	else if (token[0] == '<')
	{
		if (token[1] == '<')
			return (REDIR_HEREDOC);
		else
			return (REDIR_IN);
	}
	return (0);
}

static int	handle_redirection_word(t_cmd *cmd, char **words, int i)
{
	int	type;

	if ((words[i][0] == '>' || words[i][0] == '<') && words[i + 1]
		&& words[i + 1][0] != '>' && words[i + 1][0] != '<' && words[i
		+ 1][0] != '|')
	{
		type = get_redirection_type(words[i]);
		add_redirection(cmd, type, words[i + 1]);
		return (2);
	}
	return (0);
}

static int	handle_regular_word(t_cmd *cmd, char *word, int *arg_count)
{
	char	*arg_copy;

	if (word[0] != '>' && word[0] != '<')
	{
		arg_copy = ft_strdup(word);
		if (arg_copy)
		{
			cmd->args[*arg_count] = arg_copy;
			(*arg_count)++;
		}
		return (1);
	}
	return (0);
}

void	process_command_words(t_cmd *cmd, char **words)
{
	int		i;
	int		arg_count;
	int		increment;

	i = 0;
	arg_count = 0;
	while (words[i])
	{
		increment = handle_redirection_word(cmd, words, i);
		if (increment == 0)
			increment = handle_regular_word(cmd, words[i], &arg_count);
		if (increment == 0)
			increment = 1;
		i += increment;
	}
	cmd->args[arg_count] = NULL;
}
