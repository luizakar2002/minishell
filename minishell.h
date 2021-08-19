#ifndef	MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>
# include <termios.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

///define errors
typedef struct s_str
{
	char	*command;
	char	*option;
	char	**arg;
	int		infile;
	int		outfile;
	int		errfile;
	// char	**env;
	//keep status
}				simple_com;

typedef struct env
{
	char **myenv;
}				t_env;

//main.c

//builtin.c
int changedir(simple_com *s, t_env *e);
int pwd(simple_com *s);
int print_env(simple_com *s, t_env *e);
int export(simple_com *s, t_env *e);
int unset(simple_com *s, t_env *e);

//execution.c
int		exec_com(simple_com *s, int fd[2], t_env *e);
void	exec(simple_com *s, int n, t_env *e);
int		call_command(simple_com *s, t_env *e);

//utils.c
int		char_count(char *str, char c);
int		str_count(char *str, char *s);
void	error_exit(int error);
void	fill_null(char **arr, int c);
void	fill_fd(int *arr, char *str, int flag);
char	*add_front(char *str, char c);
int		compare(char *s1, char *s2);
int		special_char(char *str);
char	*remove_quote(char *str);
int		is_builtin(char *com);
char 	**create_arg(simple_com *s, char *str);
void	print(simple_com *s);
int		check(char *str, simple_com *s);

//utils1.c
char	**merge(simple_com *s);
char 	*get_cmd_path(simple_com *s, t_env *e);
int 	env_size(char **env);
void	free_2d(char **arr);

//lex.c
simple_com	*init_simple_com(simple_com *s, char *str, char **env);
char		*token(char *str, simple_com *s);
simple_com	*fill_struct(char *str, char **env);
simple_com	*split_pipes(char *str, char **env);
void		handle_sigint(int sig);

#endif
