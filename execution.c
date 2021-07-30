#include "minishell.h"

int	exec_com(simple_com *s, int fd[2])
{
	if (builtin_com(s->command)) //not a builtin
	{
		
	}
}

void	exec(simple_com *s, int n)
{
	int 	i;
	int		fd[2];
	pid_t	pid;

	i = 0;
	pipe(fd);
	pid = 1;
	while (pid != 0 && i < n)
	{
		pid = fork();
		if (pid == 0)
			exec_com(s[i], fd);
		else
			wait(NULL);
		i++;
	}
}