/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus_saved.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:59:04 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/22 12:28:47 by eslamber         ###   ########.fr       */
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
			return (errors(CLOSE_P0, "0"), anihilation((char **) struc->outin), 1);
		}
		if (close(struc->outin[j][1]) == -1)
		{
			struc->error = CLOSE_P1;
			return (errors(CLOSE_P1, "0"), anihilation((char **) struc->outin), 1);
		}
		j++;
	}
	anihilation((char **) struc->outin);
	return (0);
}

static int	exec_child(t_pipex *pip, int ac, char **av, char **environ)
{
	int		infile;
	int		outfile;
	char	**splitted;
	char	*cmd;

	if (pip->ind_child == 0 && pip->here_doc == 0) // Si on est dans le premier processeur alors on 
			// récupère le fichier d'entrer
	{
		infile = open(av[1], O_RDONLY);
		if (infile == -1)
			return (errors(OPEN, "0"), 1);
		if (dup2(infile, STDIN_FILENO) == -1) // Récup donnée
			return (errors(DUP, "0"), 1);
		if (close(infile) == -1)
			return (errors(CLOSE, "0"), 1);
	}
	else
	{
		if (dup2(pip->outin[pip->ind_child - 1][0], STDIN_FILENO) == -1) // Récup donnée
			return (errors(DUP, "0"), 1);
	}
	if (pip->ind_child == pip->nb_pipe) // si on est dans le dernier on met le OUT en outfile
	{
		outfile = open(av[ac - 1], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR + \
				S_IWUSR + S_IRGRP + S_IROTH);
		if (outfile < 0)
			return (errors(OPEN, "0"), 1);
		if (dup2(outfile, STDOUT_FILENO) == -1)
			return (errors(DUP, "0"), 1);
		if (close(outfile) == -1)
			return (errors(CLOSE, "0"), 1);
	}
	else
	{
		if (dup2(pip->outin[pip->ind_child][1], STDOUT_FILENO) == -1) // Récup donnée
			return (errors(DUP, "0"), 1);
	}
	splitted = ft_split(av[pip->ind_child + pip->here_doc + 2], ' '); // Execution de la commande
	if (splitted == NULL)
		return (errors(SPLIT, "0"), 1);
	cmd = cmd_build(splitted[0], environ);
	if (cmd == NULL)
		return (errors(CMD, splitted[0]), anihilation(splitted), 1);
	if (close_all_pipes(pip) == 1)
		return (free(cmd), anihilation(splitted), 2);
	execve(cmd, splitted, environ);
	errors(EXEC, "0");
	return (free(cmd), anihilation(splitted), 2);
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
	int		test;

	if (ac >= 5 && environ != NULL)
	{
		if (prep_pipe(&struc, ac) == 1)
			return (1);
		struc.ind_child = -1;
		while (++struc.ind_child < struc.nb_proc)
		{
			test = exec(&struc, ac, av, environ);
			if (test == 2)
				return (1);
			else if (test == 1)
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
	if (struc->here_doc == 1)
		if (unlink(HERE_FILE) == -1)
			errors_bonus(UNLINK);
	if (struc->error != OK)
		return (1);
	return (0);
}
