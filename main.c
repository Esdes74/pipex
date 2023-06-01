/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 17:21:42 by eslamber          #+#    #+#             */
/*   Updated: 2023/06/01 16:47:00 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

static char	*cmd_build(char *str)
{
	char	*new;
	size_t	i;
	size_t	j;
	size_t	size;

	if (str[0] == '/' && str[1] == 'b' && str[2] == 'i' && str[3] == 'n'
			&& str[4] == '/')
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
	/* ft_printf("new = %s, i = %d\n", new, i); */
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

int	main(int ac, char **av, char **env)
{
	int		id;
	int		outin[2];
	int		infile;
	int		outfile;
	char	*cmd;
	char	**splitted;
	/* int	i = 0; */

	if (pipe(outin) == -1)
		ft_printf("Error : There is a probleme with the pipe system.\n");
	if (ac == 5 && env != NULL)
	{
		id = fork();
		if (id == 0) // child
		{
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
			cmd = cmd_build(splitted[0]);
			if (cmd == NULL)
				return (free(splitted), 1);
			/* while (test[i][0] != '\0') */
			/* 	i++; */
			/* free(test[i]); */
			/* test[i] = NULL; */
			if (execve(cmd, splitted, env) == -1)
				perror("Child : command not found");
				/* ft_printf("Error : Wrong child execution.\n"); */
			free(cmd);
			anihilation(splitted);
		}
		else if (id > 0) // parent
		{
			wait(NULL);
			close(outin[1]);
			outfile = open(av[4], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR + S_IWUSR + S_IRGRP + S_IROTH);
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
			cmd = cmd_build(splitted[0]);
			if (cmd == NULL)
				return (free(splitted), 1);
			/* i = 0; */
			/* while (test[i] != NULL) */
			/* 	i++; */
			/* free(test[i]); */
			/* test[i] = NULL; */
			/* ft_printf("cmd = %s\n", cmd); */
			if (execve(cmd, splitted, env) == -1)
				perror("Parent : command not found");
				/* ft_printf("Error : Wrong parent execution.\n"); */
			free(cmd);
			anihilation(splitted);
		}
		else
			ft_printf("Error : There is a problem with the fork system.\n");
	}
	else
		ft_printf("Error : You don't have enought parameters.\n Or the \
environnement is NULL\n");
	return (0);
}
