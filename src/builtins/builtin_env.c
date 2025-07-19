/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_env(char **args, t_shell *shell)
{
	int	i;

	(void)args;
	i = 0;
	while (shell->envp[i])
	{
		printf("%s\n", shell->envp[i]);
		i++;
	}
	return (0);
}

static void	handle_export_var(char *arg, t_shell *shell)
{
	if (strchr(arg, '='))
		add_or_update_env(arg, shell);
	else
		export_existing_var(arg, shell);
}

int	builtin_export(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
	{
		print_export_format(shell->envp);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		handle_export_var(args[i], shell);
		i++;
	}
	return (0);
}
