/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 16:31:47 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/10 10:59:24 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <unistd.h>
# include <sys/wait.h>
# include <stdio.h>

void	close_pipe(int outin[2]);

void	anihilation(char **double_array);

int		exec_child(int outin[2], char **av, char **env);

int		exec_parent(int outin[2], char **av, char **env);

#endif
