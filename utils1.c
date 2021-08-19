#include "minishell.h"

char	**merge(simple_com *s)
{
	char	**r;
	int		size;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (s->option)
		++size;
	r = malloc(sizeof(char *) * size);
	r[i++] = s->command;
	if (s->option)
		r[i++] = s->option;
	if (s->arg)
		while (s->arg[j])
			r[i++] = s->arg[j++];
	r[i] = NULL;
	return (r);
}

char *get_cmd_path(simple_com *s, t_env *e)
{
    int i = 0;
    char *command;
    char *path;
    int fd;
	char	**paths;

	while (e->myenv[i] && ft_strncmp(e->myenv[i], "PATH=", 5))
		++i;
	paths = ft_split(e->myenv[i], ':');
    if (ft_strchr(s->command, '/'))
		return (s->command);
	i = -1;
    while(paths[++i])
    {
        command = ft_strjoin("/", s->command);
        path = ft_strjoin(paths[i], command);
        fd = open(path, O_RDONLY);
        if (fd > -1)
        {
            close(fd);
            free(command);
			//free paths
            return(path);
        }
        free(command);
        free(path);
    }
    //exit_message(data, "Commmand not found\n", EXIT_FAILURE);
    return (NULL);
}

int env_size(char **env)
{
	int size;

	if (!env)
		return (0);
	size = 0;
	while (env[size])
		size++;
	return (size);
}

void	free_2d(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}