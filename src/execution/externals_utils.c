/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externals_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_command_name(char *cmd)
{
	int	i;

	if (!cmd || !cmd[0])
		return (0);
	if (strcmp(cmd, "!") == 0)
		return (0);
	if (strcmp(cmd, "\\n") == 0 || strcmp(cmd, "\\t") == 0 || strcmp(cmd,
			"\\r") == 0)
		return (0);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\n' || cmd[i] == '\r' || cmd[i] == '\t')
			return (0);
		i++;
	}
	return (1);
}

void	handle_path_not_found(char *command, t_shell *shell)
{
	if (command[0] == '/' || command[0] == '.')
		printf("minishell: %s: No such file or directory\n", command);
	else
		printf("minishell: %s: command not found\n", command);
	shell->exit_status = 127;
}

char	*validate_and_get_path(char **args, t_shell *shell)
{
	char	*path;

	if (!args || !args[0])
		return (NULL);
	if (strcmp(args[0], "!") == 0 || strcmp(args[0], "\\n") == 0
		|| strcmp(args[0], "\\t") == 0 || strcmp(args[0], "\\r") == 0)
	{
		shell->exit_status = 0;
		return (NULL);
	}
	if (!is_valid_command_name(args[0]))
	{
		printf("minishell: %s: command not found\n", args[0]);
		shell->exit_status = 127;
		return (NULL);
	}
	path = find_command_in_path(args[0]);
	if (!path)
	{
		handle_path_not_found(args[0], shell);
		return (NULL);
	}
	return (path);
}

void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
