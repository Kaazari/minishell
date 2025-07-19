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

static void	handle_signal_in_main(t_shell *shell)
{
	if (g_signal == SIGINT)
	{
		if (shell->state == 3)
		{
			shell->state = 1;
		}
		else
		{
			shell->state = 1;
		}
		g_signal = 0;
	}
}

static int	handle_input_processing(t_shell *shell, char *input)
{
	if (shell->state == 1)
	{
		free(input);
		return (1);
	}
	if (strlen(input) > 0)
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
		handle_signal_in_main(shell);
		shell->state = 0;
		input = readline("minishell> ");
		if (!input)
			clean_exit(shell, 0);
		handle_signal_in_main(shell);
		if (handle_input_processing(shell, input))
			continue ;
		process_command(shell, input);
		free(input);
		if (shell->state == 2)
			clean_exit(shell, 131);
	}
}
