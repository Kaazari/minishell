/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:30:51 by kclaudan         ###   ########.fr       */
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

char	*get_env_value(char *name)
{
	char	*value;
	char	*result;

	value = getenv(name);
	if (value)
		result = ft_strdup(value);
	else
		result = ft_strdup("");
	return (result);
}

int	add_variable_to_word(char *input, int i, char *word, int *pos)
{
	char	var_name[256];
	char	exit_str[12];
	char	*value;
	int		var_pos;

	var_pos = 0;
	i++;
	if (input[i] == '?')
	{
		sprintf(exit_str, "%d", g_shell->exit_status);
		strcpy(word + *pos, exit_str);
		*pos += strlen(exit_str);
		return (i);
	}
	while (input[i] && (isalnum(input[i]) || input[i] == '_'))
	{
		var_name[var_pos++] = input[i++];
	}
	var_name[var_pos] = '\0';
	value = getenv(var_name);
	if (value)
	{
		strcpy(word + *pos, value);
		*pos += strlen(value);
	}
	return (i - 1);
}

static void	init_cmd_struct(t_cmd *cmd)
{
	cmd->args = malloc(sizeof(char *) * 64);
	cmd->redirs = malloc(sizeof(t_redir) * 10);
	cmd->redir_count = 0;
	cmd->stdin_backup = -1;
	cmd->stdout_backup = -1;
}

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	init_cmd_struct(cmd);
	if (!cmd->args || !cmd->redirs)
	{
		if (cmd->args)
			free(cmd->args);
		if (cmd->redirs)
			free(cmd->redirs);
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

static void	init_tokenizer(char *current_word, int *word_count, int *word_pos,
		char *quote_state)
{
	*word_count = 0;
	*word_pos = 0;
	*quote_state = 0;
	memset(current_word, 0, 1000);
}

char	**tokenize_words(char *input)
{
	char			**words;
	char			current_word[1000];
	int				word_count;
	int				word_pos;
	char			quote_state;
	t_token_data	data;

	words = malloc(sizeof(char *) * 64);
	if (!words)
		return (NULL);
	data.words = words;
	data.current_word = current_word;
	data.word_count = &word_count;
	data.word_pos = &word_pos;
	init_tokenizer(current_word, &word_count, &word_pos, &quote_state);
	process_input_chars(input, &data, &quote_state);
	if (word_pos > 0)
		save_word(words, &word_count, current_word, word_pos);
	words[word_count] = NULL;
	return (words);
}
