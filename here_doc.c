/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 14:40:39 by marvin            #+#    #+#             */
/*   Updated: 2023/06/20 17:13:38 by eslamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int    set_up_dup_to_temporary(int temp);
static int    set_up_heredoc(char **av);

int     check_here_doc(char **av)
{
    char    *buf;

    buf = "here_doc";
    if (ft_strncmp(av[1], buf, 9) == 0)
		return (set_up_heredoc(av));
    return (-1);
}

static int    set_up_heredoc(char **av)
{
    int	temp;
    int savedStdout;
    char *str;
    char *buf;
    int lenght;
    
    temp = open(HERE_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644); // O_APPEND permet de dire "met à la suite"
    if (temp == -1)
		errors_bonus(OPEN_HERE_DOC);
    while (1)
    {
        savedStdout = dup(STDOUT_FILENO);                   // enregistre la vrai sortie strandard, parce qu'on va la modifier et faudra la remettre après
        if (savedStdout == -1)
            errors(DUP, NULL);
        if (set_up_dup_to_temporary(temp) == 1)
            return(1);
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
            if (close(temp) == -1)
                errors(CLOSE, NULL);
            return(0);
        }
        free(str);
        if (write(temp, buf, ft_strlen(buf)) ==-1)                  // on écris le résultat dans le temp (grâce  la manière qu'on la open, ça écris direct à la fin)
            return(free(buf), errors_bonus(WRITE), 1);
        free(buf);
    }
    return (0);
}

// using dup2 to put the standard output on our temporary file,and writing like bash does when he needed a string

static int    set_up_dup_to_temporary(int temp)
{
    if (write(STDOUT_FILENO, "> ", 2) == -1)
        errors_bonus(WRITE);
    if (dup2(temp, STDOUT_FILENO) == -1)
        errors(DUP, NULL);
    return (0);
}
