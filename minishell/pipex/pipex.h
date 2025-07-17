#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>

typedef struct s_pipex
{
	int		pipe_fd[2];
	pid_t	pid;
	int		cmd_count;
	int		pipe_count;
	int		*pipe_fds;
}	t_pipex;

// Pipes functions
void	init_pipes(t_pipex *pipex, int cmd_count);
void	close_pipes(t_pipex *pipex);
void	wait_children(t_pipex *pipex);

#endif
