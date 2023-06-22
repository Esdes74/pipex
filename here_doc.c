/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:40:39 by marvin            #+#    #+#             */
/*   Updated: 2023/06/22 12:33:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int     check_here_doc(char **av, t_pipex *struc);
static int    set_up_heredoc(char **av, t_pipex *struc);

int	prep_pipe(t_pipex *pip, int ac)
{
	int	i;

	i = 0;
	check_here_doc()
	pip->nb_proc = ac - 3 - pip->here_doc;
	pip->nb_pipe = pip->nb_proc - 1;
	pip->error = OK;
	pip->outin = (int **) ft_calloc(pip->nb_proc, sizeof(int *));
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
    // set_up_heredoc
	return (0);
}

static int     check_here_doc(char **av, t_pipex *struc)
{
    char    *buf;

    buf = "here_doc";
    if (ft_strncmp(av[1], buf, 9) == 0)
		return (struc->here_doc = 1, 1);
    return (0);
}

static int    set_up_heredoc(char **av, t_pipex *struc)
{
    // int	temp;
    int savedStdout;
    char *str;
    char *buf;
    int lenght;
    
    // temp = open(HERE_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644); // O_APPEND permet de dire "met à la suite"
    // if (temp == -1)
	// 	errors_bonus(OPEN_HERE_DOC);
    while (1)
    {
        savedStdout = dup(STDOUT_FILENO);                   // enregistre la vrai sortie strandard, parce qu'on va la modifier et faudra la remettre après
        if (savedStdout == -1)
            errors(DUP, NULL);
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
        if(dup2(savedStdout, STDOUT_FILENO) == -1)          //remet la sortie strandard pour afficher correctement le '>'
            errors(DUP, NULL);
        if (close(savedStdout) == -1)                       // on en a finis avec, ça recommencera avec le dup plus haut
            errors(CLOSE, NULL);
        str[lenght - 1] = '\0';                             //pour vraiment bien comparé, parce que tu met entrer donc '\n' en plus alors que av[2] lui finis par '\0
        if (ft_strncmp(av[2], str, lenght) == 0)            // ici il faut dup dans le premier pipe dcp. en 1/0 ou le récuéré plus tard et l'assimiler à infile
        {
            free(buf);
            free(str);
            // if (close(temp) == -1)
            //     errors(CLOSE, NULL);
            return(0);
        }
        free(str);
        if (write(struc->outin[0][0], buf, ft_strlen(buf)) == -1)                  // on écris le résultat dans le temp (grâce  la manière qu'on la open, ça écris direct à la fin)
            return(free(buf), errors_bonus(WRITE), 1);
        free(buf);
    }
    return (0);
}
