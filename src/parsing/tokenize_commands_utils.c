/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**create_cmd_words(char **words, int start, int end)
{
	char	**cmd_words;
	int		j;

	cmd_words = malloc(sizeof(char *) * (end - start + 1));
	if (!cmd_words)
		return (NULL);
	j = 0;
	while (start < end)
	{
		cmd_words[j] = ft_strdup(words[start]);
		if (!cmd_words[j])
		{
			while (j > 0)
				free(cmd_words[--j]);
			free(cmd_words);
			return (NULL);
		}
		j++;
		start++;
	}
	cmd_words[j] = NULL;
	return (cmd_words);
}

void	process_final_command(t_cmd_process *proc, int start, int i)
{
	char	**cmd_words;

	if (start < i)
	{
		cmd_words = create_cmd_words(proc->words, start, i);
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
		(*(proc->cmd_idx))++;
	}
}

t_cmd	**init_commands_array(char **words)
{
	t_cmd	**commands;

	commands = malloc(sizeof(t_cmd *) * 64);
	if (!commands)
	{
		free_args(words);
		return (NULL);
	}
	return (commands);
}
