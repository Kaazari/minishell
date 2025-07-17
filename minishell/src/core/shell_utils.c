/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:30:47 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	process_empty_input(char *input)
{
	int	i;

	i = 0;
	if (strlen(input) == 0)
		return (1);
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n'
			&& input[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

static void	handle_single_command(t_shell *shell, t_cmd **commands)
{
	shell->cmd = commands[0];
	execute_shell_command(commands[0]->args, shell);
	free(commands);
}

static void	handle_piped_commands(t_shell *shell, t_cmd **commands, int count)
{
	int	i;

	execute_piped_commands(shell, commands, count);
	i = 0;
	while (i < count)
	{
		free_cmd(commands[i]);
		i++;
	}
	free(commands);
}

static void	process_command(t_shell *shell, char *input)
{
	int		cmd_count;
	t_cmd	**commands;

	commands = tokenize_piped_commands(input, &cmd_count);
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

void	main_shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		shell->state = 0;
		input = readline("minishell> ");
		if (!input)
			clean_exit(0);
		if (shell->state == 1)
		{
			free(input);
			continue ;
		}
		if (strlen(input) > 0)
			add_history(input);
		if (process_empty_input(input))
		{
			free(input);
			continue ;
		}
		process_command(shell, input);
		free(input);
		if (shell->state == 2)
			clean_exit(131);
	}
}
