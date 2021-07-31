#ifndef	MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

///define errors
///trying to push
typedef struct s_str
{
	char	*command;
	char	*option;
	char	*arg;
	int		infile;
	int		outfile;
	int		errfile;
	char	**env;
	//keep status
}				simple_com;

//new struct with env

char		*token(char *str, simple_com *s);
simple_com	*init_simple_com(simple_com *s, char *str, char **env);
int			char_count(char *str, char c);
simple_com	*fill_struct(char *str, char **env);
int			check(char *str, simple_com *s);
int			str_count(char *str, char *s);
void		error_exit(int error);
void		fill_null(char **arr, int c);
void		fill_fd(int arr, char *str, int flag);
char		*add_front(char *str, char c);
int			compare(char *s1, char *s2);
char		*remove_quote(char *str);
int			is_builtin(char *com);
void		exec(simple_com *s, int n);
int			special_char(char *str);
simple_com	*split_pipes(char *str, char **env);
char		**merge(simple_com *s);
int			exec_com(simple_com *s, int fd[2]);
int			echo(simple_com *s);
int			call_command(simple_com *s);
char		*get_cmd_path(simple_com *s);




#endif
