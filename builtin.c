// #include "minishell.h"

// int	echo(simple_com *s)
// {
// 	write(s->outfile, s->arg, ft_strlen(s->arg));
// 	return (0);
// }

// int chdir(simple_com *s)
// {
// 	int i = 0;
//     if (ft_strchr(s->command, '/'))
// 		chdir(s->command);
// 	while (s->env[i] && ft_strncmp(s->env[i], "PWD=", 4))
// 		++i;
// 	chdir(ft_strjoin(s->env[i] + 4, s->command));
//     return (0);
// }
