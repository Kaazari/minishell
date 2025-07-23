/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_single_command(t_shell *shell, t_cmd **commands)
{
	shell->cmd = commands[0];
	execute_shell_command(commands[0]->args, shell);
	free_cmds(commands);
	shell->cmd = NULL;
}

void	handle_piped_commands(t_shell *shell, t_cmd **commands, int count)
{
	execute_piped_commands(shell, commands, count);
	free_cmds(commands);
	shell->cmd = NULL;
}

void	process_command(t_shell *shell, char *input)
{
	int		cmd_count;
	t_cmd	**commands;

	commands = tokenize_piped_commands(input, &cmd_count, shell);
	if (!commands || !commands[0])
	{
		if (commands)
			free(commands);
		return ;
	}
	if (cmd_count == 1)
		handle_single_command(shell, commands);
	else
		handle_piped_commands(shell, commands, cmd_count);
}
