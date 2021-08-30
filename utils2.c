#include "minishell.h"

int	check_export(char *arg)
{
	int	i;

	if (ft_isalpha(arg[0]) || arg[0] == '_')
	{
		i = 1;
		while (arg[i] && arg[i] != '=')
		{
			if (ft_isalnum(arg[i]) || arg[i] == '_')
				++i;
			else
				return (0);
		}
		return (1);
	}
	else
		return (0);
}