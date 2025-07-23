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
	execute_shell_command(commands[0]->args, shell);
	free_partial_cmds(commands, 1);
}

void	handle_piped_commands(t_shell *shell, t_cmd **commands, int count)
{
	execute_piped_commands(shell, commands, count);
	free_partial_cmds(commands, count);
}

static void	handle_commands_execution(t_shell *shell, t_cmd **commands,
				int cmd_count)
{
	shell->current_commands = commands;
	shell->current_cmd_count = cmd_count;
	if (cmd_count == 1)
	{
		handle_single_command(shell, commands);
		shell->current_commands = NULL;
		shell->current_cmd_count = 0;
	}
	else
	{
		handle_piped_commands(shell, commands, cmd_count);
		shell->current_commands = NULL;
		shell->current_cmd_count = 0;
	}
}

void	process_command(t_shell *shell, char *input)
{
	int		cmd_count;
	t_cmd	**commands;

	commands = tokenize_piped_commands(input, &cmd_count, shell);
	if (!commands)
		return ;
	if (!commands[0])
	{
		free_partial_cmds(commands, cmd_count);
		return ;
	}
	handle_commands_execution(shell, commands, cmd_count);
}
