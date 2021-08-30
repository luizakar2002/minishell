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
		printf("");
		// printf("%dth pipe open 0: %d 1: %d\n", i, fd[i][0], fd[i][1]);
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
			if (s[i + 1].command != NULL && s[i].outfile == 1)
			{
				s[i].outfile = fd[i][1];
				printf("");
				// printf("%d's outfile became: %d pipefd: %d\n", i, s[i].outfile, fd[i][1]);
			}
			if (s[i].infile == 0 && i != 0)
			{
				s[i].infile = fd[i - 1][0];
				printf("");
				// printf("%d's infile became: %d pipefd: %d\n", i, s[i].infile, fd[i - 1][0]);
			}
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (is_builtin(s[i].command))
			{
				dup2(s[i].infile, STDIN_FILENO);
				dup2(s[i].outfile, STDOUT_FILENO);
				printf("");
				// printf("after dup in:%d out:%d\n", s[i].infile, s[i].outfile);
				k = 0;
				while (k < n - 1)
				{
					if (fd[k][0] != 0)
					{
						printf("");
						// printf("child %dth pipe close 0: %d\n", k, fd[k][0]);
						close(fd[k][0]);
					}
					if (fd[k][1] != 1)
					{
						printf("");
						// printf("child %dth pipe close 1: %d\n", k, fd[k][1]);
						close(fd[k][1]);
					}
					k++;
				}
				if (s[i].infile != 0)
				{
					printf("");
					// printf("%d's infile close: %d\n", i, s[i].infile);
					close(s[i].infile);
				}
				if (s[i].outfile != 1)
				{
					printf("");
					// printf("%d's outfile close: %d\n", i, s[i].outfile);
					close(s[i].outfile);
				}
				if (execve(get_cmd_path(s + i, e), merge(s + i), e->myenv) == -1)
					ft_putstr_fd("Command not found\n", 2);
				exit(126);
			}
			else
			{
				k = 0;
				while (k < n - 1)
				{
					if (fd[k][0] != 0)
					{
						printf("");
						// printf("child %dth pipe close 0: %d\n", k, fd[k][0]);
						close(fd[k][0]);
					}
					if (fd[k][1] != 1)
					{
						printf("");
						// printf("child %dth pipe close 1: %d\n", k, fd[k][1]);
						close(fd[k][1]);
					}
					k++;
				}
				if (s[i].infile != 0)
				{
					printf("");
					// printf("%d's infile close: %d\n", i, s[i].infile);
					close(s[i].infile);
				}
				if (s[i].outfile != 1)
				{
					printf("");
					// printf("%d's outfile close: %d\n", i, s[i].outfile);
					close(s[i].outfile);
				}
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
		{
			printf("");
			// printf("parent %dth pipe close 0: %d\n", j, fd[j][0]);
			close(fd[j][0]);
		}
		if (fd[j][1] != 1)
		{
			printf("");
			// printf("parent %dth pipe close 1: %d\n", j, fd[j][1]);
			close(fd[j][1]);
		}
		j++;
	}
	j = 0;
	while (j < n)
	{
		
		pid = waitpid(-1, &status, 0);
		//we only need the status of the last command
		if (pid == pids[n - 1])
		{
			if (!WTERMSIG(status))
				status = WEXITSTATUS(status);
			else
				status = WTERMSIG(status) + 128;
			if (status == 131)          //should be considered in case of quit
				write(1, "Quit\n", 5);
		}
		j++;
	}
}

int    call_command(simple_com *s, t_env *e, int ex)
{
    int stat;

    stat = 0;
    if (compare(s->command, "echo"))
        stat = echo(s, ex);
    else if (compare(s->command, "cd"))
        stat = changedir(s, e, ex);
    else if (compare(s->command, "pwd"))
        stat = pwd(s, ex);
    else if (compare(s->command, "export"))
        stat = export(s, e, ex);
    else if (compare(s->command, "unset"))
        stat = unset(s, e, ex);
    else if (compare(s->command, "env"))
        stat = print_env(s, e, 0, ex);
    else if (compare(s->command, "exit"))
        exit(0);
    return (stat);
}
