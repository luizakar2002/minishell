#include "minishell.h"

int	exec_com(simple_com *s, int fd[2])
{
	int status;

	if (is_builtin(s->command))
	{
		dup2(s->infile, STDIN_FILENO);
		dup2(s->outfile, STDOUT_FILENO);
		if (execve(get_cmd_path(s), merge(s), myenv) == -1)
			ft_putstr_fd("lsh", 2);
		exit(0);
	}
	else 
	{
		status = call_command(s);
		exit(0);
	}
	return (0);
}

void	exec(simple_com *s, int n)
{
	int 	i;
	int		j;
	int		fd[n][2];
	int		pids[n];
	int		status;

	if (n == 1 && !is_builtin(s->command))
	{
		call_command(s);
		return ;
	}
	i = 0;
	while (i < n + 1)
	{
		if (pipe(fd[i]) == -1)
			error_exit(6);
		i++;
	}

	i = 0;
	pids[i] = 1;
	while (pids[i] != 0 && i < n)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			error_exit(7);	
		if (pids[i] == 0)
		{
			j = 0;
			while (j < n + 1)
			{
				if (i != j)
					close(fd[j][0]);
				if (i + 1 != j)
					close(fd[j][1]);
				j++;
			}
			if (s[i + 1].command != NULL && s[i].outfile == 1)
				s[i].outfile = fd[i + 1][1];
			else if (s[i + 1].command != NULL && s[i].outfile != 1)
				s[i].outfile = fd[i + 1][1];
			if (s[i].infile == 0 && i != 0)
				s[i].infile = fd[i][0];
			exec_com(&s[i], fd[i]);	
		}
		else
		{
			waitpid(pids[i], NULL, 0);
			close(fd[i + 1][1]);
		}
		i++;
	}
}

int	call_command(simple_com *s)
{
	int status;

	// if (compare(s->command, "echo"))
	// 	status = echo(s);
	/*else */if (compare(s->command, "cd"))
		status = changedir(s);
	else if (compare(s->command, "pwd"))
		status = pwd(s);
	else if (compare(s->command, "export"))
		status = export(s);
	else if (compare(s->command, "unset"))
		status = unset(s);
	else if (compare(s->command, "env"))
		status = print_env(s);
	else if (compare(s->command, "exit"))
		exit(0);
	return (status);
}
