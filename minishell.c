#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	simple_com *s;
	int			n;
	char		*str;
	int			i;
	t_env		*e;

	i = -1;
	e = malloc(sizeof(t_env)); 
	n = env_size(env) + 1;
	e->myenv = malloc(sizeof(char *) * n);
	while (env[++i])
		e->myenv[i] = ft_strdup(env[i]);
	e->myenv[i] = NULL;
	while (1)
	{
		signal(SIGINT, &handle_sigint);
		signal(SIGQUIT, &handle_sigint);
		str = readline("minishell > ");
		while (str && str[0] == '\0')
			str = readline("minishell > ");
		if (!str)
			return (0);
		add_history(str);
		if (!ft_strncmp(str, "exit", 4))
			break ;
		n = char_count(str, '|') + 1;
		s = split_pipes(str, e->myenv);
		// print(s);
		exec(s, n, e);
		free(str);
	}
	return (0);
}