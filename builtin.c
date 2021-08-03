#include "minishell.h"

// int	echo(simple_com *s)
// {
// 	write(s->outfile, s->arg, ft_strlen(s->arg));
// 	return (0);
// }

int changedir(simple_com *s)
{
	int i;

	i = 0;
	if (!s->arg)
	{
		while (myenv[i] && ft_strncmp(myenv[i], "HOME=", 5))
			++i;
		if (chdir(myenv[i] + 5))
			ft_putstr_fd("Not changed\n", 2);
	}
	else
	{
		if (ft_strchr(s->arg[0], '/'))
		{
			if (chdir(s->arg[0]))
				ft_putstr_fd("Not changed\n", 2);
		}
		else
		{
			while (myenv[i] && ft_strncmp(myenv[i], "PWD=", 4))
				++i;
			if (chdir(ft_strjoin(ft_strjoin(myenv[i] + 4, "/"), s->arg[0])))
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
	while (myenv[i])
		ft_putendl_fd(myenv[i++], s->outfile);
	return (0);
}

int export(simple_com *s)
{
	int		size;
	char	**newenv;
	int		i;
	int		j;
	int		k;

	size = env_size(myenv) + 1;
	while (i < size - 2)
		++i;
	size += env_size(s->arg);
	newenv = malloc(sizeof(char *) * size);
	if (!newenv)
		error_exit(1);
	j = -1;
	while (++j < i)
	{
		if (!(newenv[j] = malloc(sizeof(char) * (ft_strlen(myenv[j]) + 1))))
			error_exit(1);
		newenv[j] = myenv[j];
	}
	k = 0;
	while (s->arg[k])
	{
		if (!(newenv[j] = malloc(sizeof(char) * (ft_strlen(s->arg[k]) + 1))))
			error_exit(1);
		newenv[j++] = s->arg[k++];		
	}
	if (!(newenv[j] = malloc(sizeof(char) * (ft_strlen(myenv[i]) + 1))))
		error_exit(1);
	newenv[j] = myenv[i];
	newenv[j + 1] = NULL;
	myenv = newenv;
	print_env(s);
	return (0);
	//free old env
}

int unset(simple_com *s)
{
	int		size;
	char	**newenv;
	int		i;
	int		j;
	int		k;

	size = env_size(myenv) + 1;
	size -= env_size(s->arg);
	if (!(newenv = malloc(sizeof(char *) * size)))
		error_exit(1);
	i = -1;
	j = 0;
	while (++i < env_size(myenv))
	{	
		k = 0;
		while (k < env_size(s->arg))
		{
			if (ft_strncmp(myenv[i], s->arg[k], ft_strlen(s->arg[k])))
			{
				if (!(newenv[j] = malloc(sizeof(char) * (ft_strlen(myenv[i]) + 1))))
					error_exit(1);
				newenv[j++] = myenv[i];
			}
			++k;
		}
	}
	newenv[j] = NULL;
	myenv = newenv;
	print_env(s);
	return (0);
	//free old env
}

