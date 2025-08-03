/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_exit_status(char *word, int *pos, t_shell *shell)
{
	char	*exit_str;

	exit_str = ft_itoa(shell->exit_status);
	if (!exit_str)
		return (0);
	ft_strlcpy(word + *pos, exit_str, ft_strlen(exit_str) + 1);
	*pos += ft_strlen(exit_str);
	free(exit_str);
	return (0);
}

static int	extract_variable_name(t_var_expansion *var_exp, char **var_name)
{
	int	start;
	int	end;

	start = var_exp->i + 1;
	end = start;
	while (var_exp->input[end] && (ft_isalnum(var_exp->input[end])
			|| var_exp->input[end] == '_'))
		end++;
	if (end == start)
		return (-1);
	*var_name = malloc(end - start + 1);
	if (!*var_name)
		return (-1);
	ft_strlcpy(*var_name, var_exp->input + start, end - start + 1);
	(*var_name)[end - start] = '\0';
	return (end - 1);
}

static void	insert_variable_value(t_var_expansion *var_exp, char *var_value)
{
	if (var_value)
	{
		if (*(var_exp->pos) + ft_strlen(var_value) > 1000)
		{
			write(2, "minishell: expansion would exceed limit\n", 38);
			write(2, "\n", 1);
			return ;
		}
		ft_strlcpy(var_exp->word + *(var_exp->pos),
			var_value, ft_strlen(var_value) + 1);
		*(var_exp->pos) += ft_strlen(var_value);
	}
}

int	add_variable_to_word(t_var_expansion *var_exp, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	int		result;

	if (var_exp->input[var_exp->i + 1] == '?')
	{
		handle_exit_status(var_exp->word, var_exp->pos, shell);
		return (var_exp->i + 1);
	}
	result = extract_variable_name(var_exp, &var_name);
	if (result == -1)
		return (var_exp->i);
	var_value = get_env_value(var_name, shell->envp);
	if (var_value && ft_strlen(var_value) > 1000)
	{
		write(2, "minishell: variable value too large\n", 35);
		write(2, "\n", 1);
		free(var_name);
		return (var_exp->i);
	}
	insert_variable_value(var_exp, var_value);
	free(var_name);
	return (result);
}
