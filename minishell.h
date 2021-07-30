#ifndef	MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <stdio.h>
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
	char	**infile;
	char	**outfile;
	char	**errfile;
}				simple_com;

char		*token(char *str, simple_com *s);
simple_com	*init_simple_com(simple_com *s, char *str);
int			char_count(char *str, char c);
simple_com	*fill_struct(char *str);
int			check(char *str, simple_com *s);
int			str_count(char *str, char *s);
void		error_exit(int error);
void		fill_null(char **arr, int c);
void		fill_matrix(char **arr, char *str);
char		*add_front(char *str, char c);
int			compare(char *s1, char *s2);
char		*remove_quote(char *str);
int			builtin_command(char *com);
void		exec(simple_com *s, int n);
int			special_char(char *str);

#endif
