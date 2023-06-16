/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 10:48:21 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/11 10:55:10 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	exec_child(int outin[2], char **av, char **environ);
static int	exec_parent(int outin[2], char **av, char **environ);

int	child(int outin[2], char **av, char **environ)
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

int	parent(int outin[2], char **av, char **environ)
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

static int	exec_child(int outin[2], char **av, char **environ)
{
	char	*cmd;
	char	**splitted;
	int		infile;

	infile = open(av[1], O_RDONLY);
	if (infile == -1)
		ft_printf("Error : Wrong opening infile.\n");
	if (dup2(infile, STDIN_FILENO) == -1)
		ft_printf("Error : Probleme with first dup programm in child\n");
	if (dup2(outin[1], STDOUT_FILENO) == -1)
		ft_printf("Error : Probleme with second dup programm in child\n");
	if (close(infile) == -1)
		return (perror("close infile"), 1);
	splitted = ft_split(av[2], ' ');
	if (splitted == NULL)
		return (1);
	cmd = cmd_build(splitted[0], environ);
	if (cmd == NULL)
		return (perror("child : command not found"), anihilation(splitted), 1);
	close_pipe(outin);
	if (execve(cmd, splitted, environ) == -1)
		perror("Child : command not found");
	return (free(cmd), anihilation(splitted), 1);
}

static int	exec_parent(int outin[2], char **av, char **environ)
{
	char	*cmd;
	char	**splitted;
	int		outfile;

	outfile = open(av[4], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR + S_IWUSR + \
			S_IRGRP + S_IROTH);
	if (outfile < 0)
		ft_printf("Error : Wrong opening outfile.\n");
	if (dup2(outin[0], STDIN_FILENO) == -1)
		ft_printf("Error : Problem with first dup programme in parent.\n");
	if (dup2(outfile, STDOUT_FILENO) == -1)
		ft_printf("Error : Problem with second dup programme in parent.\n");
	if (close(outfile) == -1)
		return (perror("close outfile"), 1);
	splitted = ft_split(av[3], ' ');
	if (splitted == NULL)
		return (1);
	cmd = cmd_build(splitted[0], environ);
	if (cmd == NULL)
		return (perror("parent : command not found"), anihilation(splitted), 1);
	close_pipe(outin);
	if (execve(cmd, splitted, environ) == -1)
		perror("Parent : command not found");
	return (free(cmd), anihilation(splitted), 1);
}
