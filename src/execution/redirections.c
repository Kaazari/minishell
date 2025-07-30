/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	has_heredoc(t_cmd *cmd)
{
	int	i;

	if (!cmd || !cmd->redirs)
		return (0);
	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_HEREDOC)
			return (1);
		i++;
	}
	return (0);
}

int	preprocess_all_heredocs(t_shell *shell, t_cmd **commands, int cmd_count)
{
	int	i;
	int	j;

	i = 0;
	while (i < cmd_count && commands[i])
	{
		if (has_heredoc(commands[i]))
		{
			j = 0;
			while (j < commands[i]->redir_count)
			{
				if (commands[i]->redirs[j].type == REDIR_HEREDOC)
				{
					handle_heredoc(commands[i], j, shell);
					if (shell->state == 1) /* Signal received */
						return (-1);
				}
				j++;
			}
		}
		i++;
	}
	return (0);
}

void	handle_redirections(t_cmd *cmd, t_shell *shell)
{
	int	i;

	(void)shell;
	if (!cmd || !cmd->redirs)
		return ;
	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_OUT
			|| cmd->redirs[i].type == REDIR_APPEND)
		{
			handle_output_redirection(cmd, i);
		}
		else if (cmd->redirs[i].type == REDIR_IN)
		{
			handle_input_redirection(cmd, i);
		}
		else if (cmd->redirs[i].type == REDIR_HEREDOC)
		{
			if (cmd->redirs[i].is_heredoc_fd > 0)
			{
				dup2(cmd->redirs[i].is_heredoc_fd, STDIN_FILENO);
				close(cmd->redirs[i].is_heredoc_fd);
				cmd->redirs[i].is_heredoc_fd = -1;
			}
		}
		i++;
	}
}

void	close_parent_heredoc_fds(t_cmd **commands, int cmd_count)
{
	int	i;
	int	j;

	i = 0;
	while (i < cmd_count && commands[i])
	{
		j = 0;
		while (j < commands[i]->redir_count)
		{
			if (commands[i]->redirs[j].type == REDIR_HEREDOC
				&& commands[i]->redirs[j].is_heredoc_fd > 0)
			{
				close(commands[i]->redirs[j].is_heredoc_fd);
				commands[i]->redirs[j].is_heredoc_fd = -1;
			}
			j++;
		}
		i++;
	}
}
