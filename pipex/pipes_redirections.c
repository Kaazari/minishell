/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "pipex.h"

/**
 * Configure la redirection heredoc pour un processus enfant
 *
 * @param commands: Tableau de commandes
 * @param i: Index de la commande actuelle
 * @param j: Index de la redirection
 */
void	setup_heredoc_redirection(t_cmd **commands, int i, int j)
{
	if (commands[i]->redirs[j].type == REDIR_HEREDOC
		&& commands[i]->redirs[j].is_heredoc_fd > 0)
	{
		dup2(commands[i]->redirs[j].is_heredoc_fd, STDIN_FILENO);
		close(commands[i]->redirs[j].is_heredoc_fd);
	}
}

/**
 * Configure la redirection d'entrée pour un processus enfant
 *
 * @param commands: Tableau de commandes
 * @param i: Index de la commande actuelle
 * @param j: Index de la redirection
 */
void	setup_input_redirection(t_cmd **commands, int i, int j)
{
	int	fd;

	if (commands[i]->redirs[j].type == REDIR_IN)
	{
		fd = open(commands[i]->redirs[j].file, O_RDONLY);
		if (fd >= 0)
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else
		{
			perror("open");
			exit(1);
		}
	}
}

/**
 * Gère la redirection du descripteur de fichier de sortie
 *
 * @param fd: Descripteur de fichier
 */
void	handle_output_fd(int fd)
{
	if (fd >= 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else
	{
		perror("open");
		exit(1);
	}
}

/**
 * Configure la redirection de sortie pour un processus enfant
 *
 * @param commands: Tableau de commandes
 * @param i: Index de la commande actuelle
 * @param j: Index de la redirection
 */
void	setup_output_redirection(t_cmd **commands, int i, int j)
{
	int	fd;

	if (commands[i]->redirs[j].type == REDIR_OUT)
	{
		fd = open(commands[i]->redirs[j].file,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		handle_output_fd(fd);
	}
	else if (commands[i]->redirs[j].type == REDIR_APPEND)
	{
		fd = open(commands[i]->redirs[j].file,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		handle_output_fd(fd);
	}
}

/**
 * Configure toutes les redirections pour un processus enfant
 *
 * @param commands: Tableau de commandes
 * @param i: Index de la commande actuelle
 */
void	setup_child_redirections(t_cmd **commands, int i)
{
	int	j;

	j = 0;
	while (j < commands[i]->redir_count)
	{
		setup_heredoc_redirection(commands, i, j);
		setup_input_redirection(commands, i, j);
		setup_output_redirection(commands, i, j);
		j++;
	}
}
