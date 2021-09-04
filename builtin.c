#include "minishell.h"

int	echo(simple_com *s, int ex)
{
	int i;

	i = -1;
	if (!(s->arg))
	{
		write(s->outfile, "\n", 1);
		return (0);
	}
	while (s->arg[++i])
	{
		write(s->outfile, s->arg[i], ft_strlen(s->arg[i]));
		if (s->arg[i + 1] != NULL)
			write(s->outfile, " ", 1);
	}
	if (!s->option)
		write(s->outfile, "\n", 1);
	if (ex == 1)
		error_exit(0);
	return (0);
}

int changedir(simple_com *s, t_env *e, int ex) 
{
	int 	i;
	char	*temp;

	i = 0;
	if (!s->arg)
	{
		if (get_env("HOME", e))
		{
			if (chdir(get_env("HOME", e)))
			{
				if (ex == 1)
					error_exit(3);
    			return (1);
			}
		}
		else
		{
			if (ex == 1)
				error_exit(6);
			return (1);			
		}
	}
	else if (s->arg[0][0] == '-')
	{
		if (get_env("OLDPWD", e))
		{
			if (chdir(get_env("OLDPWD", e)))
			{
				if (ex == 1)
					error_exit(3);
    			return (1);
			}
		}
		else
		{
			if (ex == 1)
				error_exit(3);
			return (1);
		}
	}
	else
	{
		if (ft_strchr(s->arg[0], '/'))
		{
			if (chdir(s->arg[0]))
			{
				if (ex == 1)
					error_exit(3);
    			return (1);
			}
		}
		else
		{
			while (e->myenv[i] && ft_strncmp(e->myenv[i], "PWD=", 4))
				++i;
			temp = ft_strjoin(ft_strjoin(e->myenv[i] + 4, "/", 0), s->arg[0], 0);
			if (chdir(temp))
			{
				free(temp);
				if (ex == 1)
					error_exit(3);
    			return (1);
			}
			free(temp);
		}
	}
	e->myenv = update_env(e);
	if (ex == 1)
		error_exit(0);
    return (0);
}

int pwd(simple_com *s, int ex)
{
	char *path;

	path = malloc(sizeof(char) * 1000);
	if (!getcwd(path, 1000))
	{
		if (ex == 1)
			error_exit(3);
		return (1);
	}
	ft_putendl_fd(path, s->outfile);

	if (ex == 1)
		error_exit(0);
	return (0);
}

int print_env(simple_com *s, t_env *e, int flag, int ex)
{
	int		i;
	char	*key;

	i = 0;
	while (e->myenv[i])
	{
		key = ft_substr(e->myenv[i], 0, ft_strlen(e->myenv[i]) - ft_strlen(ft_strchr(e->myenv[i], '=')));
		if (flag == 0 && get_env(key, e) && get_env(key, e)[0] == '\0');
		else
			ft_putendl_fd(e->myenv[i], s->outfile);
		++i;
	}
	if (ex == 1)
		error_exit(0);
	return (0);
}

int export(simple_com *s, t_env *e, int ex)
{
	int		size;
	char	**newenv;
	int		j;
	int		m;
	char	*key;
	char	*temp;
	int		flag;

	m = 0;
	flag = 0;
	if (!s->arg)
	{
		print_env(s, e, 1, ex);
	}
	else
	{
		while (s->arg[m])
		{
			if (check_export(s->arg[m]))
			{
				key = ft_substr(s->arg[m], 0, ft_strlen(s->arg[m])  - ft_strlen(ft_strchr(s->arg[m], '=')));
				if (get_env(key, e) != NULL)
				{
					j = 0;
					while (e->myenv[j])
					{
						if (ft_strncmp(key, e->myenv[j], ft_strlen(key)) == 0)
						{
							free(e->myenv[j]);
							e->myenv[j] = ft_strdup(s->arg[m]);
						}
						++j;
					}
				}
				else
				{
					size = env_size(e->myenv);
					newenv = malloc(sizeof(char *) * (size + 2));
					if (!newenv)
					{
						if (ex == 1)
							error_exit(1);
						return (1);
					}
					j = 0;
					while (j < size - 1)
					{
						newenv[j] = ft_strdup(e->myenv[j]);
						++j;
					}
					if (!ft_strchr(s->arg[m], '='))
					{
						temp = ft_strjoin(s->arg[m], "=", 0);
						newenv[j++] = ft_strdup(temp);	
						free(temp);
					}
					else
						newenv[j++] = ft_strdup(s->arg[m]);	
					newenv[j] = ft_strdup(e->myenv[size - 1]);
					newenv[j + 1] = NULL;
					free_2d(e->myenv);
					e->myenv = newenv;
				}
			}
			else
				flag = 1;
			++m;
		}
	}
	if (flag == 1)
	{
		if (ex == 1)
			error_exit(1);
		return (1);		
	}
	if (ex == 1)
		error_exit(0);
	return (0);
}

int unset(simple_com *s, t_env *e, int ex)
{
	int		size;
	char	**newenv;
	int		i;
	int		j;
	int		k;

	size = env_size(e->myenv) + 1;
	size -= env_size(s->arg);
	if (!(newenv = malloc(sizeof(char *) * size)))
	{
		if (ex == 1)
			error_exit(1);
		return (1);
	}
	i = -1;
	j = 0;
	while (++i < env_size(e->myenv))
	{	
		k = 0;
		while (k < env_size(s->arg))
		{
			if (ft_strncmp(e->myenv[i], s->arg[k], ft_strlen(s->arg[k])))
				newenv[j++] = ft_strdup(e->myenv[i]);
			++k;
		}
	}
	newenv[j] = NULL;
	free_2d(e->myenv);
	e->myenv = newenv;
	if (ex == 1)
		error_exit(0);
	return (0);
}

