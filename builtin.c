#include "minishell.h"

int	exitt(simple_com *s, int ex)
{
	ex++;
	if (s->arg && !only_digits(s->arg[0]))
	{
		status = 255;
		if (ex == 0)
			write(2, "exit\n", 5);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(s->arg[0], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(255);
	}
	else if (s->arg && s->arg[1] != NULL)
	{
		status = 1;
		if (ex == 0)
			write(2, "exit\n", 5);
		write(2, "minishell: exit: too many arguments\n", 31);
		return (1);
	}
	else if (s->arg && s->arg[1] == NULL)
	{
		status = ft_atoi(s->arg[0]);
		if (ex == 0)
			write(2, "exit\n", 5);
		exit(status);
	}
	else if (!s->arg)
	{
		if (ex == 0)
			write(2, "exit\n", 5);
		exit(0);
	}
	return (0);
}

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
	char 	*path;

	i = 0;
	path = malloc(sizeof(char) * 1000);
	getcwd(path, 1000);
	if (s->option)
	{
		if (get_env("OLDPWD", e))
		{
			if (chdir(get_env("OLDPWD", e)))
			{
				if (ex == 1)
					error_exit(3);
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(get_env("OLDPWD", e), 2);
				ft_putendl_fd(": No such file or directory", 2);
    			return (1);
			}
		}
		else
		{
			if (ex == 1)
				error_exit(3);
			ft_putendl_fd("OLDPWD not set", 2);
			return (1);
		}

	}
	else if (!s->arg)
	{
		if (get_env("HOME", e))
		{
			if (chdir(get_env("HOME", e)))
			{
				if (ex == 1)
					error_exit(3);
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(get_env("HOME", e), 2);
				ft_putendl_fd(": No such file or directory", 2);
    			return (1);
			}
		}
		else
		{
			if (ex == 1)
				error_exit(6);
			ft_putendl_fd("HOME not set", 2);
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
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(s->arg[0], 2);
				ft_putendl_fd(": No such file or directory", 2);
    			return (1);
			}
		}
		else
		{
			while (e->myenv[i] && ft_strncmp(e->myenv[i], "PWD=", 4) != 0)
				++i;
			if (!e->myenv[i])
			{
				char *path1;
				path1 = malloc(sizeof(char) * 1000);
				getcwd(path1, 1000);
				temp = ft_strjoin(ft_strjoin(path1, "/", 1), s->arg[0], 0);
			}
			else
				temp = ft_strjoin(ft_strjoin(e->myenv[i] + 4, "/", 0), s->arg[0], 0);
			if (chdir(temp))
			{
				if (ex == 1)
				{
					free(temp);
					error_exit(3);
				}
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(temp, 2);
				ft_putendl_fd(": No such file or directory", 2);
				free(temp);
    			return (1);
			}
			free(temp);
		}
	}
	e->myenv = update_env(e, path);
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
		if (get_env(key, e) && ft_strncmp(get_env(key, e), "''", 2) == 0)
		{
			if (flag != 0)
			{
				write(s->outfile, "declare -x ", 11);
				write(s->outfile, e->myenv[i], ft_strlen(e->myenv[i]) - 2);
				write(s->outfile, "\n", 1);
			}
		}
		else
		{
			if (flag == 1)
				write(s->outfile, "declare -x ", 11);
			ft_putendl_fd(e->myenv[i], s->outfile);
		}
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
						if (ft_strncmp(key, e->myenv[j], ft_strlen(key)) == 0 && check_export(s->arg[m]) == 2)
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
						temp = ft_strjoin(s->arg[m], "=''", 0);
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
			{
				flag = 1;
				ft_putstr_fd("minishell: unset: `", 2);
				ft_putstr_fd(s->arg[m], 2);
				ft_putendl_fd("\': not a valid identifier", 2);
			}
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
	int		flag;
	char	*temp;

	i = 0;
	flag = 0;
	if (!s->arg)
	{
		if (ex == 1)
			error_exit(0);
		return (0);
	}
	while (s->arg[i])
	{
		j = 0;
		k = 0;
		size = env_size(e->myenv) + 1;
		if (!(newenv = malloc(sizeof(char *) * size)))
		{
			if (ex == 1)
				error_exit(1);
			return (1);
		}
		if (ft_strchr(s->arg[i], '='))
		{
			flag = 1;
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(s->arg[i], 2);
			ft_putendl_fd("\': not a valid identifier", 2);
			free(newenv);
		}
		else
		{
			temp = ft_strjoin(s->arg[i], "=", 0);
			while (e->myenv[j])
			{
				if (ft_strncmp(e->myenv[j], temp, ft_strlen(temp)) != 0)
					newenv[k++] = ft_strdup(e->myenv[j]);
				j++;
			}
			free(temp);
			newenv[k] = NULL;
			free_2d(e->myenv);
			e->myenv = newenv;
		}
		i++;
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
