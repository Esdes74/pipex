/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 09:35:27 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/20 17:29:13 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "libft/libft.h"
# include <unistd.h>
# include <sys/wait.h>
# include <stdio.h>

typedef enum e_errors{
	OK,
	PIPE,
	CONDITIONS,
	FORK,
	OPEN,
	DUP,
	CLOSE,
	SPLIT,
	CMD,
	EXEC,
	CLOSE_P0,
	CLOSE_P1,
	JOIN,
	OPEN_HERE_DOC,
	GNL,
	STRDUP,
	WRITE,
	UNLINK,
}	t_errors;

typedef struct s_pipex {
	int			**outin;
	int			ind_child;
	int			nb_pipe;
	int			nb_proc;
	int			here_doc;
	t_errors	error;
}	t_pipex;

void	errors(t_errors error, char *cmd);

void	errors_bonus(t_errors error);

void	close_pipe(int outin[2]);

void	anihilation(char **double_array);

char	*cmd_build(char *str, char **env);

int		child(int outin[2], char **av, char **environ);

int		parent(int outin[2], char **av, char **environ);

int     check_here_doc(char **av);

#endif
