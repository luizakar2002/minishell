#include "minishell.h"

int	echo(simple_com *s)
{
	write(s->outfile, s->arg, ft_strlen(s->arg));
	return (0);
}

int changedir(simple_com *s)
{
	int i;

	i = 0;
	if (!s->arg)
	{
		while (s->env[i] && ft_strncmp(s->env[i], "HOME=", 5))
			++i;
		if (chdir(s->env[i] + 5))
			ft_putstr_fd("Not changed\n", 2);
	}
	else
	{
		if (ft_strchr(s->arg, '/'))
		{
			if (chdir(s->arg))
				ft_putstr_fd("Not changed\n", 2);
		}
		else
		{
			while (s->env[i] && ft_strncmp(s->env[i], "PWD=", 4))
				++i;
			if (chdir(ft_strjoin(ft_strjoin(s->env[i] + 4, "/"), s->arg)))
				ft_putstr_fd("Not changed\n", 2);
		}
	}
    return (0);
	//-----------------------------------------update env 
}

int pwd(simple_com *s)
{
	char *path;

	path = malloc(sizeof(char) * 1000);
	getcwd(path, 1000);
	ft_putendl_fd(path, s->outfile);
	return (0);
}

int print_env(simple_com *s)
{
	int	i;

	i = 0;
	while (s->env[i])
	{
		ft_putstr_fd(s->env[i++], s->outfile);
		ft_putstr_fd("\n", s->outfile);
	}
	return (0);
}

// int export(simple_com *s)
// {
// 	//--------args should be 2d arr
// 	int		size;
// 	char	**newenv;
// 	int		i;
// 	int		j;

// 	size = env_size(s->env);
// 	while (i < size)
// 		++i;
// 	size += arg_count(s);
// 	newenv = malloc(sizeof(char *) * size);
// 	if (!newenv)
// 		error_exit(1);
// 	j = -1;
// 	while (++j < i)
// 	{
// 		if (!(newenv[j] = malloc(sizeof(char) * ft_strlen(s->env[j]))))
// 			error_exit(1);
// 		newenv[j] = s->env[j];
// 	}

// }

// int unset(simple_com *s)
// {
// 	int		size;
// 	char	**newenv;
// 	int		i;
// 	int		j;

// 	size = env_size(s->env);
// 	size -= arg_count(s);
// 	if (!(newenv = malloc(sizeof(char *) * size)))
// 		error_exit(1);
	
// }

