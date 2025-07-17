#include "minishell.h"

extern t_shell	*g_shell;

int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '|' || c == '>' || c == '<');
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
	if (len == 0)
		return ;
	words[*count] = malloc(len + 1);
	strncpy(words[*count], word, len);
	words[*count][len] = '\0';
	(*count)++;
}

char	*get_env_value(char *name)
{
	char	*value;

	value = getenv(name);
	if (value)
		return (strdup(value));
	else
		return (strdup(""));
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
	init_cmd_struct(cmd);
	if (!cmd->args || !cmd->redirs)
	{
		free(cmd->args);
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

static void	add_redirection(t_cmd *cmd, int type, char *file)
{
	(cmd->redirs + cmd->redir_count)->type = type;
	(cmd->redirs + cmd->redir_count)->file = strdup(file);
	(cmd->redirs + cmd->redir_count)->fd = -1;
	cmd->redir_count++;
}

static void	process_variable(char *current_word, int *word_pos, char *input,
	int *i, char quote_state)
{
	if (input[*i] == '$' && quote_state != '\'')
	{
		*i = add_variable_to_word(input, *i, current_word, word_pos);
	}
}

static void	process_regular_char(char *current_word, int *word_pos, char *input,
	int *i)
{
	add_char_to_word(current_word, word_pos, input[*i]);
	(*i)++;
}

static int	check_syntax_errors(char **words)
{
	int	i;

	i = 0;
	while (words[i])
	{
		if ((strcmp(words[i], ">") == 0 || strcmp(words[i], "<") == 0
			|| strcmp(words[i], ">>") == 0 || strcmp(words[i], "<<") == 0)
			&& (!words[i + 1] || words[i + 1][0] == '>'
				|| words[i + 1][0] == '<' || words[i + 1][0] == '|'))
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

static void	process_input_chars(char *input, t_token_data *data, char *quote_state)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && *quote_state != '\'')
		{
			process_variable(data->current_word, data->word_pos, input, &i,
				*quote_state);
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
		process_regular_char(data->current_word, data->word_pos, input, &i);
	}
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

static int get_redirection_type(char *token)
{
	int type;

	if (*(token + 0) == '>')
	{
		if (*(token + 1) == '>')
			type = REDIR_APPEND;
		else
			type = REDIR_OUT;
	}
	else if (*(token + 0) == '<')
	{
		if (*(token + 1) == '<')
			type = REDIR_HEREDOC;
		else
			type = REDIR_IN;
	}
	return (type);
}

static void	process_command_words(t_cmd *cmd, char **words)
{
	int	i;
	int	arg_count;
	int	type;

	i = 0;
	arg_count = 0;
	while (words[i])
	{
		if ((words[i][0] == '>' || words[i][0] == '<') && words[i + 1]
			&& words[i + 1][0] != '>' && words[i + 1][0] != '<'
			&& words[i + 1][0] != '|')
		{
			type = get_redirection_type(words[i]);
			add_redirection(cmd, type, words[i + 1]);
			i += 2;
		}
		else if (words[i][0] != '>' && words[i][0] != '<')
		{
			cmd->args[arg_count] = strdup(words[i]);
			arg_count++;
			i++;
		}
		else
			i++;
	}
	cmd->args[arg_count] = NULL;
}

static int	is_pipe_token(char *token)
{
	return (token && *(token + 0) == '|' && *(token + 1) == '\0');
}

t_cmd	**tokenize_piped_commands(char *input, int *cmd_count)
{
	char	**words;
	t_cmd	**commands;
	int		i;
	int		start;
	int		cmd_idx;

	words = tokenize_words(input);
	if (!words)
		return (NULL);

	// Check for syntax errors
	if (check_syntax_errors(words))
	{
		free_args(words);
		return (NULL);
	}

	commands = malloc(sizeof(t_cmd *) * 64);
	if (!commands)
	{
		free_args(words);
		return (NULL);
	}
	i = 0;
	start = 0;
	cmd_idx = 0;
	*cmd_count = 1;
	while (words[i])
	{
		if (is_pipe_token(words[i]))
		{
			char	**cmd_words;
			int		j;

			cmd_words = malloc(sizeof(char *) * (i - start + 1));
			j = 0;
			while (start < i)
			{
				cmd_words[j] = strdup(words[start]);
				j++;
				start++;
			}
			cmd_words[j] = NULL;
			commands[cmd_idx] = create_cmd();
			process_command_words(commands[cmd_idx], cmd_words);
			free_args(cmd_words);
			start = i + 1;
			cmd_idx++;
			(*cmd_count)++;
		}
		i++;
	}
	if (start < i)
	{
		char	**cmd_words;
		int		j;

		cmd_words = malloc(sizeof(char *) * (i - start + 1));
		j = 0;
		while (start < i)
		{
			cmd_words[j] = strdup(words[start]);
			j++;
			start++;
		}
		cmd_words[j] = NULL;
		commands[cmd_idx] = create_cmd();
		process_command_words(commands[cmd_idx], cmd_words);
		free_args(cmd_words);
		cmd_idx++;
	}
	commands[cmd_idx] = NULL;
	free_args(words);
	return (commands);
}

t_cmd	*tokenize_input(char *input)
{
	t_cmd	*cmd;
	char	**words;
	int		i;
	int		arg_count;
	int		type;

	cmd = create_cmd();
	if (!cmd)
		return (NULL);
	words = tokenize_words(input);
	if (!words)
	{
		free_cmd(cmd);
		return (NULL);
	}
	i = 0;
	arg_count = 0;
	while (words[i])
	{
		if ((words[i][0] == '>' || words[i][0] == '<') && words[i + 1])
		{
			type = get_redirection_type(words[i]);
			add_redirection(cmd, type, words[i + 1]);
			i += 2;
		}
		else
		{
			cmd->args[arg_count] = strdup(words[i]);
			arg_count++;
			i++;
		}
	}
	cmd->args[arg_count] = NULL;

	i = 0;
	while (words[i])
	{
		free(words[i]);
		i++;
	}
	free(words);
	return (cmd);
}
