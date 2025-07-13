#include "minishell.h"

int is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' ||
			c == '|' || c == '>' || c == '<');
}

int is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

char update_quote_state(char c, char current_state)
{
	if (!is_quote(c))
		return current_state;
	if (current_state == c)
		return 0;
	if (!current_state)
		return c;
	return current_state;
}

void add_char_to_word(char *word, int *pos, char c)
{
	word[*pos] = c;
	(*pos)++;
}

void save_word(char **words, int *count, char *word, int len)
{
	if (len == 0)
		return;
	words[*count] = malloc(len + 1);
	strncpy(words[*count], word, len);
	words[*count][len] = '\0';
	(*count)++;
}

char *get_env_value(char *name) {
	char *value = getenv(name);
		if (value)
		return strdup(value);
	else
		return strdup("");
}

int add_variable_to_word(char *input, int i, char *word, int *pos)
{
	char var_name[256];
	int var_pos = 0;
	i++;

	while (input[i] && (isalnum(input[i]) || input[i] == '_'))
	{
		var_name[var_pos++] = input[i++];
	}
	var_name[var_pos] = '\0';

	char *value = getenv(var_name);
	if (value)
	{
		strcpy(word + *pos, value);
		*pos += strlen(value);
	}
	return i - 1;
}

static void	init_cmd_struct(t_cmd *cmd)
{
	cmd->args = malloc(sizeof(char *) * 64);
	cmd->redirs = malloc(sizeof(t_redir) * 10);
	cmd->redir_count = 0;
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
		free(cmd->args);
		free(cmd->redirs);
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

static void init_tokenizer(char *current_word, int *word_count, int *word_pos, char *quote_state)
{
	*word_count = 0;
	*word_pos = 0;
	*quote_state = 0;
	memset(current_word, 0, 1000);
}

static void handle_redirection_token(char **words, int *word_count, char *current_word,
	int *word_pos, char *input, int *i)
{
	if (*word_pos > 0)
	{
		save_word(words, word_count, current_word, *word_pos);
		*word_pos = 0;
	}
	current_word[0] = input[*i];
	if (input[*i + 1] == '>')
		current_word[1] = input[++(*i)];
	save_word(words, word_count, current_word, strlen(current_word));
	memset(current_word, 0, 1000);
	(*i)++;
}

static void handle_separator_token(char **words, int *word_count, char *current_word,
	int *word_pos, int *i)
{
	if (*word_pos > 0)
		save_word(words, word_count, current_word, *word_pos);
	*word_pos = 0;
	memset(current_word, 0, 1000);
	(*i)++;
}

static void add_redirection(t_cmd *cmd, int type, char *file)
{
	cmd->redirs[cmd->redir_count].type = type;
	cmd->redirs[cmd->redir_count].file = strdup(file);
	cmd->redirs[cmd->redir_count].fd = -1;
	cmd->redir_count++;
}

static void process_variable(char **words, int *word_count, char *current_word,
	int *word_pos, char *input, int *i, char quote_state)
{
	(void)words;
	(void)word_count;
	if (input[*i] == '$' && quote_state != '\'')
	{
		*i = add_variable_to_word(input, *i, current_word, word_pos);
		(*i)++;
	}
}

static void process_quotes(char *input, int *i, char *quote_state)
{
	*quote_state = update_quote_state(input[*i], *quote_state);
	if (is_quote(input[*i]))
		(*i)++;
}

static void process_regular_char(char *current_word, int *word_pos, char *input, int *i)
{
	add_char_to_word(current_word, word_pos, input[*i]);
	(*i)++;
}

static char **tokenize_words(char *input)
{
	char **words = malloc(sizeof(char *) * 64);
	char current_word[1000];
	int word_count, word_pos;
	char quote_state;
	int i;

	init_tokenizer(current_word, &word_count, &word_pos, &quote_state);
	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && quote_state != '\'')
		{
			process_variable(words, &word_count, current_word, &word_pos, input, &i, quote_state);
			continue;
		}
		process_quotes(input, &i, &quote_state);
		if (is_quote(input[i - 1]))
			continue;
		if ((input[i] == '>' || input[i] == '<') && !quote_state)
		{
			handle_redirection_token(words, &word_count, current_word, &word_pos, input, &i);
			continue;
		}
		if (is_separator(input[i]) && !quote_state)
		{
			handle_separator_token(words, &word_count, current_word, &word_pos, &i);
			continue;
		}
		process_regular_char(current_word, &word_pos, input, &i);
	}
	if (word_pos > 0)
		save_word(words, &word_count, current_word, word_pos);
	words[word_count] = NULL;
	return words;
}

static int get_redirection_type(char *token)
{
	int type;

	if (token[0] == '>')
	{
		if (token[1] == '>')
			type = REDIR_APPEND;
		else
			type = REDIR_OUT;
	}
	else
		type = REDIR_IN;
	return (type);
}

t_cmd *tokenize_input(char *input)
{
	t_cmd *cmd = create_cmd();
	if (!cmd)
		return NULL;

	char **words = tokenize_words(input);
	if (!words)
	{
		free_cmd(cmd);
		return NULL;
	}

	int i = 0;
	int arg_count = 0;
	while (words[i])
	{
		if ((words[i][0] == '>' || words[i][0] == '<') && words[i + 1])
		{
			int type = get_redirection_type(words[i]);
			add_redirection(cmd, type, words[i + 1]);
			i += 2;
		}
		else
		{
			cmd->args[arg_count++] = strdup(words[i]);
			i++;
		}
	}
	cmd->args[arg_count] = NULL;

	for (i = 0; words[i]; i++)
		free(words[i]);
	free(words);

	return cmd;
}
