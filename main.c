/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 17:21:42 by eslamber          #+#    #+#             */
/*   Updated: 2023/05/08 12:33:24 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char **av, char **env)
{
	int	id;
	int	outin[2];
	int	infile;
	int	outfile;

	if (pipe(outin) == -1)
		ft_printf("Error : There is a probleme with the pipe system\n");
	id = fork();
	if (ac == 5)
	{
		if (id == 0) // child
		{
			infile = open(av[1], O_RDONLY);
			if (infile == -1)
				ft_printf("Error : Wrong opening infile.\n");
			dup2(); // TODO :
		}
		else if (id > 0) // parent
						 // TODO :
		else
			ft_printf("Error : There is a problem with the fork system.\n");
	}
	else
		ft_printf("Error : You don't have enought parameters.\n");
	return (0);
}
