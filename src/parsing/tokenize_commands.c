/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:30:58 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	add_redirection(t_cmd *cmd, int type, char *file)
{
	char	*file_copy;

	file_copy = ft_strdup(file);
	if (!file_copy)
		return ;
	(cmd->redirs + cmd->redir_count)->type = type;
	(cmd->redirs + cmd->redir_count)->file = file_copy;
	(cmd->redirs + cmd->redir_count)->fd = -1;
	cmd->redir_count++;
}

static int	get_redirection_type(char *token)
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

static void	process_command_words(t_cmd *cmd, char **words)
{
	int		i;
	int		arg_count;
	int		type;
	char	*arg_copy;

	i = 0;
	arg_count = 0;
	while (words[i])
	{
		if ((words[i][0] == '>' || words[i][0] == '<') && words[i + 1]
			&& words[i + 1][0] != '>' && words[i + 1][0] != '<' && words[i
			+ 1][0] != '|')
		{
			type = get_redirection_type(words[i]);
			add_redirection(cmd, type, words[i + 1]);
			i += 2;
		}
		else if (words[i][0] != '>' && words[i][0] != '<')
		{
			arg_copy = ft_strdup(words[i]);
			if (arg_copy)
			{
				cmd->args[arg_count] = arg_copy;
				arg_count++;
			}
			i++;
		}
		else
			i++;
	}
	cmd->args[arg_count] = NULL;
}

static char	**create_cmd_words(char **words, int start, int end)
{
	char	**cmd_words;
	int		j;

	cmd_words = malloc(sizeof(char *) * (end - start + 1));
	j = 0;
	while (start < end)
	{
		cmd_words[j] = strdup(words[start]);
		j++;
		start++;
	}
	cmd_words[j] = NULL;
	return (cmd_words);
}

static void	process_pipe_command(t_cmd **commands, char **words, int *cmd_idx,
		int *start, int i)
{
	char	**cmd_words;

	cmd_words = create_cmd_words(words, *start, i);
	commands[*cmd_idx] = create_cmd();
	process_command_words(commands[*cmd_idx], cmd_words);
	free_args(cmd_words);
	*start = i + 1;
	(*cmd_idx)++;
}

static void	process_final_command(t_cmd **commands, char **words, int *cmd_idx,
		int start, int i)
{
	char	**cmd_words;

	if (start < i)
	{
		cmd_words = create_cmd_words(words, start, i);
		commands[*cmd_idx] = create_cmd();
		process_command_words(commands[*cmd_idx], cmd_words);
		free_args(cmd_words);
		(*cmd_idx)++;
	}
}

t_cmd	**tokenize_piped_commands(char *input, int *cmd_count)
{
	char **words;
	t_cmd **commands;
	int i;
	int start;
	int cmd_idx;

	words = tokenize_words(input);
	if (!words)
		return (NULL);
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
		if (words[i][0] == '|' && words[i][1] == '\0')
		{
			process_pipe_command(commands, words, &cmd_idx, &start, i);
			(*cmd_count)++;
		}
		i++;
	}
	process_final_command(commands, words, &cmd_idx, start, i);
	commands[cmd_idx] = NULL;
	free_args(words);
	return (commands);
}