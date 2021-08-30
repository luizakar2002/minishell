#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	simple_com *s;
	int			n;
	char		*str;
	int			i;
	t_env		*e;

	status = 0;
	if (ac != 1 || av[1] != NULL)
		return (0);
	i = -1;
	e = malloc(sizeof(t_env)); 
	if (!e)
		error_exit(5);
	n = env_size(env) + 1;
	e->myenv = malloc(sizeof(char *) * n);
	if (!e->myenv)
		error_exit(5);
	while (env[++i])
		e->myenv[i] = ft_strdup(env[i]); 
	e->myenv[i] = NULL;
	signal(SIGINT, &handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	// signal(SIGCHLD, &handle_sigint);
	while (1)
	{
		str = readline("\033[0;33mminishell > \033[0m");
		while (str && (str[0] == '\0'))
			str = readline("\033[0;33mminishell > \033[0m");
		if (!str)
		{
			return (0);
		}
		add_history(str);
		if (!ft_strncmp(str, "exit", 4))
			break ;
		n = char_count(str, '|') + 1;
		s = split_pipes(str, e);
		exec(s, n, e);
		signal(SIGINT, &handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		int j = 0;
		while (j < n)
		{
			if (s[j].infile != 0)
			{
				// dprintf(2, "parent infile: %d\n", s[j].infile);
				close(s[j].infile);
			}
			if (s[j].outfile != 1)
			{
				// dprintf(2, "parent outfile: %d\n", s[j].outfile);
				close(s[j].outfile);
			}
			j++;
		}
		free(str);
	}
	return (0);
}