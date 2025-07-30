/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:00:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	init_cmd_struct(t_cmd *cmd)
{
	int	i;

	cmd->args = malloc(sizeof(char *) * 64);
	if (!cmd->args)
	{
		cmd->redirs = NULL;
		return (-1);
	}
	i = 0;
	while (i < 64)
	{
		cmd->args[i] = NULL;
		i++;
	}
	cmd->redirs = malloc(sizeof(t_redir) * 10);
	if (!cmd->redirs)
	{
		free(cmd->args);
		cmd->args = NULL;
		return (-1);
	}
	cmd->redir_count = 0;
	cmd->stdin_backup = -1;
	cmd->stdout_backup = -1;
	cmd->logical_operator = 0;
	return (0);
}

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	if (init_cmd_struct(cmd) == -1)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

static void	init_tokenize_data(t_token_data *data, t_tokenize_init *init)
{
	data->words = init->words;
	data->word_count = init->word_count;
	data->current_word = init->current_word;
	data->word_pos = init->word_pos;
	*(init->word_count) = 0;
	*(init->word_pos) = 0;
	memset(init->current_word, 0, 1000);
}

char	**tokenize_words(char *input, t_shell *shell)
{
	char			current_word[1000];
	int				counts[2];
	char			quote_state;
	t_token_data	data;
	t_tokenize_init	init;

	init.words = malloc(sizeof(char *) * 1000);
	if (!init.words)
		return (NULL);
	quote_state = 0;
	init.current_word = current_word;
	init.word_count = &counts[0];
	init.word_pos = &counts[1];
	init_tokenize_data(&data, &init);
	process_input_chars(input, &data, &quote_state, shell);
	if (counts[1] > 0)
		save_word(init.words, &counts[0], current_word, counts[1]);
	init.words[counts[0]] = NULL;
	return (init.words);
}
