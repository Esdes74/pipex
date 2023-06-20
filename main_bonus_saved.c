/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus_saved.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:59:04 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/20 17:30:21 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	close_all_pipes(t_pipex *struc)
{
	int	j;

	j = 0;
	while (j < struc->nb_pipe)
	{
		if (close(struc->outin[j][0]) == -1)
		{
			struc->error = CLOSE_P0;
			return (errors(CLOSE_P0, "0"), 1);
		}
		if (close(struc->outin[j][1]) == -1)
		{
			struc->error = CLOSE_P1;
			return (errors(CLOSE_P1, "0"), 1);
		}
		j++;
	}
	return (0);
}

static int	exec_child(t_pipex *pip, int ac, char **av, char **environ)
{
	int		infile;
	int		outfile;
	char	**splitted;
	char	*cmd;

	if (pip->ind_child == 0) // Si on est dans le premier processeur alors on 
			// récupère le fichier d'entrer
	{
		infile = open(av[1], O_RDONLY);
		if (infile == -1)
			return (anihilation((char **) pip->outin), errors(OPEN, NULL), 1);
		if (dup2(infile, STDIN_FILENO) == -1) // Récup donnée
			return (anihilation((char **) pip->outin), errors(DUP, NULL), 1);
		if (close(infile) == -1)
			return (anihilation((char **) pip->outin), errors(CLOSE, NULL), 1);
	}
	else
	{
		if (dup2(pip->outin[pip->ind_child - 1][0], STDIN_FILENO) == -1) // Récup donnée
			return (anihilation((char **) pip->outin), errors(DUP, NULL), 1);
	}
	if (pip->ind_child == pip->nb_pipe) // si on est dans le dernier on met le OUT en outfile
	{
		outfile = open(av[ac - 1], O_CREAT | O_RDWR | O_TRUNC, \
		S_IRUSR + S_IWUSR + S_IRGRP + S_IROTH);
		if (outfile < 0)
			return (anihilation((char **) pip->outin), errors(OPEN, NULL), 1);
		if (dup2(outfile, STDOUT_FILENO) == -1)
			return (anihilation((char **) pip->outin), errors(DUP, NULL), 1);
		if (close(outfile) == -1)
			return (anihilation((char **) pip->outin), errors(CLOSE, NULL), 1);
	}
	else
	{
		if (dup2(pip->outin[pip->ind_child][1], STDOUT_FILENO) == -1) // Récup donnée
			return (anihilation((char **) pip->outin), errors(DUP, NULL), 1);
	}
	splitted = ft_split(av[pip->ind_child + 2], ' '); // Execution de la commande
	if (splitted == NULL)
		return (anihilation((char **) pip->outin), errors(SPLIT, NULL), 1);
	cmd = cmd_build(splitted[0], environ);
	if (cmd == NULL)
	{
		anihilation((char **) pip->outin);
		return (errors(CMD, splitted[0]), anihilation(splitted), 1);
	}
	if (close_all_pipes(pip) == 1)
	{
		anihilation((char **) pip->outin);
		return (free(cmd), anihilation(splitted), 2);
	}
	if (execve(cmd, splitted, environ) == -1)
		errors(EXEC, "0");
	anihilation((char **) pip->outin);
	return (free(cmd), anihilation(splitted), 2);
}

static int	prep_pipe(t_pipex *pip, int ac)
{
	int	i;

	i = 0;
	pip->nb_proc = ac - 3;
	pip->nb_pipe = pip->nb_proc - 1;
	pip->error = OK;
	pip->outin = (int **) ft_calloc(pip->nb_proc, sizeof(int*));
	if (pip->outin == NULL)
		return (ft_printf_fd(2, "Error : Problem with callocfunction\n"), 1);
	while (i < pip->nb_pipe) // Création du tableau de pipe
	{
		pip->outin[i] = (int *) ft_calloc(2, sizeof(int));
		if (pip->outin[i] == NULL)
		{
			pip->nb_pipe = i;
			ft_printf_fd(2, "Error : Problem with calloc function\n");
			return (anihilation((char **) pip->outin), close_all_pipes(pip), 1);
		}
		if (pipe(pip->outin[i]) == -1)
		{
			pip->nb_pipe = i;
			return (close_all_pipes(pip), errors(PIPE, NULL), 1);
		}
		i++;
	}
	return (0);
}

static int	exec(t_pipex *struc, int ac, char **av, char **environ)
{
	int	id;

	id = fork();
	if (id == -1)
	{
		struc->error = FORK;
		return (errors(FORK, "0"), 1);
	}
	if (id == 0) // child
		return (exec_child(struc, ac, av, environ));
	return (0);
}

static int	end(t_pipex *struc);

int	main(int ac, char **av, char *environ[])
{
	t_pipex	struc;

	struc.here_doc = check_here_doc(av);
	if (ac >= 5 && environ != NULL && struc.here_doc++ <= 0)
	{
		if (prep_pipe(&struc, ac) == 1)
			return (1);
		struc.ind_child = 0;
		while (struc.ind_child++ < struc.nb_proc)
		{
			if (exec(&struc, ac, av, environ) == 2)
				return (1);
			else
				break;
		}
		if (close_all_pipes(&struc) == 1)
			return (1);
		while (wait(NULL) > 0);
	}
	else
		errors(CONDITIONS, NULL);
	return (end(&struc));
}

static int	end(t_pipex *struc)
{
	anihilation((char **) struc->outin);
	if (struc->here_doc == 1)
		if (unlink(".here_doc.tmp") == -1)
			errors_bonus(UNLINK);
	if (struc->error != OK)
		return (1);
	return (0);
}