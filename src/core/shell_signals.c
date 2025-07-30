/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	signal_handler(int signo)
{
	g_signal = signo;
	if (signo == SIGINT)
	{
		if (g_signal_exit_status == 999)
			return ;
		g_signal_exit_status = 130;
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		/* Only redisplay if we're not in a child process */
		if (g_signal_exit_status != 999)
			rl_redisplay();
	}
	else if (signo == SIGQUIT)
	{
		if (g_signal_exit_status == 999)
			return ;
		g_signal_exit_status = 131;
	}
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}


