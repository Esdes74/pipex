/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 17:21:42 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/09 23:37:14 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	anihilation(char **splitted)
{
	int	i;

	i = 0;
	while (splitted[i])
		free(splitted[i++]);
	free(splitted);
}

static char	*cmd_build(char *str, char **env)
{
	char	*new;
	char	**path;
	size_t	i;

	if (ft_in('/', str) == 1)
		return (str);
	i = 0;
	new = NULL;
	while (env[i])
	{
		new = ft_strnstr(env[i], "PATH=", ft_strlen(env[i]));
		if (new != NULL)
			break;
		i++;
	}
	new += ft_strlen("PATH=");
	path = ft_split(new, ':');
	if (path == NULL)
		return (perror("PATH"), NULL); // Vérifié jusque la, le path retourné est ok
									   // attention a bien anihiler path a la fin de son utilisation
	/* new = NULL; */
	/* size = 6 + ft_strlen(str); */
	/* new = (char *) malloc(sizeof(char) * size); */
	/* if (new == NULL) */
	/* 	return (NULL); */
	/* i = 5; */
	/* j = 0; */
	/* new[0] = '/'; */
	/* new[1] = 'b'; */
	/* new[2] = 'i'; */
	/* new[3] = 'n'; */
	/* new[4] = '/'; */
	/* while (j < ft_strlen(str)) */
	/* 	new[i++] = str[j++]; */
	/* new[i] = '\0'; */
	return (NULL);
}

static int	child(int outin[2], char **av, char **environ)
{
	char	*cmd;
	char	**splitted;
	int		infile;

	close(outin[0]);
	infile = open(av[1], O_RDONLY);
	if (infile == -1)
		ft_printf("Error : Wrong opening infile.\n");
	if (dup2(infile, STDIN_FILENO) == -1)
		ft_printf("Error : Probleme with first dup programm in child\n");
	if (dup2(outin[1], STDOUT_FILENO) == -1)
		ft_printf("Error : Probleme with second dup programm in child\n");
	close(infile);
	splitted = ft_split(av[2], ' ');
	if (splitted == NULL)
		return (1);
	cmd = cmd_build(splitted[0], environ);
	if (cmd == NULL)
		return (anihilation(splitted), 1);
	if (execve(cmd, splitted, environ) == -1)
		perror("Child : command not found");
	return (free(cmd), anihilation(splitted), 0);
}

static int	parent(int outin[2], char **av, char **environ)
{
	char	*cmd;
	char	**splitted;
	int		outfile;

	if (wait(NULL) == -1)
		return (1);
	close(outin[1]);
	outfile = open(av[4], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR + S_IWUSR + \
			S_IRGRP + S_IROTH);
	if (outfile < 0)
		ft_printf("Error : Wrong opening outfile.\n");
	if (dup2(outin[0], STDIN_FILENO) == -1)
		ft_printf("Error : Problem with first dup programme in parent.\n");
	if (dup2(outfile, STDOUT_FILENO) == -1)
		ft_printf("Error : Problem with second dup programme in parent.\n");
	close(outfile);
	splitted = ft_split(av[3], ' ');
	if (splitted == NULL)
		return (1);
	cmd = cmd_build(splitted[0], environ);
	if (cmd == NULL)
		return (anihilation(splitted), 1);
	if (execve(cmd, splitted, environ) == -1)
		perror("Parent : command not found");
	return (free(cmd), anihilation(splitted), 0);
}

int	main(int ac, char **av, char *environ[])
{
	int		id;
	int		outin[2];

	if (ac == 5 && environ != NULL)
	{
		if (pipe(outin) == -1)
			ft_printf("Error : There is a probleme with the pipe system.\n");
		id = fork();
		if (id == 0)
		{
			if (child(outin, av, environ) == 1)
				return (1);
		}
		else if (id > 0)
		{
			if (parent(outin, av, environ) == 1)
				return (1);
		}
		else
			ft_printf("Error : There is a problem with the fork system.\n");
	}
	else
		ft_printf("Error : You don't have enought parameters.\n Or the \
environnement is NULL\n");
	return (0);
}
