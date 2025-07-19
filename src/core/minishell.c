/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void	init_shell(t_shell *shell, char **envp)
{
	shell->envp = envp;
	shell->local_envp = NULL;
	shell->state = 0;
	shell->exit_status = 0;
	shell->pipex = malloc(sizeof(t_pipex));
	if (!shell->pipex)
	{
		printf("Error: Failed to allocate memory for pipex\n");
		exit(1);
	}
	shell->pipex->pipe_fds = NULL;
	shell->cmd = create_cmd();
	if (!shell->cmd)
	{
		free(shell->pipex);
		printf("Error: Failed to allocate memory for cmd\n");
		exit(1);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	setup_signals();
	init_shell(&shell, envp);
	main_shell_loop(&shell);
	return (0);
}
