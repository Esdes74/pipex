/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 17:21:42 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/10 10:58:53 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	child(int outin[2], char **av, char **environ)
{
	int		id;

	id = fork();
	if (id == 0)
		return (exec_child(outin, av, environ));
	else if (id == -1)
		return (perror("fork child"), 1);
	else
		return (0);
}

static int	parent(int outin[2], char **av, char **environ)
{
	int		id;

	id = fork();
	if (id == 0)
		return (exec_parent(outin, av, environ));
	else if (id == -1)
		return (perror("fork parent"), 1);
	else
		return (0);
}

int	main(int ac, char **av, char *environ[])
{
	int		outin[2];

	if (ac == 5 && environ != NULL)
	{
		if (pipe(outin) == -1)
			ft_printf("Error : There is a probleme with the pipe system.\n");
		if (child(outin, av, environ) == 1)
			return (1);
		if (parent(outin, av, environ) == 1)
			return (1);
		close_pipe(outin);
		wait(NULL);
		wait(NULL);
	}
	else
		ft_printf("Error : You don't have enought parameters.\n Or the \
environnement is NULL\n");
	return (0);
}
