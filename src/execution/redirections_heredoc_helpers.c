/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc_helpers.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Lit une ligne en mode interactif
 *
 * @return Ligne lue ou NULL en cas d'erreur
 */
char	*read_interactive_line(void)
{
	char	*line;

	line = readline("> ");
	if (!line || g_signal_exit_status == SIGINT)
	{
		if (line)
			free(line);
		return (NULL);
	}
	return (line);
}

/**
 * Lit une ligne en mode non-interactif
 *
 * @return Ligne lue ou NULL en cas d'erreur
 */
char	*read_non_interactive_line(void)
{
	char	*line;

	line = read_line_from_stdin();
	if (!line || g_signal_exit_status == SIGINT)
	{
		if (line)
			free(line);
		return (NULL);
	}
	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	return (line);
}

/**
 * Gère l'erreur lors de la création d'un heredoc
 *
 * @param pipe_fd: Descripteurs de pipe
 * @param cmd: Commande
 * @param i: Index de la redirection
 * @param shell: Structure shell
 */
void	handle_heredoc_error(int *pipe_fd, t_cmd *cmd, int i, t_shell *shell)
{
	close(pipe_fd[0]);
	cmd->redirs[i].is_heredoc_fd = -1;
	restore_main_signals();
	shell->state = 0;
}

/**
 * Gère le succès lors de la création d'un heredoc
 *
 * @param pipe_fd: Descripteurs de pipe
 * @param cmd: Commande
 * @param i: Index de la redirection
 */
void	handle_heredoc_success(int *pipe_fd, t_cmd *cmd, int i)
{
	cmd->redirs[i].is_heredoc_fd = pipe_fd[0];
	restore_main_signals();
}
