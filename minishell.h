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
int			err_count(char *str);
void		error_exit(int error);
void		fill_null(char **arr, int c);
void		fill_matrix(char **arr, char *str);

#endif
