/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 10:33:13 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/10 10:59:06 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	anihilation(char **splitted)
{
	int	i;

	i = 0;
	while (splitted[i])
		free(splitted[i++]);
	free(splitted);
}

void	close_pipe(int outin[2])
{
	if (close(outin[0]) == -1)
	{
		perror("close outin[0]");
		exit(1);
	}
	if (close(outin[1]) == -1)
	{
		perror("close outin[1]");
		exit(1);
	}
}
