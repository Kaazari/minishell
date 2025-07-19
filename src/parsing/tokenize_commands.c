/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	process_pipe_command(t_cmd_process *proc, int i)
{
	char	**cmd_words;

	cmd_words = create_cmd_words(proc->words, *(proc->start), i);
	if (!cmd_words)
		return ;
	proc->commands[*(proc->cmd_idx)] = create_cmd();
	if (!proc->commands[*(proc->cmd_idx)])
	{
		free_args(cmd_words);
		return ;
	}
	process_command_words(proc->commands[*(proc->cmd_idx)], cmd_words);
	free_args(cmd_words);
	*(proc->start) = i + 1;
	(*(proc->cmd_idx))++;
}

static void	process_pipe_tokens(char **words, t_cmd **commands, int *cmd_count)
{
	int				i;
	int				start;
	int				cmd_idx;
	t_cmd_process	proc;

	i = 0;
	start = 0;
	cmd_idx = 0;
	proc.commands = commands;
	proc.words = words;
	proc.cmd_idx = &cmd_idx;
	proc.start = &start;
	proc.cmd_count = cmd_count;
	while (words[i])
	{
		if (words[i][0] == '|' && words[i][1] == '\0')
		{
			process_pipe_command(&proc, i);
			(*cmd_count)++;
		}
		i++;
	}
	process_final_command(&proc, start, i);
	commands[cmd_idx] = NULL;
}

t_cmd	**tokenize_piped_commands(char *input, int *cmd_count, t_shell *shell)
{
	char	**words;
	t_cmd	**commands;

	words = tokenize_words(input, shell);
	if (!words)
		return (NULL);
	if (check_syntax_errors(words))
	{
		free_args(words);
		return (NULL);
	}
	commands = init_commands_array(words);
	if (!commands)
		return (NULL);
	*cmd_count = 1;
	process_pipe_tokens(words, commands, cmd_count);
	free_args(words);
	return (commands);
}
