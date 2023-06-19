/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:59:04 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/17 09:43:58 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	close_all_pipes(int outin[][2], int nb_pipe)
{
	int	j;

	j = 0;
	while (j < nb_pipe)
	{
		if (close(outin[j][0]) == -1)
			return (perror("close outin"), 1);
		if (close(outin[j][1]) == -1)
			return (perror("close outin"), 1);
		j++;
	}
	return (0);
}

static int	exec_child(int outin[][2], int ac, char **av, char **environ)
{
	int		infile;
	int		outfile;
	char	**splitted;
	char	*cmd;
	int		i;

	if (i == 0) // Si on est dans le premier processeur alors on 
			// récupère le fichier d'entrer
	{
		infile = open(av[1], O_RDONLY);
		if (infile == -1)
			return (1);
		if (dup2(infile, STDIN_FILENO) == -1) // Récup donnée
			return (perror("dup(in, fileno)"), 1);
		if (close(infile) == -1)
			return (perror("close infile"), 1);
	}
	else
	{
		if (dup2(outin[i - 1][0], STDIN_FILENO) == -1) // Récup donnée
			return (perror("dup outin[i - 1][0]"), 1);
	}
	if (i == nb_pipe) // si on est dans le dernier on met le OUT en outfile
	{
		outfile = open(av[ac - 1], O_CREAT | O_RDWR | O_TRUNC, \
		S_IRUSR + S_IWUSR + S_IRGRP + S_IROTH);
		if (outfile < 0)
			return (ft_printf("Error : Wrong opening outfile.\n") , 1);
		if (dup2(outfile, STDOUT_FILENO) == -1)
			return (ft_printf("Error : Problem with second dup programme in parent.\n"), 1);
		if (close(outfile) == -1)
			return (perror("close outfile"), 1);
	}
	else
	{
		if (dup2(outin[i][1], STDOUT_FILENO) == -1) // Récup donnée
			return (perror("dup outin[i][1]"), 1);
	}
	splitted = ft_split(av[i + 2], ' '); // Execution de la commande
	if (splitted == NULL)
		return (perror("split"), 1);
	cmd = cmd_build(splitted[0], environ);
	if (cmd == NULL)
		return (perror("build"), anihilation(splitted), 1);
	if (close_all_pipes(outin, nb_pipe) == 1)
		return (perror("close pipes"), free(cmd), anihilation(splitted), 2);
	if (execve(cmd, splitted, environ) == -1)
		perror("Child : command not found");
	return (free(cmd), anihilation(splitted), 2);
}

static int	prep_pipe(t_pipex *struc)
{
	int	i;

	i = 0;
	struc->nb_proc = ac - 3;
	struc->nb_pipe = struc->nb_proc - 1;
	while (i < *nb_pipe) // Création du tableau de pipe
	{
		if (pipe(struc->outin[i]) == -1)
			return (close_all_pipes(struc->outin, i), 1);
		i++;
	}
	return (0);
}

static int	exec(int outin[][2], int ac, char **av, char **environ)
{
	int	id;

	id = fork();
	if (id == -1)
		return (perror("fork"), 1);
	if (id == 0) // child
		return (exec_child(outin, ac, av, environ));
	return (0);
}

int	main(int ac, char **av, char *environ[])
{
	int		outin[ac - 4][2];
	int		i;
	t_pipex	struc;

	if (ac >= 5 && environ != NULL)
	{
		struc.outin = outin;
		if (prep_pipe(&struc) == 1)
			return (1);
		i = 0;
		while (i++ < nb_proc) // Boucle des multiples processeurs
							// On commence a la première commande
		{
			if (exec(outin, ac, av, environ) == 2)
				return (1);
			else
				break;
		}
		if (close_all_pipes(outin, nb_pipe) == 1)
			return (perror("close pipes"), 1);
		while (wait(NULL) > 0);
	}
	else
		ft_printf("Error : You don't have enought parameters.\n Or the \
environnement is NULL\n");
	return (0);
}
