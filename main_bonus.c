/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:59:04 by marvin            #+#    #+#             */
/*   Updated: 2023/06/07 15:59:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*cmd_build(char *str)
{
	char	*new;
	size_t	i;
	size_t	j;
	size_t	size;

	if (ft_in('/', str) == 1)
		return (str);
	new = NULL;
	size = 6 + ft_strlen(str);
	new = (char *) malloc(sizeof(char) * size);
	if (new == NULL)
		return (NULL);
	i = 5;
	j = 0;
	new[0] = '/';
	new[1] = 'b';
	new[2] = 'i';
	new[3] = 'n';
	new[4] = '/';
	while (j < ft_strlen(str))
		new[i++] = str[j++];
	new[i] = '\0';
	return (new);
}

static void	anihilation(char **splitted)
{
	int	i;

	i = 0;
	while (splitted[i] != NULL)
		free(splitted[i++]);
	free(splitted);
}

int	main(int ac, char **av, char *environ[])
{
	int		id[ac - 4];
	int		nb_proc;
	int		outin[ac - 4][2];
	int		i;
	int		j;
	int		infile;
	int		outfile;
	char	**splitted;
	char	*cmd;

	if (ac >= 5 && environ != NULL)
	{
		i = 0;
		nb_proc = ac - 4; // 2
		while (i < nb_proc) // Création du tableau de pipe
		{
			if (pipe(outin[i]) == -1) // 0 et 1
				return (1);
			i++;
		}
		i = 0;
		while (i < nb_proc) // Boucle des multiples processeurs
							// On commence a la première commande
		{
			// ft_printf("i = %d\n", i);
			// if (i == 0 || )
			id[i] = fork(); // 2 prog
			// ft_printf("pid = %d, iden = %d\n", id[i], getpid());
			if (id[i] == -1)
				return (perror("fork"), 1);
			if (id[i] == 0) // child
			{
				if (i != 0)
					sleep(1);
				// j = 0;
				// while (j < nb_proc) // Close les pipes
				// {
				// 	// if (i == 0)
				// 	// 	close(outin[1][0]);
				// 	if (i != j)
				// 		if (close(outin[j][1]) == -1)
				// 			return (perror("close 1"), 1);
				// 	if (j + 1 != i)
				// 		if (close(outin[j][0]) == -1)
				// 			return (perror("close 2"), 1);
				// 	j++;
				// }
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
				 	ft_printf_fd(STDERR_FILENO, "infile est mis en IN\n");
				}
				else
				{
					if (dup2(outin[i - 1][0], STDIN_FILENO) == -1) // Récup donnée
						return (perror("dup outin[i - 1][0]"), 1);
				 	ft_printf_fd(STDERR_FILENO, "outin[%d][0] est mis en IN\n", i - 1);
				}
				if (dup2(outin[i][1], STDOUT_FILENO) == -1) // Récup donnée
					return (perror("dup outin[i][1]"), 1);
				ft_printf_fd(STDERR_FILENO, "outin[%d][1] est mis en OUT\n", i);
				splitted = ft_split(av[i + 2], ' '); // Execution de la commande
				if (splitted == NULL)
					return (perror("split"), 1);
				cmd = cmd_build(splitted[0]);
				if (cmd == NULL)
					return (perror("build"), free(splitted), 1);
				// ft_printf("cmd = %s\n", cmd);
				j = 0;
				while (j < nb_proc)
				{
					if (close(outin[j][0]) == -1)
						return (perror("close outin"), 1);
					if (close(outin[j][1]) == -1)
						return (perror("close outin"), 1);
					j++;
				}
				// if (i != 0)
				// {
				// 	j = 0;
				// 	while (environ[j])
				// 	{
				// 		ft_printf_fd(STDERR_FILENO, "environ[%d] = %s\n", j, environ[j]);
				// 		j++;
				// 	}
				// }
				// if (i != 0)
				// 	return (1);
				if (execve(cmd, splitted, environ) == -1) // Ne fonctionne pas
					perror("Child : command not found");
				return (free(cmd), anihilation(splitted), 0);
			}
			i++;
		}
		i = 0;
		while (i < nb_proc)
		{
			if (wait(NULL) == -1)
				return (perror("wait"), 1);
			i++;
		}
		// sleep(2);
		// j = 0;
		// while (j < nb_proc) // Close les pipes
		// {
		// 	if (nb_proc - 1 != j)
		// 		if (close(outin[j][0]) == -1)
		// 			return (ft_printf("pipe[%d]\n", j), perror("close main"), 1);
		// 	j++;
		// }
		outfile = open(av[ac - 1], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR + S_IWUSR + \
				S_IRGRP + S_IROTH);
		if (outfile < 0)
			return (ft_printf("Error : Wrong opening outfile.\n") , 1);
		if (dup2(outin[nb_proc - 1][0], STDIN_FILENO) == -1)
		{
			ft_printf("nb_proc = %d\n", nb_proc - 1);
			ft_printf("Error : Problem with first dup programme in parent.\n");
			return (perror("dup2"), 1);
		}
		ft_printf_fd(STDERR_FILENO, "outin[%d][0] est mis en IN\n", nb_proc - 1);
		if (dup2(outfile, STDOUT_FILENO) == -1)
			return (ft_printf("Error : Problem with second dup programme in parent.\n"), 1);
		ft_printf_fd(STDERR_FILENO, "outfile est mis en OUT\n");
		if (close(outfile) == -1)
			return (perror("close outfile"), 1);
		splitted = ft_split(av[ac - 2], ' ');
		if (splitted == NULL)
			return (1);
		cmd = cmd_build(splitted[0]);
		if (cmd == NULL)
			return (free(splitted), 1);
		j = 0;
		while (j < nb_proc)
		{
			if (close(outin[j][0]) == -1)
				return (perror("close outin parent 0"), 1);
			if (close(outin[j][1]) == -1)
				return (ft_printf("j = %d\n", j), perror("close outin parent 1"), 1);
			j++;
		}
		if (execve(cmd, splitted, environ) == -1)
			 perror("Parent : command not found");
		return (free(cmd), anihilation(splitted), 0);
	}
	else
		ft_printf("Error : You don't have enought parameters.\n Or the \
environnement is NULL\n");
	return (0);
}
