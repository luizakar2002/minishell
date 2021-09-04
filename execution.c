#include "minishell.h"

void	exec(simple_com *s, int n, t_env *e)
{
	int 	i;
	int		j;
	int		fd[n - 1][2];
	int		pid;
	int		pids[n];
	int		k;

	if (n == 1 && !is_builtin(s->command))
	{
		call_command(s, e, 0);
		return ;
	}
	i = 0;
	while (i < n - 1)
	{
		if (pipe(fd[i]) == -1)
			error_exit(6);
		i++;
	}
	i = 0;
	j = 0;
	pids[i] = 1;
	while (pids[j] != 0 && i < n)
	{
		if (s[i].infile == -1)
		{
			i++;
		}
		j = i;
		pids[i] = fork();
		if (pids[i] == -1)
			error_exit(7);
		if (pids[i] == 0)
		{
			if (s[i + 1].command != NULL && s[i].outfile == 1)
				s[i].outfile = fd[i][1];
			if (s[i].infile == 0 && i != 0)
				s[i].infile = fd[i - 1][0];
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			dup2(s[i].infile, STDIN_FILENO);
			dup2(s[i].outfile, STDOUT_FILENO);
			if (is_builtin(s[i].command))
			{
				k = 0;
				while (k < n - 1)
				{
					if (fd[k][0] != 0)
						close(fd[k][0]);
					if (fd[k][1] != 1)
						close(fd[k][1]);
					k++;
				}
				if (s[i].infile != 0)
					close(s[i].infile);
				if (s[i].outfile != 1)
					close(s[i].outfile);
				execve(get_cmd_path(s + i, e), merge(s + i), e->myenv);
					// dprintf(2, "Command not found\n");
				exit(127);
			}
			else
			{
				k = 0;
				while (k < n - 1)
				{
					if (fd[k][0] != 0)
						close(fd[k][0]);
					if (fd[k][1] != 1)
						close(fd[k][1]);
					k++;
				}
				if (s[i].infile != 0)
					close(s[i].infile);
				if (s[i].outfile != 1)
					close(s[i].outfile);
				s[i].outfile = 1;
				call_command(s + i, e, 1);
			}
		}
		i++;
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	j = 0;
	while (j < n - 1)
	{
		if (fd[j][0] != 0)
			close(fd[j][0]);
		if (fd[j][1] != 1)
			close(fd[j][1]);
		j++;
	}
	j = 0;
	while (j < n)
	{
		
		pid = waitpid(-1, &status, 0);
		if (pid == pids[n - 1])
		{
			if (!WTERMSIG(status))
				status = WEXITSTATUS(status);
			else
				status = WTERMSIG(status) + 128;
			if (status == 131)
				write(1, "Quit\n", 5);
			else if (status == 127 || status == 126)
				printf("Command not found\n");
		}
		j++;
	}
}

void    call_command(simple_com *s, t_env *e, int ex)
{
    if (!ft_strncmp(s->command, "echo", 5))
        status = echo(s, ex);
    else if (!ft_strncmp(s->command, "cd", 3))
        status = changedir(s, e, ex);
    else if (!ft_strncmp(s->command, "pwd", 4))
        status = pwd(s, ex);
    else if (!ft_strncmp(s->command, "export", 7))
        status = export(s, e, ex);
    else if (!ft_strncmp(s->command, "unset", 6))
        status = unset(s, e, ex);
    else if (!ft_strncmp(s->command, "env", 4))
        status = print_env(s, e, 0, ex);
    else if (!ft_strncmp(s->command, "exit", 5))
        exit(0);
	if (status == 1)
		printf("Wrong Input\n");
}
