/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	nline;

	i = 1;
	nline = 1;
	if (args[1] && strcmp(args[1], "-n") == 0)
	{
		nline = 0;
		i = 2;
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (nline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	builtin_colon(char **args)
{
	(void)args;
	return (0);
}

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args)
{
	int	exit_code;

	exit_code = 0;
	printf("exit\n");
	if (args[1] != NULL)
	{
		if (!is_numeric(args[1]))
		{
			printf("minishell: exit: %s: numeric argument required\n", args[1]);
			exit_code = 255;
		}
		else
			exit_code = ft_atoi(args[1]);
	}
	return (exit_code + 1000); // Special code to indicate exit
}
