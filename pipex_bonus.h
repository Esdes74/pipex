/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 09:35:27 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/19 10:25:00 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "libft/libft.h"
# include <unistd.h>
# include <sys/wait.h>
# include <stdio.h>

typedef struct s_pipex {
	int	**outin;
	int	ind_child;
	int	nb_pipe;
	int	nb_proc;
}	t_pipex;

void	close_pipe(int outin[2]);

void	anihilation(char **double_array);

char	*cmd_build(char *str, char **env);

int		child(int outin[2], char **av, char **environ);

int		parent(int outin[2], char **av, char **environ);

int     check_here_doc(char *av);

int     set_up_heredoc(char **av);

#endif
