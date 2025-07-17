#include "pipex.h"

void	setup_pipe_in(t_pipex *pipex, int cmd_index)
{
	if (cmd_index > 0)
	{
		dup2(pipex->pipe_fds[2 * (cmd_index - 1)], STDIN_FILENO);
	}
}

void	setup_pipe_out(t_pipex *pipex, int cmd_index)
{
	if (cmd_index < pipex->cmd_count - 1)
	{
		dup2(pipex->pipe_fds[2 * cmd_index + 1], STDOUT_FILENO);
	}
}
