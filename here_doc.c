/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:40:39 by marvin            #+#    #+#             */
/*   Updated: 2023/06/10 14:40:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int    set_up_dup_to_temporary(int temp);

int     check_here_doc(char *av)
{
    char    *buf;
    int     x;

    buf = "here_doc";
    x = ft_strncmp(av, buf, 9);
    return (x);
}

int    set_up_heredoc(char **av)
{
    int	temp;
    int savedStdout;
    char *str;
    char *buf;
    int lenght;
    
    temp = open(".here_doc.tmp", O_WRONLY | O_APPEND | O_CREAT, 0644); // O_APPEND permet de dire "met à la suite"
    if (temp == -1)
        return(perror("Error, from opening a new temporary file for here_doc"), -1);
    while (1)
    {
        savedStdout = dup(STDOUT_FILENO);                   // enregistre la vrai sortie strandard, parce qu'on va la modifier et faudra la remettre après
        if (savedStdout == -1)
            return(perror("dup from STDOUT_FILENO"), -1);
        if (set_up_dup_to_temporary(temp) == -1)
            return(-1);
        buf = get_next_line(STDIN_FILENO);                  //récupère la phrase envoyer dans le STDIN
        if (buf == NULL)
            return(perror("get_next_line"), -1);
        lenght = ft_strlen(buf);
        str = ft_strdup(buf);
        if (str == NULL)
            return(perror("strdup"), -1);
        if(dup2(savedStdout, STDOUT_FILENO) == -1)          //remet la sortie strandard pour afficher correctement le '>'
            return(perror("dup2 from temporary file to STOUT_FILENO"), -1);
        if (close(savedStdout) == -1)                       // on en a finis avec, ça recommencera avec le dup plus haut
            return (perror("close savedStdout"), -1);
        str[lenght - 1] = '\0';                             //pour vraiment bien comparé, parce que tu met entrer donc '\n' en plus alors que av[2] lui finis par '\0
        if (ft_strncmp(av[2], str, lenght) == 0)            // ici il faut dup dans le premier pipe dcp. en 1/0 ou le récuéré plus tard et l'assimiler à infile
        {
            free(buf);
            free(str);
            if (close(temp) == -1)
                return(perror("closing the temporary file") , -1);
            return(0);
        }
        free(str);
        if (write(temp, buf, ft_strlen(buf)) ==-1)                  // on écris le résultat dans le temp (grâce  la manière qu'on la open, ça écris direct à la fin)
            return(free(buf), perror("writing in the temporary file"), -1);
        free(buf);
    }
    return (0);
}

// using dup2 to put the standard output on our temporary file,and writing like bash does when he needed a string

static int    set_up_dup_to_temporary(int temp)
{
    if (write(STDOUT_FILENO, "> ", 2) == -1)
        return(perror("writing on STDOUT"), -1);
    if (dup2(temp, STDOUT_FILENO) == -1)
        return(perror("dup2 from STDOUT_FILENO to temporary file"), -1);
    return (0);
}