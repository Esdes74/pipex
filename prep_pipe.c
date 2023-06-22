/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:40:39 by marvin            #+#    #+#             */
/*   Updated: 2023/06/22 19:18:05 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void		check_here_doc(char **av, t_pipex *struc, int ac);
static int		set_up_heredoc(char **av, t_pipex *struc);

int	prep_pipe(t_pipex *pip, char **av, int ac)
{
	int	i;

	i = 0;
	check_here_doc(av, pip, ac);
	if (pip->outin == NULL)
		return (ft_printf_fd(2, "Error : Problem with calloc function\n"), 1);
	while (i < pip->nb_pipe) // Création du tableau de pipe
	{
		pip->outin[i] = (int *) ft_calloc(2, sizeof(int));
		if (pip->outin[i] == NULL)
		{
			pip->nb_pipe = i;
			ft_printf_fd(2, "Error : Problem with calloc function\n");
			return (close_all_pipes(pip), 1);
		}
		if (pipe(pip->outin[i]) == -1)
		{
			pip->nb_pipe = i;
			return (close_all_pipes(pip), errors(PIPE, NULL), 1);
		}
		i++;
	}
	if (pip->here_doc == 1)
		set_up_heredoc(av, pip);
	return (0);
}

static void	check_here_doc(char **av, t_pipex *struc, int ac)
{
    if (ft_strncmp(av[1], "here_doc", ft_strlen("here_doc")) == 0)
		struc->here_doc = 1;
	else
		struc->here_doc = 0;
	struc->nb_proc = ac - 3 - struc->here_doc;
	struc->nb_pipe = struc->nb_proc - 1 + struc->here_doc;
	struc->error = OK;
	struc->outin = (int **) ft_calloc(struc->nb_pipe + 1, sizeof(int *));
}

static int	set_up_heredoc(char **av, t_pipex *struc)
{
    // int	temp;
    /* int savedStdout; */
    char *str;
    char *buf;
    int lenght;

	str = ft_strdup("\0");
	buf = ft_strdup("\0");
	lenght = 1;
    // temp = open(HERE_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644); // O_APPEND permet de dire "met à la suite"
    // if (temp == -1)
	// 	errors_bonus(OPEN_HERE_DOC);
    while (ft_strncmp(av[2], str, lenght) != 0)
    {
		free(str);
		if (buf != NULL && write(struc->outin[0][1], buf, ft_strlen(buf)) == -1)
            return(close_all_pipes(struc), free(buf), errors_bonus(WRITE), 1);
		if (buf != NULL)
			free(buf);
        /* savedStdout = dup(STDOUT_FILENO);                   // enregistre la vrai sortie strandard, parce qu'on va la modifier et faudra la remettre après */
        /* if (savedStdout == -1) */
        /*     errors(DUP, NULL); */
        if (write(STDOUT_FILENO, "> ", 2) == -1)
            errors_bonus(WRITE);
        // if (set_up_dup_to_temporary(temp) == 1)
        //     return(1);
        buf = get_next_line(STDIN_FILENO);                  //récupère la phrase envoyer dans le STDIN
        if (buf == NULL)
            errors_bonus(GNL);
        lenght = ft_strlen(buf);
        str = ft_strdup(buf);
        if (str == NULL)
            errors_bonus(STRDUP);
        /* if(dup2(savedStdout, STDOUT_FILENO) == -1)          //remet la sortie strandard pour afficher correctement le '>' */
        /*     errors(DUP, NULL); */
        /* if (close(STDOUT_FILENO) == -1)                       // on en a finis avec, ça recommencera avec le dup plus haut */
        /*     errors(CLOSE, NULL); */
		str[lenght - 1] = '\0';                             //pour vraiment bien comparé, parce que tu met entrer donc '\n' en plus alors que av[2] lui finis par '\0
        /* if (ft_strncmp(av[2], str, lenght) == 0)            // ici il faut dup dans le premier pipe dcp. en 1/0 ou le récuéré plus tard et l'assimiler à infile */
        /* { */
        /*     free(buf); */
        /*     free(str); */
        /*     // if (close(temp) == -1) */
        /*     //     errors(CLOSE, NULL); */
        /*     return(0); */
        /* } */
        /* free(str); */
    }
	free(str);
	free(buf);
    return (0);
}
