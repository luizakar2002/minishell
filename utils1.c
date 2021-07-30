#include "minishell.h"

//////getpath
char	**merge(simple_com *s)
{
	char	**r;
	int		size;
	int		i;

	size = 4;
	i = 0;
	if (!s->option)
		size--;
	if (!s->arg)
		size--;
	r = malloc(sizeof(char *) * size);
	r[i++] = s->command;
	if (s->option)
		r[i++] = s->option;
	if (s->arg)
		r[i++] = s->arg;
	r[i] = NULL;
	return (r);
}

char *get_cmd_path(simple_com *s)
{
    int i = 0;
    char *command;
    char *path;
    int fd;
	char	**paths;

	while (s->env[i] && ft_strncmp(s->env[i], "PATH=", 5))
		++i;
	paths = ft_split(s->env[i], ':');
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