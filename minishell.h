#ifndef	MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <stdio.h>
# include "libft/libft.h"

///define errors
///trying to push
typedef struct s_str
{
	char	*command;
	char	*option;
	char	*arg;
	char	*infile;
	char	*outfile;
	char	*errfile;
}				simple_com;

char		*token(char *str, char **arg);
simple_com	*init_simple_com(simple_com *s);
int			pipe_count(char *str);
simple_com	*fill_struct(char *str);
void		error_exit(int error);

#endif
