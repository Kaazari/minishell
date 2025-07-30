/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_input_processing(t_shell *shell, char *input)
{
	if (g_signal_exit_status == 130)
	{
		shell->exit_status = 130;
		g_signal_exit_status = 0;
		free(input);
		return (1);
	}
	if (ft_strlen(input) > 0)
		add_history(input);
	if (process_empty_input(input))
	{
		free(input);
		return (1);
	}
	return (0);
}

void	main_shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (g_signal_exit_status != 0 && g_signal_exit_status != 999)
		{
			shell->exit_status = g_signal_exit_status;
			g_signal_exit_status = 0;
		}
		else if (g_signal_exit_status == 999)
		{
			g_signal_exit_status = 0;
		}
		if (!input)
			clean_exit(shell, 0);
		if (handle_input_processing(shell, input))
			continue ;
		process_command(shell, input);
		free(input);
		if (shell->state == 2)
			clean_exit(shell, 131);
	}
}
