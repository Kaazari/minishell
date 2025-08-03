/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <termios.h>
# include "../pipex/pipex.h"
# include "../libft/libft.h"

# define REDIR_IN 1
# define REDIR_OUT 2
# define REDIR_APPEND 3
# define REDIR_HEREDOC 4

/* Variables globales */
extern int	g_signal_exit_status;

typedef struct s_redir
{
	int			type;
	char		*file;
	int			fd;
	int			is_heredoc_fd;
}				t_redir;

typedef struct s_cmd
{
	char		**args;
	t_redir		*redirs;
	int			redir_count;
	int			stdin_backup;
	int			stdout_backup;
	int			logical_operator; /* 0=none, 1=||, 2=&& */
}				t_cmd;

typedef struct s_shell
{
	char		**envp;
	char		**local_envp;
	t_cmd		*cmd;
	t_pipex		*pipex;
	int			state;
	int			exit_status;
	t_cmd		**current_commands;
	int			current_cmd_count;
	char		*pwd;
	char		*oldpwd;
}				t_shell;

typedef struct s_token_data
{
	char		*current_word;
	int			*word_pos;
	int			*word_count;
	char		**words;
}				t_token_data;

typedef struct s_quote_context
{
	char	*input;
	int		*i;
	t_token_data	*data;
	char	*quote_state;
	int		*quote_start;
}				t_quote_context;

typedef struct s_var_expansion_context
{
	char	*input;
	int		*i;
	t_token_data	*data;
	char	*quote_state;
	t_shell	*shell;
}				t_var_expansion_context;

typedef struct s_char_iteration_context
{
	char	*input;
	t_token_data	*data;
	char	*quote_state;
	t_shell	*shell;
	int		*i;
	int		*quote_start;
}				t_char_iteration_context;

typedef struct s_var_expansion
{
	char	*input;
	int		i;
	char	*word;
	int		*pos;
	int		depth;
	char	**expanding_vars;
	int		expanding_count;
	int		limit_reached;
}				t_var_expansion;

typedef struct s_cmd_process
{
	t_cmd	**commands;
	char	**words;
	int		*cmd_idx;
	int		*start;
	int		*cmd_count;
}				t_cmd_process;

typedef struct s_tokenize_init
{
	char	**words;
	char	*current_word;
	int		*word_count;
	int		*word_pos;
}				t_tokenize_init;

/**
 * Structure pour les paramètres du processus enfant
 */
typedef struct s_child_params
{
	t_cmd	**commands;
	int		i;
	int		prev_fd;
	int		*pipe_fd;
	int		cmd_count;
	t_shell	*shell;
}	t_child_params;

typedef struct s_child_exec_params
{
	t_cmd	**commands;
	int		i;
	int		prev_fd;
	int		*pipe_fd;
	int		cmd_count;
	t_shell	*shell;
}	t_child_exec_params;

typedef struct s_pipe_iteration_params
{
	t_shell	*shell;
	t_cmd	**commands;
	int		cmd_count;
	int		*pipe_fd;
	int		*prev_fd;
	pid_t	*pids;
	int		*i;
}	t_pipe_iteration_params;

typedef struct s_pipe_loop_params
{
	t_shell	*shell;
	t_cmd	**commands;
	int		cmd_count;
	int		*prev_fd;
	pid_t	*pids;
}	t_pipe_loop_params;

/* Function prototypes - organized by modules */

/* Core functions */
void		execute_shell_command(char **args, t_shell *shell, int cmd_count);
void		clean_exit(t_shell *shell, int status);
void		signal_handler(int signo);
void		setup_signals(void);

void		main_shell_loop(t_shell *shell);
int			process_empty_input(char *input);
void		handle_single_command(t_shell *shell, t_cmd **commands);
void		handle_logical_commands(t_shell *shell, t_cmd **commands, int count);
void		handle_piped_commands(t_shell *shell, t_cmd **commands, int count);
void		process_command(t_shell *shell, char *input);
int			init_pipex_for_commands(t_shell *shell, t_cmd **commands, int count);
void		cleanup_pipex_after_execution(t_shell *shell);
int			should_continue_logical(t_cmd **commands, int i, int prev_exit_status);
int			execute_logical_command(t_shell *shell, t_cmd **commands, int i);

/* Parsing functions */
void		free_args(char **args);
void		free_envp(char **envp);
void		process_command_words(t_cmd *cmd, char **words);
t_cmd		**tokenize_piped_commands(char *input, int *cmd_count,
				t_shell *shell);
char		**create_cmd_words(char **words, int start, int end);
void		process_final_command(t_cmd_process *proc, int start, int i);
t_cmd		**init_commands_array(char **words);
char		**tokenize_words(char *input, t_shell *shell);
void		process_input_chars(char *input, t_token_data *data,
				char *quote_state, t_shell *shell);
void		handle_redirection_token(t_token_data *data, char *input, int *i);
void		handle_pipe_token(t_token_data *data, int *i);
void		handle_logical_token(t_token_data *data, char *input, int *i);
void		handle_pipe_token_cmd(t_cmd_process *proc, int i);
void		handle_logical_token_cmd(t_cmd_process *proc, int i, int operator);
void		handle_separator_token(t_token_data *data, int *i);
int			init_cmd_args(t_cmd *cmd);
int			init_cmd_redirs(t_cmd *cmd);
void		init_cmd_defaults(t_cmd *cmd);
void		init_tokenize_data(t_token_data *data, t_tokenize_init *init);
int			init_tokenize_struct(t_tokenize_init *init, char *current_word,
				int *counts);
char		**handle_quote_error(char **words);
int			check_syntax_errors(char **words);
int			add_variable_to_word(t_var_expansion *var_exp, t_shell *shell);

void		add_char_to_word(char *word, int *pos, char c);
void		save_word(char **words, int *count, char *word, int len);
int			is_word_empty(char *word, int len);
int			is_empty_quotes(char *word, int len);
void		add_word_to_array(char **words, int *count, char *word, int len);
int			is_separator(char c);
int			is_quote(char c);
char		update_quote_state(char c, char current_state);
char		*get_env_value(char *var, char **envp);
t_cmd		*create_cmd(void);
void		add_redirection(t_cmd *cmd, int type, char *file);
int			get_redirection_type(char *token);
void		free_partial_cmds(t_cmd **cmds, int count);

/* Execution functions */
void		execute_external_commands(char **args, char **envp, t_shell *shell);
void		handle_parent_process(pid_t pid, int *status, char *path,
				t_shell *shell);
void		child_exit(char *path, int status);
void		handle_signal_termination(int status, t_shell *shell);
char		*validate_and_get_path(char **args, t_shell *shell);
void		handle_path_not_found(char *command, t_shell *shell);
void		setup_child_signals(void);
void		handle_redirections(t_cmd *cmd, t_shell *shell);
void		handle_output_redirection(t_cmd *cmd, int i);
void		handle_input_redirection(t_cmd *cmd, int i);
void		handle_heredoc(t_cmd *cmd, int i, t_shell *shell);
int			has_heredoc(t_cmd *cmd);
int			preprocess_all_heredocs(t_shell *shell, t_cmd **commands, int cmd_count);
void		close_parent_heredoc_fds(t_cmd **commands, int cmd_count);
void		setup_heredoc_signals(void);
void		restore_main_signals(void);
void		restore_redirections(t_cmd *cmd);
void		restore_output_redirection(t_cmd *cmd, int i);
void		restore_input_redirection(t_cmd *cmd, int i);
void		execute_piped_commands(t_shell *shell, t_cmd **commands,
				int cmd_count);
void		setup_child_pipes(t_shell *shell, int i, int cmd_count);
void		execute_child_command(t_shell *shell, t_cmd *command);
void		init_pipes(t_pipex *pipex, int cmd_count);
int			*allocate_pipe_fds(int pipe_count);
int			create_pipes(int *pipe_fds, int pipe_count);
void		close_pipes(t_pipex *pipex);
void		wait_all_children(pid_t *pids, int cmd_count);
void		setup_heredoc_redirection(t_cmd **commands, int i, int j);
void		setup_input_redirection(t_cmd **commands, int i, int j);
void		setup_output_redirection(t_cmd **commands, int i, int j);
void		handle_output_fd(int fd);
void		setup_child_redirections(t_cmd **commands, int i);
void		execute_child_with_params(t_child_exec_params *params);
void		process_pipe_iteration(t_pipe_iteration_params *params);
void		finalize_pipe_execution(t_cmd **commands, int cmd_count, int prev_fd,
				pid_t *pids);
void		execute_pipe_loop(t_pipe_loop_params *params);
int			create_pipe_for_command(int *pipe_fd, int i, int cmd_count);
int			fork_child_process(pid_t *pids, int i);
void		cleanup_parent_pipes(int *prev_fd, int *pipe_fd, int i, int cmd_count);
void		setup_input_pipe(t_shell *shell, int i);
void		setup_output_pipe(t_shell *shell, int i, int cmd_count);
void		close_all_pipe_fds(t_shell *shell);

/* Built-in functions */
int			builtin_echo(char **args, t_shell *shell);
int			builtin_cd(char **args, t_shell *shell);
int			builtin_pwd(t_shell *shell);
int			builtin_export(char **args, t_shell *shell);
int			builtin_unset(char **args, t_shell *shell);
int			builtin_env(t_shell *shell);
int			builtin_exit(char **args, t_shell *shell);

/* Built-in navigation helpers */
char		*get_cd_path(char **args, t_shell *shell);
int			handle_cd_error(char *old_pwd);
int			handle_chdir_error(char *path, char *old_pwd);
int			update_shell_pwd(t_shell *shell, char *old_pwd);
void		update_pwd_vars(t_shell *shell);

/* Utility functions */
char		*find_command_in_path(char *command);
char		*check_absolute_path(char *command);
char		*search_in_path(char *command, char **paths);
void		print_export_format(char **envp);
void		add_or_update_env(char *var, t_shell *shell);
int			update_existing_env_var(char *var, t_shell *shell);
void		add_new_env_var(char *var, t_shell *shell);
void		export_existing_var(char *var, t_shell *shell);

/* Memory management functions */
void		free_cmd(t_cmd *cmd);
void		free_cmds(t_cmd **cmds);

char		**dup_envp(char **envp);
void		cleanup_pipex(t_shell *shell);
void		cleanup_cmd(t_shell *shell);
void		cleanup_envp(t_shell *shell);
void		cleanup_local_envp(t_shell *shell);
void		cleanup_system_envp(t_shell *shell);
void		cleanup_work_paths(t_shell *shell);

void		cleanup_terminal_resources(void);

/* Fonctions utilitaires */
int		has_logical_operators(t_cmd **commands, int cmd_count);
int		is_valid_command(t_cmd **commands, int cmd_count);
void	handle_command_not_found(t_cmd **commands, int cmd_count,
			t_shell *shell);
void	handle_commands_execution(t_shell *shell, t_cmd **commands,
			int cmd_count);
int		should_continue_logical(t_cmd **commands, int i, int prev_exit_status);
int		execute_logical_command(t_shell *shell, t_cmd **commands, int i);
void	handle_redirections_context(t_shell *shell, int in_pipe_context);
void	restore_redirections_context(t_shell *shell, int in_pipe_context);
void	execute_command_by_type(char **args, t_shell *shell);
void	execute_builtin_command(char **args, t_shell *shell);
void	handle_exit_command(char **args, t_shell *shell);

/* tokenize_input helpers */
void	handle_variable_expansion(char *input, int *i, t_token_data *data,
		t_shell *shell);
void	handle_backslash(char *input, int *i, t_token_data *data);
void	handle_quote_error_input(char quote_state, t_token_data *data);
void	process_variable_expansion(t_var_expansion_context *ctx);
void	process_quote_handling(t_quote_context *ctx);
void	process_backslash_handling(char *input, int *i, t_token_data *data,
		char *quote_state);
void	handle_quotes(t_quote_context *ctx);
void	handle_quote_start(t_quote_context *ctx);
void	handle_quote_end(t_quote_context *ctx);
void	extract_var_name(char *input, int *i);
void	handle_empty_var(t_token_data *data);
void	expand_var_value(char *var_value, t_token_data *data);

int			check_signal_or_eof(int bytes_read);
char		*read_line_from_stdin(void);
char		*read_interactive_line(void);
char		*read_non_interactive_line(void);
void		handle_heredoc_error(int *pipe_fd, t_cmd *cmd, int i, t_shell *shell);
void		handle_heredoc_success(int *pipe_fd, t_cmd *cmd, int i);
void		handle_variable_in_iteration(t_char_iteration_context *ctx, int *i);
void		handle_quote_in_iteration(t_char_iteration_context *ctx, int *i);
void		handle_backslash_in_iteration(t_char_iteration_context *ctx, int *i);

#endif
