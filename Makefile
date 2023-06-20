# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eslamber <eslamber@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/04 16:30:03 by eslamber          #+#    #+#              #
#    Updated: 2023/06/20 17:00:48 by eslamber         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#
### Definitions of variables
#

# Compilation flags
FLAGS := -Wall -Werror -Wextra
CC := gcc

#
### Definition of variables
#

# Definition of lib
LIB := libft/libft.a
INC := -Ilibft

# Definition of project variables
NAME := pipex
HEADER := pipex.h

# Definition of files variables
SRC := main.c \
	   free_struc.c \
	   exec.c \
	   cmd_build.c \
	   errors.c
OBJ := $(SRC:%.c=.obj/%.o)

# Definition of bonus files variables
SRC_B := main_bonus_saved.c \
		 free_struc.c \
		 cmd_build.c \
		 here_doc.c \
		 errors.c \
		 errors_bonus.c
OBJ_B := $(SRC_B:%.c=.obj/%.o)

#
### Compilation rules
#

# Compilation
all: $(NAME)

bonus: $(OBJ_B)
	$(CC) $(FLAGS) $(OBJ_B) -o $(NAME) -L. $(LIB)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $@ -L. $(LIB)

.obj/%.o: %.c $(HEADER) $(LIB)
	$(CC) $(FLAGS) $(INC) -c $< -o $@

$(LIB): FORCE
	make -C libft

# Debug
debug:
	make -C libft debug
	make -C ./ "FLAGS = -Wall -Werror -Wextra -fsanitize=address \
	-fno-omit-frame-pointer"

val_deb:
	make -C libft valgrind_deb
	make -C ./ "FLAGS = -Wall -Werror -Wextra -g"

bonus_deb:
	make -C libft valgrind_deb
	make -C ./ "FLAGS = -Wall -Werror -Wextra -g" bonus

#
### Cleanup rules
#

clean:
	@rm -rfv $(OBJ) $(OBJ_B)
	make -sC libft clean

fclean:
	@rm -rfv $(OBJ) $(OBJ_B) $(NAME)
	make -sC libft fclean

re: fclean all

re_deb: fclean debug

re_val: fclean val_deb

re_bonus: fclean bonus

re_bonus_deb : fclean bonus_deb

FORCE:

.PHONY: all clean fclean re FORCE debug re_deb val_deb re_val bonus re_bonus 
.PHONY: re_bonus_deb
