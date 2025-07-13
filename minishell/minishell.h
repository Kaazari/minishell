#ifndef MINISHELL_h
# define MINISHEL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <term.h>
# include "libft/libft.h"

# define REDIR_OUT 1    // >
# define REDIR_APPEND 2 // >>
# define REDIR_IN 3     // <

// type = > < >>
typedef struct s_redir
{
	char	*file;
	int		type;
	int		fd;
}	t_redir;

// tokenized commands
typedef struct s_cmd
{
	char		**args;
	t_redir		*redirs;
	int			redir_count;
}	t_cmd;

// shell struct (envp)
typedef struct s_shell
{
	char	**envp;
	char	**local_envp;
	t_cmd	*cmd;
}	t_shell;

// Execution functions
void execute_command(char **args, t_shell *shell);
void execute_external_commands(char **args, char **envp);

// Parsing Tokenisation
t_cmd *tokenize_input(char *input);
void free_args(char **args);
void free_cmd(t_cmd *cmd);
t_cmd *create_cmd(void);

// Utils
char *find_command_in_path(char *command);
void print_export_format(char **envp);
void add_or_update_env(char *var, t_shell *shell);
void export_existing_var(char *var, t_shell *shell);

// Builtins
void builtin_cd(char **args);
void builtin_pwd(char **args);
void builtin_export(char **args, t_shell *shell);
void builtin_unset(char **args, t_shell *shell);
void builtin_env(char **args, t_shell *shell);
void builtin_exit(char **args);
void builtin_echo(char **args);

// Redirections > < >>
void	handle_redirections(t_cmd *cmd);
void	restore_redirections(t_cmd *cmd);

// Tokenisation Utils
int		is_separator(char c);
int		is_quote(char c);
char	update_quote_state(char c, char current_state);
void	add_char_to_word(char *word, int *pos, char c);
void	save_word(char **words, int *count, char *word, int len);
int		add_variable_to_word(char *input, int i, char *word, int *pos);
void add_char_to_token(char **token, int *pos, int *size, char c);
void save_token(char ***args, int *arg_count, char *token, int token_pos);
char *expand_variable(char *var_name);

#endif
