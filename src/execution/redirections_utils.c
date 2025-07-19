/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:31:12 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	restore_output_redirection(t_cmd *cmd, int i)
{
	dup2(cmd->redirs[i].fd, STDOUT_FILENO);
	close(cmd->redirs[i].fd);
}

void	restore_input_redirection(t_cmd *cmd, int i)
{
	dup2(cmd->redirs[i].fd, STDIN_FILENO);
	close(cmd->redirs[i].fd);
}

void	restore_redirections(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_OUT
			|| cmd->redirs[i].type == REDIR_APPEND)
		{
			restore_output_redirection(cmd, i);
		}
		else if (cmd->redirs[i].type == REDIR_IN)
		{
			restore_input_redirection(cmd, i);
		}
		free(cmd->redirs[i].file);
		i++;
	}
	cmd->redir_count = 0;
}

void	handle_output_redirection(t_cmd *cmd, int i)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cmd->redirs[i].type == REDIR_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(cmd->redirs[i].file, flags, 0644);
	if (fd != -1)
	{
		cmd->redirs[i].fd = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else
		perror("open");
}

void	handle_input_redirection(t_cmd *cmd, int i)
{
	int	fd;

	fd = open(cmd->redirs[i].file, O_RDONLY);
	if (fd != -1)
	{
		cmd->redirs[i].fd = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}
