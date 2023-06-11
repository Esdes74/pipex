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

static char	*cmd_build(char *str, char **env);
static char	*search_command(char *str, char *new);
int	exec_parent(int outin[2], char **av, char **environ);

int	exec_child(int outin[2], char **av, char **environ)
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

int	exec_parent(int outin[2], char **av, char **environ)
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

static char	*cmd_build(char *str, char **env)
{
	char	*new;
	char	*cmd;
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
	cmd = search_command(str, new);
	if (cmd == NULL)
		return (NULL);
	return (cmd);
}

static char	*search_command(char *str, char *new)
{
	char	**path;
	char	*cmd;
	int		i;

	path = ft_split(new, ':');
	if (path == NULL)
		return (perror("PATH"), NULL);
	i = 0;
	while (path[i])
	{
		new = ft_strjoin(path[i], "/");
		if (new == NULL)
			return (anihilation(path), perror("join new"), NULL);
		cmd = ft_strjoin(new, str);
		free(new);
		new = NULL;
		if (cmd == NULL)
			return (anihilation(path), perror("join cmd"), NULL);
		if (access(cmd, F_OK | X_OK) == 0)
			return (anihilation(path), cmd);
		free(cmd);
		cmd = NULL;
		i++;
	}
	return (anihilation(path), free(cmd), NULL);
}
