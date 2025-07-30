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
	execute_shell_command(commands[0]->args, shell, 1);
	shell->cmd = NULL;
	free_partial_cmds(commands, 1);
}

void	handle_logical_commands(t_shell *shell, t_cmd **commands, int count)
{
	int	i;
	int	prev_exit_status;

	i = 0;
	while (i < count && commands[i])
	{
		shell->cmd = commands[i];
		execute_shell_command(commands[i]->args, shell, 1);
		prev_exit_status = shell->exit_status;

		/* Check if we should continue based on logical operator */
		if (i + 1 < count && commands[i + 1])
		{
			if (commands[i]->logical_operator == 1) /* || */
			{
				/* Only execute next command if current failed */
				if (prev_exit_status == 0)
					break ;
			}
			else if (commands[i]->logical_operator == 2) /* && */
			{
				/* Only execute next command if current succeeded */
				if (prev_exit_status != 0)
					break ;
			}
		}
		i++;
	}
	shell->cmd = NULL;
	free_partial_cmds(commands, count);
}

void	handle_piped_commands(t_shell *shell, t_cmd **commands, int count)
{
	/* Preprocess all heredocs before executing pipes */
	if (preprocess_all_heredocs(shell, commands, count) == -1)
	{
		free_partial_cmds(commands, count);
		return ;
	}

	/* Initialize pipex for pipe context */
	shell->pipex = malloc(sizeof(t_pipex));
	if (shell->pipex)
	{
		shell->pipex->cmd_count = count;
		shell->pipex->pipe_count = count - 1;
	}

	execute_piped_commands(shell, commands, count);

	/* Clean up pipex */
	if (shell->pipex)
	{
		free(shell->pipex);
		shell->pipex = NULL;
	}

	free_partial_cmds(commands, count);
}

static int	has_logical_operators(t_cmd **commands, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count && commands[i])
	{
		if (commands[i]->logical_operator != 0)
			return (1);
		i++;
	}
	return (0);
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
	else if (has_logical_operators(commands, cmd_count))
	{
		handle_logical_commands(shell, commands, cmd_count);
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

	/* Check if input is empty or only whitespace */
	if (!input || ft_strlen(input) == 0)
		return ;

	commands = tokenize_piped_commands(input, &cmd_count, shell);
	if (!commands)
		return ;

	/* Check if we have any valid commands */
	if (!commands[0] || !commands[0]->args || !commands[0]->args[0] || ft_strlen(commands[0]->args[0]) == 0)
	{
		printf("minishell: : command not found\n");
		shell->exit_status = 127;
		free_partial_cmds(commands, cmd_count);
		return ;
	}

	handle_commands_execution(shell, commands, cmd_count);
}
