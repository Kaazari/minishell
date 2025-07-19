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

char	*get_env_value(char *var, char **envp)
{
	int		i;
	int		len;
	char	*value;

	i = 0;
	len = strlen(var);
	while (envp[i])
	{
		if (strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
		{
			value = envp[i] + len + 1;
			return (value);
		}
		i++;
	}
	return (NULL);
}

static int	handle_exit_status(char *word, int *pos, t_shell *shell)
{
	char	exit_str[12];

	sprintf(exit_str, "%d", shell->exit_status);
	strcpy(word + *pos, exit_str);
	*pos += strlen(exit_str);
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
	strncpy(*var_name, var_exp->input + start, end - start);
	(*var_name)[end - start] = '\0';
	return (end - 1);
}

static void	insert_variable_value(t_var_expansion *var_exp, char *var_value)
{
	if (var_value)
	{
		strcpy(var_exp->word + *(var_exp->pos), var_value);
		*(var_exp->pos) += strlen(var_value);
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
	insert_variable_value(var_exp, var_value);
	free(var_name);
	return (result);
}
