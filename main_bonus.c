/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:59:04 by marvin            #+#    #+#             */
/*   Updated: 2023/06/20 17:22:18 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	main(int ac, char **av, char *environ[])
{
	int		id[ac - 3];
	int		nb_pipe;
	int		nb_proc;
	int		outin[ac - 4][2];
	int		i;
	int		j;
	int		infile;
	int		outfile;
	char	**splitted;
	char	*cmd;
	int		x;

	x = check_here_doc(av);
	if (x <= 0) // je regarde si le premier argument est here_doc
		x += 1;
	else
		return (1);
	if (ac >= 5 && environ != NULL)
	{
		i = 0;
		nb_proc = ac - 3;
		nb_pipe = nb_proc - 1;
		while (i < nb_pipe) // Création du tableau de pipe
		{
			if (pipe(outin[i]) == -1)
				return (1);
			i++;
		}
		i = x;
		while (i < nb_proc) // Boucle des multiples processeurs
							// On commence a la première commande
		{
			id[i] = fork();
			if (id[i] == -1)
				return (perror("fork"), 1);
			if (id[i] == 0) // child
			{
				if (i == 0 || x == 1) // Si on est dans le premier processeur alors on 
							// récupère le fichier d'entrer
				{
					if (x == 0)
						infile = open(av[1], O_RDONLY);
					if (x == 1)
					{
						infile = open(".here_doc.tmp", O_RDONLY); //si here_doc exite ouvre le dossier temporaire
					}
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
					return (perror("build 1"), anihilation(splitted), 1);
				j = 0;
				while (j < nb_pipe)
				{
					if (close(outin[j][0]) == -1)
						return (perror("close outin"), 1);
					if (close(outin[j][1]) == -1)
						return (perror("close outin"), 1);
					j++;
				}
				if (execve(cmd, splitted, environ) == -1) // Ne fonctionne pas
					perror("Child : command not found");
				return (free(cmd), anihilation(splitted), 0);
			}
			i++;
		}
		j = 0;
		while (j < nb_pipe)
		{
			if (close(outin[j][0]) == -1)
				return (perror("close outin parent 0"), 1);
			if (close(outin[j][1]) == -1)
				return (ft_printf("j = %d\n", j), perror("close outin parent 1"), 1);
			j++;
		}
		while (wait(NULL) > 0);
		if (x == 1)
		{
			if (unlink(".here_doc.tmp") == -1)
				return (perror("unlink"), 1);
		}
	}
	else
		ft_printf("Error : You don't have enought parameters.\n Or the \
environnement is NULL\n");
	return (0);
}
