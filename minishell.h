#ifndef	MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>
# include <termios.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

int status;

typedef struct s_str
{
	char	*command;
	char	*option;
	char	**arg;
	int		infile;
	int		outfile;
	int		stat;
	char	**dlm;
	int		herdoc;
}				simple_com;

typedef struct env
{
	char **myenv;
}				t_env;

//builtin.c
int changedir(simple_com *s, t_env *e, int ex);
int pwd(simple_com *s, int ex);
int print_env(simple_com *s, t_env *e, int flag, int ex);
int export(simple_com *s, t_env *e, int ex);
int unset(simple_com *s, t_env *e, int ex);
int	exitt(simple_com *s, int ex);
int	echo(simple_com *s, int ex);

//execution.c
int		exec_com(simple_com *s, t_env *e);
void	exec(simple_com *s, int n, t_env *e);
void	call_command(simple_com *s, t_env *e, int ex);

//utils.c
int		char_count(char *str, char c);
int		str_count(char *str, char *s);
void	error_exit(int error);
void	fill_null(char **arr, int c);
void	fill_fd(int *arr, char *str, int flag);
char	*add_front(char *str, char c);
int		compare(char *s1, char *s2);
int		special_char(char *str);
int		is_builtin(char *com);
char 	**create_arg(char **s, char *str, int flag);
int		check(char *str, simple_com *s);

//utils1.c
int		is_redir(char *s);
char	*redir(char *str);
char	**merge(simple_com *s);
char	*remove_quote(char *str, t_env *e);
char 	*get_cmd_path(simple_com *s, t_env *e);
int 	env_size(char **env);
char 	*get_env(char *key, t_env *e);
char	**update_env(t_env *e, char *oldpath);
char 	*parse_com(char *com);
void	free_2d(char **arr);


//utils2.c
int		check_export(char *arg);
int		heredoc(simple_com *s, t_env *e);
char	*parse_heredoc(char *str, t_env *e);
void	free_coms(simple_com *s);
int		is_valid(char *str);
int 	check_com(char *com);
int		only_digits(char *str);

//lex.c
simple_com	*init_simple_com(simple_com *s);
void		token(char *str, simple_com *s);
simple_com	*fill_struct(char *str, t_env *env);
simple_com	*split_pipes(char *str, t_env *env);
void		handle_sigint(int sig);
char		**divide(char *s);

#endif
