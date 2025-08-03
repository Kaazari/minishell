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

static int	is_n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	if (!arg[1])
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(char **args, t_shell *shell)
{
	int	i;
	int	newline;

	(void)shell;
	i = 1;
	newline = 1;
	while (args[i] && is_n_option(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		i++;
		if (args[i])
			write(STDOUT_FILENO, " ", 1);
	}
	if (newline == 1)
		write(STDOUT_FILENO, "\n", 1);
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

static int	handle_exit_value(char *arg)
{
	long long	value;

	value = ft_atoi(arg);
	if (value > 255)
		return (value % 256);
	else if (value < 0)
		return (256 + (value % 256));
	else
		return ((int)value);
}

int	builtin_exit(char **args, t_shell *shell)
{
	int	exit_code;

	(void)shell;
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
			exit_code = handle_exit_value(args[1]);
	}
	return (exit_code + 1000);
}
